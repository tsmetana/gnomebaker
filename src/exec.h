/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
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

#ifndef _EXEC_H_
#define _EXEC_H_

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif


#include <gnome.h>


typedef void (*ExecFunc) (void *, void*);

typedef enum
{
	RUNNING = 0,
	SKIPPED,
	CANCELLED,
	FAILED,
	COMPLETED
} ExecState;

typedef struct
{
	GPtrArray *args;
	pid_t pid;
	gint exit_code;    
	ExecState state;
    GMutex *state_mutex;
    ExecFunc lib_proc;
	ExecFunc pre_proc;
	ExecFunc read_proc;
	ExecFunc post_proc;	
    gchar *working_dir;
    gboolean piped;
} ExecCmd;

typedef struct
{
    gchar *process_title;
    gchar *process_description;
	GList *cmds;
	GError *err;
    ExecState outcome;
} Exec;


Exec *exec_new(const gchar *process_title, const gchar *process_description);
ExecCmd *exec_cmd_new(Exec *e);
void exec_delete(Exec *self);
void exec_run(Exec *e);
void exec_stop(Exec *e);
void exec_cmd_add_arg(ExecCmd *e, const gchar *format, ...);
void exec_cmd_update_arg(ExecCmd *e, const gchar *arg_start, const gchar *format, ...);
gint exec_run_cmd(const gchar *cmd, gchar **output);
ExecState exec_cmd_get_state(ExecCmd *e);
ExecState exec_cmd_set_state(ExecCmd *e, ExecState state);
gint exec_count_operations(const Exec *e);

#endif	/*_EXEC_H_*/
