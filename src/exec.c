/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
/*
 * File: exec.h
 * Copyright: luke_biddell@yahoo.com
 * Created on: Wed Feb 26 00:33:10 2003
 */

#include "exec.h"
#include <sys/wait.h>
#include "gbcommon.h"

#if defined (__FreeBSD__)
#include <signal.h>
#endif


static gint child_child_pipe[2];


static void
exec_print_cmd(const ExecCmd *e)
{
    g_return_if_fail(e != NULL);
	if(show_trace)
	{
		gint i = 0;
		for(; i < e->args->len; i++)
			g_print("%s ", (gchar*)g_ptr_array_index(e->args, i));
		g_print("\n");		
	}
}


static void 
exec_set_outcome(Exec *e)
{
    GB_LOG_FUNC
    g_return_if_fail(e != NULL);
    
    e->outcome = COMPLETED;    
    GList *cmd = e->cmds;
    for(; cmd != NULL; cmd = cmd->next)
    {
        const ExecState state = exec_cmd_get_state((ExecCmd*)cmd->data);
        if(state == CANCELLED)
        {           
            e->outcome = CANCELLED;
            break;
        }
        else if(state == FAILED)
        {            
            e->outcome = FAILED;
            break;
        }
    }
}


static gboolean 
exec_channel_callback(GIOChannel *channel, GIOCondition condition, gpointer data)
{   
    GB_LOG_FUNC
    ExecCmd *cmd = (ExecCmd*)data;
    gboolean cont = TRUE;
    
    if((condition & G_IO_IN) || (condition & G_IO_PRI)) /* there's data to be read */
    {
        static const gint BUFF_SIZE = 1024;
        gchar buffer[BUFF_SIZE];
        memset(buffer, 0x0, BUFF_SIZE * sizeof(gchar));
        gsize bytes = 0;
        const GIOStatus status = g_io_channel_read_chars(channel, buffer, (BUFF_SIZE - 1) * sizeof(gchar), &bytes, NULL);  
        if ((status == G_IO_STATUS_ERROR) || (status == G_IO_STATUS_AGAIN)) /* need to check what to do for again */
        {
            GB_TRACE("exec_channel_callback - read error [%d]\n", status);
            cont = FALSE;
        }        
        else if(cmd->read_proc) 
        {
            GError *error = NULL;        
            gchar *converted = g_convert(buffer, bytes, "UTF-8", "ISO-8859-1", NULL, NULL, &error);
            if(converted != NULL)
            {
                cmd->read_proc(cmd, converted);
                g_free(converted);
            }
            else
            {
                if(error != NULL)
                {
                    g_warning("exec_channel_callback - conversion error [%s]", error->message);
                    g_error_free(error);
                }
                else
                    g_warning("exec_channel_callback - unknown conversion error");
                cmd->read_proc(cmd, buffer); 
            }
        }
    }
    
    if ((cont == FALSE) || (condition & G_IO_HUP) || (condition & G_IO_ERR) || (condition & G_IO_NVAL))
    {
        /* We assume a failure here (even on G_IO_HUP) as exec_spawn_process will 
         check the return code of the child to determine if it actually worked
         and set the correct state accordingly.*/
        exec_cmd_set_state(cmd, FAILED);
        GB_TRACE("exec_channel_callback - condition [%d]\n", condition);        
        cont = FALSE;
    }
    
    return cont;
}


static void
exec_spawn_process(ExecCmd *e, GSpawnChildSetupFunc child_setup)
{
	GB_LOG_FUNC
	g_return_if_fail(e != NULL);
	
    /* Make sure that the args are null terminated */
    g_ptr_array_add(e->args, NULL);
	exec_print_cmd(e);
	gint std_out = 0, std_err = 0;		
    GError *err = NULL;
	if(g_spawn_async_with_pipes(NULL, (gchar**)e->args->pdata, NULL, G_SPAWN_SEARCH_PATH | G_SPAWN_LEAVE_DESCRIPTORS_OPEN | G_SPAWN_DO_NOT_REAP_CHILD , 
        child_setup, e, &e->pid, NULL, &std_out, &std_err, &err))
	{
		GB_TRACE("exec_spawn_process - spawed process with pid [%d]\n", e->pid);
		GIOChannel *char_out = NULL, *chan_err = NULL;
		guint chan_out_id = 0, chan_err_id = 0;
		
		if(!e->piped)
		{
			char_out = g_io_channel_unix_new(std_out);			
			g_io_channel_set_encoding(char_out, NULL, NULL);
			g_io_channel_set_buffered(char_out, FALSE);
			g_io_channel_set_flags(char_out, G_IO_FLAG_NONBLOCK, NULL );
			chan_out_id = g_io_add_watch(char_out, G_IO_IN | G_IO_HUP | G_IO_ERR | G_IO_PRI | G_IO_NVAL,
				exec_channel_callback, (gpointer)e);
		  
			chan_err = g_io_channel_unix_new(std_err);
			g_io_channel_set_encoding(chan_err, NULL, NULL);			
			g_io_channel_set_buffered(chan_err, FALSE);			
			g_io_channel_set_flags( chan_err, G_IO_FLAG_NONBLOCK, NULL );
			chan_err_id = g_io_add_watch(chan_err, G_IO_IN | G_IO_HUP | G_IO_ERR | G_IO_PRI | G_IO_NVAL ,
				exec_channel_callback, (gpointer)e);

            while(exec_cmd_get_state(e) == RUNNING)
                gtk_main_iteration();                               
                
            g_source_remove(chan_out_id);
            g_source_remove(chan_err_id); 
            g_io_channel_shutdown(char_out, FALSE, NULL);
            g_io_channel_unref(char_out);  
            g_io_channel_shutdown(chan_err, FALSE, NULL);
            g_io_channel_unref(chan_err);                
        }
        else
        {
            while((waitpid(e->pid, &e->exit_code, WNOHANG) != -1) && (exec_cmd_get_state(e) == RUNNING))
                g_usleep(500000);
        }
        
        /* If the process was cancelled then we kill off the child */
        if(exec_cmd_get_state(e) == CANCELLED)
            kill(e->pid, SIGKILL);
        
        /* Reap the child so we don't get a zombie */
        waitpid(e->pid, &e->exit_code, 0);
		g_spawn_close_pid(e->pid);
		close(std_out);
		close(std_err);	
		
        exec_cmd_set_state(e, (e->exit_code == 0) ? COMPLETED : FAILED);
		GB_TRACE("exec_spawn_process - child [%d] exitcode [%d]\n", e->pid, e->exit_code);
	}
	else
	{
		g_warning("exec_spawn_process - failed to spawn process [%d] [%s]\n",
			err->code, err->message);			
        exec_cmd_set_state(e, FAILED);
        if(err != NULL) g_error_free(err);
	}
}


static void
exec_working_dir_setup_func(gpointer data)
{
    GB_LOG_FUNC
    g_return_if_fail(data != NULL);
    ExecCmd *ex = (ExecCmd*)data;
    if(ex->working_dir != NULL)
        g_return_if_fail(chdir(ex->working_dir) == 0);
}


static void
exec_stdout_setup_func(gpointer data)
{
	GB_LOG_FUNC
    g_return_if_fail(data != NULL);
    exec_working_dir_setup_func(data);
	dup2(child_child_pipe[1], 1);
	close(child_child_pipe[0]);
}


static void
exec_stdin_setup_func(gpointer data)
{
	GB_LOG_FUNC	
    g_return_if_fail(data != NULL);
    exec_working_dir_setup_func(data);
	dup2(child_child_pipe[0], 0);
	close(child_child_pipe[1]);
}


static gpointer
exec_run_remainder(gpointer data)
{
	GB_LOG_FUNC
	g_return_val_if_fail(data != NULL, NULL); 	

	GList *cmd = (GList*)data;
    for(; cmd != NULL; cmd = cmd->next)
	{
        ExecCmd *e = (ExecCmd*)cmd->data;
        if(e->lib_proc != NULL)
            e->lib_proc(e, child_child_pipe);
		else 
            exec_spawn_process(e, exec_stdout_setup_func);
        
        ExecState state = exec_cmd_get_state(e);
        if((state == CANCELLED) || (state == FAILED))
            break;
	}	
	close(child_child_pipe[1]);			
	GB_TRACE("exec_run_remainder - thread exiting\n");
	return NULL;
}


static void
exec_cmd_delete(ExecCmd  *e)
{
    GB_LOG_FUNC
    g_return_if_fail(e != NULL);
    g_mutex_free(e->state_mutex);
    g_ptr_array_free(e->args, TRUE);
    g_free(e->working_dir);
}


ExecCmd* 
exec_cmd_new(Exec *exec)
{
    GB_LOG_FUNC 
    g_return_val_if_fail(exec != NULL, NULL);
    
    ExecCmd *e = g_new0(ExecCmd, 1);
    e->args = g_ptr_array_new();
    e->state = RUNNING;
    e->state_mutex = g_mutex_new();
    exec->cmds = g_list_append(exec->cmds, e);
    exec->cmds = g_list_first(exec->cmds);
    return e;
}


void
exec_delete(Exec  *exec)
{
    GB_LOG_FUNC
    g_return_if_fail(exec != NULL);
    g_free(exec->process_title);
    g_free(exec->process_description);
    GList *cmd = exec->cmds;
    for(; cmd != NULL; cmd = cmd->next)
        exec_cmd_delete((ExecCmd*)cmd->data);
    g_list_free(exec->cmds);
    if(exec->err != NULL) g_error_free(exec->err);
    g_free(exec);
}


Exec*
exec_new(const gchar *process_title, const gchar *process_description)
{
	GB_LOG_FUNC
    g_return_val_if_fail(process_title != NULL, NULL);
    g_return_val_if_fail(process_description != NULL, NULL);
    
	Exec *exec = g_new0(Exec, 1);
	g_return_val_if_fail(exec != NULL, NULL);        
    exec->process_title = g_strdup(process_title);
    exec->process_description = g_strdup(process_description);
    exec->outcome = FAILED;
	return exec;
}


void
exec_cmd_add_arg(ExecCmd *e, const gchar *format, ...)
{
    GB_LOG_FUNC
    g_return_if_fail(e != NULL);
    g_return_if_fail(format != NULL);

    va_list va; 
    va_start(va, format);
    gchar *arg = NULL;
    g_vasprintf(&arg, format, va);
    va_end(va);    
    g_ptr_array_add(e->args, arg);
}


void 
exec_cmd_update_arg(ExecCmd *e, const gchar *arg_start, const gchar *format, ...)
{
    GB_LOG_FUNC
    g_return_if_fail(e != NULL);
    g_return_if_fail(arg_start != NULL);
    g_return_if_fail(format != NULL);

    gint i = 0;    
    for(; i < e->args->len; ++i)
    {
        gchar *arg = g_ptr_array_index(e->args, i);
        if(strstr(arg, arg_start) != NULL)
        {
            g_free(arg);
            va_list va; 
            gchar *new_arg = NULL;
            va_start(va, format);
            g_vasprintf(&new_arg, format, va);
            va_end(va);    
            e->args->pdata[i] = new_arg;
            GB_TRACE("exec_cmd_update_arg - set to [%s]\n", (gchar*)g_ptr_array_index(e->args, i));
            break;
        }
    }
}


ExecState 
exec_cmd_get_state(ExecCmd *e) 
{
    /*GB_LOG_FUNC*/
    g_return_val_if_fail(e != NULL, FAILED);
    g_mutex_lock(e->state_mutex);
    ExecState ret = e->state;
    g_mutex_unlock(e->state_mutex);
    return ret;
}


ExecState 
exec_cmd_set_state(ExecCmd *e, ExecState state)
{
    GB_LOG_FUNC
    g_return_val_if_fail(e != NULL, FAILED);
    g_mutex_lock(e->state_mutex);
    if(e->state != CANCELLED)
        e->state = state;
    ExecState ret = e->state;
    g_mutex_unlock(e->state_mutex);
    return ret;
}


void
exec_run(Exec *ex)
{    
    GB_LOG_FUNC
    g_return_if_fail(ex != NULL);   

    ExecState state = RUNNING;
    GList *piped = NULL;    
    GList *cmd = ex->cmds;
    for(; cmd != NULL && ((state != CANCELLED) && (state != FAILED)); cmd = cmd->next)
    {
        ExecCmd *e = (ExecCmd*)cmd->data;
        if(e->piped) 
        {
            piped = g_list_append(piped, e);
            piped = g_list_first(piped);
            continue;
        }
        if(e->pre_proc) e->pre_proc(e, NULL);                 
        
        state = exec_cmd_get_state(e);
        if(state == SKIPPED) continue;
        else if(state == CANCELLED) break;    
        
        GThread *thread = NULL;
        if(e->lib_proc != NULL) 
            e->lib_proc(e, NULL);
        else if(piped != NULL)
        {
            pipe(child_child_pipe); 
            thread = g_thread_create(exec_run_remainder, (gpointer)piped, TRUE, NULL);
            exec_spawn_process(e, exec_stdin_setup_func);
            close(child_child_pipe[0]);
            close(child_child_pipe[1]); 
        }
        else 
            exec_spawn_process(e, exec_working_dir_setup_func);
            
        state = exec_cmd_get_state(e);  
        /* If we have spawned off a bunch of children and something went wrong in the
         * target process, we update the spawned children to failed so they stop. If
         * cancel was clicked we will already have updated all of the children. */
        if((/*(state == CANCELLED) || */(state == FAILED)) && (piped != NULL))
        {
            GList *cmd = ex->cmds;
            for(; cmd != NULL; cmd = cmd->next)
                exec_cmd_set_state((ExecCmd*)cmd->data, FAILED);
        } 
         
        if(e->post_proc) e->post_proc(e, NULL);
        if(thread != NULL) g_thread_join(thread);        
        
        g_list_free(piped);
        piped = NULL;
    }
    g_list_free(piped);
    exec_set_outcome(ex);
}


void
exec_stop(Exec *e)
{
    GB_LOG_FUNC
    g_return_if_fail(e != NULL);
    
    GList *cmd = e->cmds;
    for(; cmd != NULL; cmd = cmd->next)
        exec_cmd_set_state((ExecCmd*)cmd->data, CANCELLED);        
    
    GB_TRACE("exec_stop - complete\n");
}


gint
exec_run_cmd(const gchar *cmd, gchar **output)
{
    GB_LOG_FUNC    
    g_return_val_if_fail(cmd != NULL, -1);
        
    gchar *std_out = NULL;
    gchar *std_err = NULL;
    gint exit_code = -1;
    GError *error = NULL;    
    if(g_spawn_command_line_sync(cmd, &std_out, &std_err, &exit_code, &error))
    {
        *output = g_strconcat(std_out, std_err, NULL);
        g_free(std_out);
        g_free(std_err);     
    }
    else if(error != NULL)
    {       
        g_warning("exec_run_cmd - error [%s] spawning command [%s]", error->message, cmd);        
        g_error_free(error);
    }
    else
    {
        g_warning("exec_run_cmd - Unknown error spawning command [%s]", cmd);     
    }
    GB_TRACE("exec_run_cmd - [%s] returned [%d]\n", cmd, exit_code);
    return exit_code;
}


gint 
exec_count_operations(const Exec *e)
{
    GB_LOG_FUNC
    g_return_val_if_fail(e != NULL, 0);   
    
    gint count = 0;
    GList *cmd = e->cmds;
    for(; cmd != NULL; cmd = cmd->next)
    {
        if(!((ExecCmd*)cmd->data)->piped)
            ++count;
    }
    GB_TRACE("exec_count_operations - there are [%d] operations\n", count);
    return count;
}


