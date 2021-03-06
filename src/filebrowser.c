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
 * File: filebrowser.c
 * Copyright: luke_biddell@yahoo.com
 * Created on: Sun Mar 21 20:02:51 2004
 */

#include "filebrowser.h"
#include "gnomebaker.h"
#include "gbcommon.h"
#include "preferences.h"
#include <libgnomevfs/gnome-vfs-mime-utils.h>
#include <libgnomevfs/gnome-vfs-mime-handlers.h>
#include "burn.h"


static const gchar *const widget_browser_dirtree = "treeview4";
static const gchar *const widget_browser_filelist = "treeview5";


/* callback id, so we can block it! */
static gulong sel_changed_id;

static GtkTreePath *dir_tree_selected = NULL;

enum /* Directory Tree */
{
    DT_COL_ICON = 0,
    DT_COL_NAME,
    DT_NUM_COLS
};

enum /* FileList */
{
    FL_COL_ICON = 0,
    FL_COL_NAME,
	FL_COL_SIZE,
	FL_COL_HUMANSIZE,
    FL_COL_TYPE,
	FL_COL_MIME,
    FL_NUM_COLS
};

enum
{
    TARGET_URI_LIST,
    TARGET_STRING,
    TARGET_COUNT
};


static GtkTargetEntry target_entries[] =
{
    {"text/uri-list", 0, TARGET_URI_LIST},
    {"text/plain", 0, TARGET_STRING}
};


static const gchar *ROOT_LABEL = N_("Filesystem");
static const gchar *HOME_LABEL = N_("Home");
static const gchar *EMPTY_LABEL = N_("(empty)");
static const gchar *DIRECTORY = N_("Folder");

static gchar *right_click_selection = NULL;



static void
filebrowser_burn_cd_image(gpointer widget, gpointer user_data)
{
	GB_LOG_FUNC
	burn_cd_image_file(right_click_selection);
}


static void
filebrowser_burn_dvd_image(gpointer widget, gpointer user_data)
{
	GB_LOG_FUNC
	burn_dvd_iso(right_click_selection);
}


static void
filebrowser_on_show_hidden_changed(GConfClient *client,
                                   guint cnxn_id,
                                   GConfEntry *entry,
                                   gpointer user_data)
{
	GB_LOG_FUNC
	filebrowser_refresh();
}


static void
filebrowser_on_show_humansize_changed(GConfClient *client,
                                   guint cnxn_id,
								   GConfEntry *entry,
								   gpointer user_data)
{
	GB_LOG_FUNC
	GtkTreeView *file_list =  GTK_TREE_VIEW(glade_xml_get_widget(gnomebaker_getxml(), widget_browser_filelist));
	g_return_if_fail(file_list != NULL);

	GtkTreeViewColumn *size_column =
		gtk_tree_view_get_column(file_list, FL_COL_SIZE-1);
	g_return_if_fail(size_column != NULL);

	GtkTreeViewColumn *humansize_column = gtk_tree_view_get_column(file_list, FL_COL_HUMANSIZE-1);
	g_return_if_fail(humansize_column != NULL);

	const gboolean show_human_size = preferences_get_bool(GB_SHOWHUMANSIZE);
	gtk_tree_view_column_set_visible(size_column, !show_human_size);
	gtk_tree_view_column_set_visible(humansize_column, show_human_size);
}


static GString *
filebrowser_build_path(GtkTreeModel *model, GtkTreeIter *iter)
{
	GB_LOG_FUNC
	g_return_val_if_fail(model != NULL, NULL);
	g_return_val_if_fail(iter != NULL, NULL);

	gchar *val = NULL;
	gtk_tree_model_get(model, iter, DT_COL_NAME, &val, -1);
	g_return_val_if_fail(val != NULL, NULL);

	GString *full_path = g_string_new("");

	/* Have a look at the selection value, if it's the root label then we
	   append / rather than "Filesystem" as that would make an invalid path */
	if(g_ascii_strcasecmp(val, ROOT_LABEL) == 0)
		g_string_append(full_path, "/");
	else if(g_ascii_strcasecmp(val, HOME_LABEL) == 0)
		g_string_append(full_path, g_get_home_dir());
	else
		g_string_append(full_path, val);

	g_free(val);

	GB_DECLARE_STRUCT(GtkTreeIter, parent);
	GtkTreeIter current = *iter;

	/* Now work our way up the ancestors building the path */
	while(gtk_tree_model_iter_parent(model, &parent, &current))
	{
		gchar *sub_dir = NULL;
		gtk_tree_model_get(model, &parent, DT_COL_NAME, &sub_dir, -1);
		if(sub_dir != NULL)
		{
			g_string_insert(full_path, 0, "/");

			/* If this isn't the "Filesystem" label then append the name */
			if(g_ascii_strcasecmp(sub_dir, HOME_LABEL) == 0)
				g_string_insert(full_path, 0, g_get_home_dir());
			else if(g_ascii_strcasecmp(sub_dir, ROOT_LABEL) != 0)
				g_string_insert(full_path, 0, sub_dir);
		}
		else
		{
			g_warning("filebrowser_build_path - sub_dir from GValue is NULL");
		}

		g_free(sub_dir);
		current = parent;
	}

	return full_path;
}


static gint
filebrowser_list_sortfunc(GtkTreeModel *model, GtkTreeIter *a,
                           GtkTreeIter *b, gpointer user_data)
{
	gchar *aname = NULL, *amime = NULL, *bname = NULL, *bmime = NULL;
	gtk_tree_model_get (model, a, FL_COL_NAME, &aname, FL_COL_TYPE, &amime, -1);
	gtk_tree_model_get (model, b, FL_COL_NAME, &bname, FL_COL_TYPE, &bmime, -1);

	gboolean aisdir = g_ascii_strcasecmp(amime, DIRECTORY) == 0;
	gboolean bisdir = g_ascii_strcasecmp(bmime, DIRECTORY) == 0;

	gint result = 0;
	if(aisdir && !bisdir)
		result = -1;
	else if(!aisdir && bisdir)
		result = 1;
	else
		result = g_ascii_strcasecmp(aname, bname);

	g_free(aname);
	g_free(amime);
	g_free(bname);
	g_free(bmime);

	return result;
}


static void
filebrowser_populate(GtkTreeModel *tree_model,
					GtkTreeIter *iter,
					GtkTreeView *tree_view,
					GtkTreeView *file_view)
{
	GB_LOG_FUNC
	g_return_if_fail(tree_model != NULL);
	g_return_if_fail(iter != NULL);
	g_return_if_fail(tree_view != NULL);
	/*g_return_if_fail(file_view != NULL);*/

	gnomebaker_show_busy_cursor(TRUE);

	/* Now get the full path for the selection */
	GString *full_path = filebrowser_build_path(tree_model, iter);

	/* Get the directory tree and unref it's model so it sorts faster
	GtkTreeView* dir_tree = gtk_tree_selection_get_tree_view(selection);
	g_object_ref(tree_model);
	gtk_tree_view_set_model(dir_tree, NULL);*/

	/* Get the right hand file list, disconnect it from the view so it
	   sorts quicker and clear out any existing files */
	GtkTreeModel *file_model = NULL;
	if(file_view != NULL)
	{
		file_model = gtk_tree_view_get_model(file_view);
		g_object_ref(file_model);
		gtk_tree_view_set_model(file_view, NULL);
		gtk_list_store_clear(GTK_LIST_STORE(file_model));
	}

	/* We may have already selected this directory in the tree before so
	   if the iter already has children then we wont add it to the dir tree */
	gboolean add_to_tree = TRUE;
	GB_DECLARE_STRUCT(GtkTreeIter, empty);
	if(gtk_tree_model_iter_nth_child(tree_model, &empty, iter, 0))
	{
		GValue value = { 0 };
		gtk_tree_model_get_value(tree_model, &empty, DT_COL_NAME, &value);

		if(g_ascii_strcasecmp(g_value_get_string(&value), EMPTY_LABEL) != 0)
			add_to_tree = FALSE;
		g_value_unset(&value);
	}

	const gboolean show_hidden = preferences_get_bool(GB_SHOWHIDDEN);

	/* Now we open the directory specified by the full path */
	GError *err = NULL;
	GDir *dir = g_dir_open(full_path->str, 0, &err);
	if(dir != NULL)
	{
		/* loop around reading the files in the directory */
		const gchar *name = g_dir_read_name(dir);
		while(name != NULL)
		{
			/* First, check if we want to add this entry! (Hidden file/dir) */
			if (show_hidden == FALSE && name[0] == '.')
			{
				name = g_dir_read_name(dir);
				continue;
			}

			/* build up the full path to the name */
			gchar *full_name = g_build_filename(full_path->str, name, NULL);
			GB_DECLARE_STRUCT(struct stat, s);
			if(lstat(full_name, &s) == 0)
			{
				/* see if the name is actually a directory */
				if(S_ISDIR(s.st_mode))
				{
					/* It's a directory so if it doesn't already have children
					   then we add it */
					if(add_to_tree)
					{
						GB_DECLARE_STRUCT(GtkTreeIter, sibling);
						gtk_tree_store_insert_after(GTK_TREE_STORE(tree_model), &sibling, iter, NULL);

						GdkPixbuf *icon = gbcommon_get_icon_for_name("gnome-fs-directory", 16);
						gtk_tree_store_set(GTK_TREE_STORE(tree_model), &sibling,
                                DT_COL_ICON, icon, DT_COL_NAME, name, -1);
						g_object_unref(icon);

						GB_DECLARE_STRUCT(GtkTreeIter, sibling_child);
						gtk_tree_store_insert_after(GTK_TREE_STORE(tree_model), &sibling_child, &sibling, NULL);
						gtk_tree_store_set(GTK_TREE_STORE(tree_model), &sibling_child,
                                DT_COL_ICON, NULL, DT_COL_NAME, EMPTY_LABEL, -1);
					}
					/* need this when I improve the list to include dirs */
					if(file_model != NULL)
					{
						GB_DECLARE_STRUCT(GtkTreeIter, iter_right);
						gtk_list_store_append(GTK_LIST_STORE(file_model), &iter_right);
						GdkPixbuf *icon = gbcommon_get_icon_for_name("gnome-fs-directory", 16);
						gtk_list_store_set(GTK_LIST_STORE(file_model), &iter_right,
                                FL_COL_ICON, icon, FL_COL_NAME, name,
                                FL_COL_SIZE, (guint64)4096, FL_COL_HUMANSIZE, "4 KB",
                                FL_COL_TYPE, DIRECTORY, FL_COL_MIME, "x-directory/normal", -1);
						g_object_unref(icon);
					}
				}
				/* It's a file */
				else if(S_ISREG(s.st_mode) && (file_model != NULL))
				{
					/* We stored the right hand file list as user data when
					   when we set up the directory tree selection changed func */
					GB_DECLARE_STRUCT(GtkTreeIter, iter_right);
					gtk_list_store_append(GTK_LIST_STORE(file_model), &iter_right);
  					gchar *mime = gbcommon_get_mime_type(full_name);
					GdkPixbuf *icon = gbcommon_get_icon_for_mime(mime, 16);
  					gchar *human_size = gbcommon_humanreadable_filesize(s.st_size);
					gchar *type = gbcommon_get_mime_description(mime);
  					gtk_list_store_set(GTK_LIST_STORE(file_model), &iter_right,
  						    FL_COL_ICON, icon, FL_COL_NAME, name,
 						    FL_COL_SIZE, (guint64)s.st_size, FL_COL_HUMANSIZE, human_size,
						    FL_COL_TYPE, type, FL_COL_MIME, mime, -1);
					g_object_unref(icon);
					g_free(mime);
  					g_free(type);
  					g_free(human_size);
				}
			}
			else
			{
				g_warning("filebrowser_populate - Stat of file [%s] failed", full_name);
			}

			g_free(full_name);
			name = g_dir_read_name(dir);
		}

		g_dir_close(dir);
	}

	/* Connect the left hand dir tree view back to its model
	gtk_tree_view_set_model(dir_tree, tree_model);
	g_object_unref(tree_model);*/

	/* Connect the right hand file list view back to its model */
	if(file_view != NULL)
	{
		gtk_tree_view_set_model(file_view, file_model);
		g_object_unref(file_model);
	}

	/* Finally we remove the empty child. We don't do it until
		we have add other children otherwise the tree attempts
	    to collapse */
	if(add_to_tree && (empty.stamp != 0))
		gtk_tree_store_remove(GTK_TREE_STORE(tree_model), &empty);

	g_string_free(full_path, TRUE);

	gnomebaker_show_busy_cursor(FALSE);
}


static void
filebrowser_sel_changed(GtkTreeSelection *selection, gpointer user_data)
{
	GB_LOG_FUNC
	g_return_if_fail(selection != NULL);
	g_return_if_fail(user_data != NULL);

    GtkTreeModel *tree_model = NULL;
    GB_DECLARE_STRUCT(GtkTreeIter, iter);
    /* Block the signal so that it doesn't trigger recursively (saving CPU cycles here :-))*/
    g_signal_handler_block (selection, sel_changed_id);

    /* The selection in the dir tree has changed so get that selection */
    if(gtk_tree_selection_get_selected(selection, &tree_model, &iter))
    {
		GtkTreePath *selected_path = gtk_tree_model_get_path(tree_model, &iter);
		if((dir_tree_selected == NULL) || (gtk_tree_path_compare(selected_path, dir_tree_selected) != 0))
		{
			GtkTreeView *dir_tree = gtk_tree_selection_get_tree_view(selection);
			filebrowser_populate(tree_model, &iter, dir_tree, GTK_TREE_VIEW(user_data));
			gtk_tree_path_free(dir_tree_selected);
			dir_tree_selected = selected_path;
    	}
		else
		{
			gtk_tree_path_free(selected_path);
		}
	}
    g_signal_handler_unblock (selection, sel_changed_id);
}


static void
filebrowser_on_tree_expanding(GtkTreeView *tree_view, GtkTreeIter *iter,
				            GtkTreePath *path, gpointer user_data)
{
	GB_LOG_FUNC
	g_return_if_fail(tree_view != NULL);
	g_return_if_fail(iter != NULL);

	GtkTreeModel *tree_model = gtk_tree_view_get_model(tree_view);
	g_return_if_fail(tree_model != NULL);

	filebrowser_populate(tree_model, iter, tree_view, NULL);
}


static gchar*
filebrowser_build_filename(GtkTreeModel *file_model, GtkTreeIter *iter)
{
	GB_LOG_FUNC
	g_return_val_if_fail(file_model != NULL, NULL);
	g_return_val_if_fail(iter != NULL, NULL);

	gchar *name = NULL;
	gtk_tree_model_get(file_model, iter, FL_COL_NAME, &name, -1);

	/* now we have the file_name we can get the directory tree and fully
		expand the path */
	GtkTreeView *view = (GtkTreeView*)g_object_get_data(G_OBJECT(file_model), "dirtreeview");

	/* Now expand the path in the dir tree */
	GtkTreeModel *tree_model = NULL;
	GB_DECLARE_STRUCT(GtkTreeIter, dir_iter);
	gtk_tree_selection_get_selected(gtk_tree_view_get_selection(view), &tree_model, &dir_iter);
	GString *full_path = filebrowser_build_path(tree_model, &dir_iter);
	/* build up the full file_name from the path and name */
	gchar *file_name = g_build_filename(full_path->str, name, NULL);
	g_free(name);
	g_string_free(full_path, TRUE);
	return file_name;
}


static void
filebrowser_foreach_fileselection(GtkTreeModel *file_model, GtkTreePath *path,
			                     GtkTreeIter *iter, gpointer user_data)
{
	GB_LOG_FUNC
	g_return_if_fail(user_data != NULL);

	GString *drag_path = (GString*)user_data;
	gchar *file_name = filebrowser_build_filename(file_model, iter);
	gchar *uri = gbcommon_get_uri(file_name);
	g_string_append(drag_path, uri);
	g_string_append(drag_path, "\n");
	g_free(file_name);
	g_free(uri);
}


static void
filebrowser_on_drag_data_get(GtkWidget *widget, GdkDragContext *context,
			                 GtkSelectionData *selection_data,
			                 guint info, guint time, gpointer data)
{
	GB_LOG_FUNC
	g_return_if_fail(widget != NULL);
	g_return_if_fail(selection_data != NULL);

	GtkTreeView *view = GTK_TREE_VIEW (widget);
	GString *file = NULL;

	/* This function handles the drag get for the list and the tree, so first
	   we see if the tree_view's model is a list store or a tree store */
	if(GTK_IS_LIST_STORE(gtk_tree_view_get_model(view)))
	{
		file = g_string_new("");
		GtkTreeSelection *sel = gtk_tree_view_get_selection(view);
		gtk_tree_selection_selected_foreach(sel, filebrowser_foreach_fileselection, file);
	}
	else
	{
		/* Expand the path in the dir tree */
		GtkTreeModel *tree_model = NULL;
		GB_DECLARE_STRUCT(GtkTreeIter, dir_iter);
		gtk_tree_selection_get_selected(gtk_tree_view_get_selection(view), &tree_model, &dir_iter);
		file = filebrowser_build_path(tree_model, &dir_iter);
		gchar *uri = gbcommon_get_uri(file->str);
		g_string_assign(file, uri);
	}

	GB_TRACE("filebrowser_on_drag_data_get - selection data is [%s]\n", file->str);

	/* Set the fully built path(s) as the selection data */
	gtk_selection_data_set(selection_data, selection_data->target, 8,
            (const guchar*)file->str, strlen(file->str)  *sizeof(gchar));

	g_string_free(file, TRUE);
}


static void
filebrowser_select_directory(const gchar *directory)
{
	GtkTreeView *dir_tree = GTK_TREE_VIEW(glade_xml_get_widget(gnomebaker_getxml(), widget_browser_dirtree));
	GtkTreeModel *tree_model = NULL;
	GB_DECLARE_STRUCT(GtkTreeIter, dir_iter);
	GtkTreeSelection *sel = gtk_tree_view_get_selection(dir_tree);
	gtk_tree_selection_get_selected(sel, &tree_model, &dir_iter);

	GB_DECLARE_STRUCT(GtkTreeIter, child_iter);
	if(gtk_tree_model_iter_children(tree_model, &child_iter, &dir_iter))
	{
		do
		{
			gchar *val = NULL;
			gtk_tree_model_get(tree_model, &child_iter, DT_COL_NAME, &val, -1);
			const gchar *name = g_basename(directory);
			if(g_ascii_strcasecmp(val, name) == 0)
			{
				/* Force the tree to expand */
				GtkTreePath *currentpath = gtk_tree_model_get_path(tree_model, &dir_iter);
				gtk_tree_view_expand_row(dir_tree, currentpath, FALSE);
				gtk_tree_path_free(currentpath);

				/* Now select the node we have found */
				gtk_tree_selection_select_iter(sel, &child_iter);
				g_free(val);
				break;
			}

			g_free(val);
		}
		while(gtk_tree_model_iter_next(tree_model, &child_iter));
	}
}


static void
filebrowser_on_open(gpointer widget, gpointer user_data)
{
	GB_LOG_FUNC
	if(g_file_test(right_click_selection, G_FILE_TEST_IS_DIR))
		filebrowser_select_directory(right_click_selection);
	else
		gbcommon_launch_app_for_file(right_click_selection);
}


static gboolean
filebrowser_on_button_pressed(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	GB_LOG_FUNC
	g_return_val_if_fail(widget != NULL, FALSE);

	/* look for a right click */
	if(event->button == 3)
	{
		GtkWidget *menu = gtk_menu_new();
		GtkTreeView *view = GTK_TREE_VIEW(widget);

		if(GTK_IS_TREE_STORE(gtk_tree_view_get_model(view)))
		{
			gbcommon_append_menu_item_stock(menu, _("_Add directory"), GTK_STOCK_ADD,
				    (GCallback)gnomebaker_on_add_dir, widget);
		}
		else
		{
			GtkTreeSelection *selection = gtk_tree_view_get_selection(view);
			const gint count = gtk_tree_selection_count_selected_rows(selection);
			if(count == 1)
			{
				gbcommon_append_menu_item_stock(menu, _("_Open"), GTK_STOCK_OPEN,
					   (GCallback)filebrowser_on_open, view);
			}

            gbcommon_append_menu_item_stock(menu, _("_Add file(s)"), GTK_STOCK_ADD,
				    (GCallback)gnomebaker_on_add_files, view);
            if(count == 1)
            {
                gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());
				GtkTreeModel *model = gtk_tree_view_get_model(view);
				GB_DECLARE_STRUCT(GtkTreeIter, iter);
				gtk_tree_selection_selected_foreach(selection,
					   (GtkTreeSelectionForeachFunc)gbcommon_get_first_selected_row, &iter);
				gchar *name = NULL, *mime = NULL;
				gtk_tree_model_get(model, &iter, FL_COL_NAME, &name, FL_COL_MIME, &mime, -1);
				g_free(right_click_selection);
                right_click_selection = filebrowser_build_filename(model, &iter);

				if(gbcommon_str_has_suffix(name, ".cue") || gbcommon_str_has_suffix(name, ".toc") || (g_ascii_strcasecmp(mime, "application/x-cd-image") == 0))
                    gbcommon_append_menu_item_file(menu, _("_Burn CD Image"),
                            "baker-cd-iso.png", (GCallback)filebrowser_burn_cd_image, view);
                if(g_ascii_strcasecmp(mime, "application/x-cd-image") == 0)
                    gbcommon_append_menu_item_file(menu, _("_Burn DVD Image"),
                            "baker-dvd-iso.png", (GCallback)filebrowser_burn_dvd_image, view);
				g_free(name);
				g_free(mime);
			}
		}

		gtk_widget_show_all(menu);

		/* Note: event can be NULL here when called. However,
		 *  gdk_event_get_time() accepts a NULL argument */
		gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
                (event != NULL) ? event->button : 0, gdk_event_get_time((GdkEvent*)event));
		return TRUE;
	}
	return FALSE;
}


static void
filebrowser_on_list_dbl_click(GtkTreeView *tree_view, GtkTreePath *path,
                       	      GtkTreeViewColumn *col, gpointer user_data)
{
	GB_LOG_FUNC
	g_return_if_fail(tree_view != NULL);
	g_return_if_fail(path != NULL);

	GtkTreeModel *model = gtk_tree_view_get_model(tree_view);
	g_return_if_fail(model != NULL);

	GB_DECLARE_STRUCT(GtkTreeIter, iter);
	gtk_tree_model_get_iter(model, &iter, path);

	GString *selection = g_string_new("");
	filebrowser_foreach_fileselection(model, path, &iter, selection);
	gchar *local_path = gbcommon_get_local_path(selection->str);
	if(g_file_test(local_path, G_FILE_TEST_IS_DIR))
		filebrowser_select_directory(local_path);
	else
		gbcommon_launch_app_for_file(local_path);
	g_string_free(selection, TRUE);
	g_free(local_path);
}


static void
filebrowser_on_tree_dbl_click(GtkTreeView *tree_view, GtkTreePath *path,
                       	      GtkTreeViewColumn *col, gpointer user_data)
{
	GB_LOG_FUNC
	g_return_if_fail(tree_view != NULL);
	g_return_if_fail(path != NULL);

	if(gtk_tree_view_row_expanded(tree_view, path))
		gtk_tree_view_collapse_row(tree_view, path);
	else
		gtk_tree_view_expand_row(tree_view, path, FALSE);
}


static void
filebrowser_setup_tree(GtkTreeView *dir_tree, GtkTreeView *file_list)
{
	GB_LOG_FUNC
	g_return_if_fail(dir_tree != NULL);
	g_return_if_fail(file_list != NULL);

	/* Create the tree store for the dir tree */
    GtkTreeStore *store = gtk_tree_store_new(DT_NUM_COLS, GDK_TYPE_PIXBUF, G_TYPE_STRING);
    gtk_tree_view_set_model(dir_tree, GTK_TREE_MODEL(store));
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(store), DT_COL_NAME, GTK_SORT_ASCENDING);
    g_object_unref(store);

	/* One column which has an icon renderer and a text renderer packed in */
    GtkTreeViewColumn *col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(col, _("Filesystem"));
	/*gtk_tree_view_column_set_sort_column_id(col, DT_COL_NAME);
	gtk_tree_view_column_set_sort_order(col, GTK_SORT_ASCENDING);*/
    GtkCellRenderer *renderer = gtk_cell_renderer_pixbuf_new();
    gtk_tree_view_column_pack_start(col, renderer, FALSE);
    gtk_tree_view_column_set_attributes(col, renderer, "pixbuf", DT_COL_ICON, NULL);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_set_attributes(col, renderer, "text", DT_COL_NAME, NULL);
    gtk_tree_view_append_column(dir_tree, col);

	/* Enable the file list as a drag source */
    gtk_drag_source_set(GTK_WIDGET(dir_tree), GDK_BUTTON1_MASK, target_entries,
            TARGET_COUNT, GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK);

	g_signal_connect(dir_tree, "drag_data_get", G_CALLBACK(filebrowser_on_drag_data_get), store);

	/* Add in a root file system label as the base of our tree */
	GdkPixbuf *icon = gbcommon_get_icon_for_name("gnome-dev-harddisk", 16);
	GB_DECLARE_STRUCT(GtkTreeIter, root_iter);
    gtk_tree_store_append(store, &root_iter, NULL);
    gtk_tree_store_set(store, &root_iter, DT_COL_ICON, icon, DT_COL_NAME, ROOT_LABEL, -1);
	g_object_unref(icon);

	/* Add in a home label as the base of our tree */
	GB_DECLARE_STRUCT(GtkTreeIter, home_iter);
	icon = gbcommon_get_icon_for_name("gnome-fs-home", 16);
    gtk_tree_store_append(store, &home_iter, NULL);
	const gchar *user_name = g_get_user_name();
	HOME_LABEL = g_strdup_printf(_("%s's home"), user_name);
    gtk_tree_store_set(store, &home_iter, DT_COL_ICON, icon, DT_COL_NAME, HOME_LABEL, -1);
	g_object_unref(icon);

	/* now give the right hand file list a reference to the left hand dir tree.
       We do this so when we drag a file from the right hand list we can fully
	   expand the path */
	g_object_set_data(G_OBJECT(gtk_tree_view_get_model(file_list)), "dirtreeview", dir_tree);

	/* Set the selection mode of the dir tree */
	GtkTreeSelection *selection = gtk_tree_view_get_selection(dir_tree);
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_BROWSE);

	/* Connect up the changed signal so we can populate the file list according
	   to the selection in the dir tree */
	sel_changed_id = g_signal_connect((gpointer) selection, "changed",
            G_CALLBACK(filebrowser_sel_changed), file_list);

	g_signal_connect((gpointer) dir_tree, "row-expanded",
            G_CALLBACK(filebrowser_on_tree_expanding), file_list);

	/* connect the signal to handle right click */
	g_signal_connect (G_OBJECT(dir_tree), "button-press-event",
            G_CALLBACK(filebrowser_on_button_pressed), NULL);

	/* handle double clicks */
	g_signal_connect(G_OBJECT(dir_tree), "row-activated",
            G_CALLBACK(filebrowser_on_tree_dbl_click), NULL);

	/* Force the populate of the filesystem node so that it shows an expander. We
	   must do this before we force selection of the home dir so that the list
	   correctly shows the contents of the home dir. */
	filebrowser_populate(GTK_TREE_MODEL(store), &root_iter, dir_tree, file_list);

	/* force the selection of the home node so we initially populate the home tree and file list */
	gtk_tree_selection_select_iter(selection, &home_iter);
	GtkTreePath *path = gtk_tree_model_get_path(GTK_TREE_MODEL(store), &home_iter);
	gtk_tree_view_expand_row(dir_tree, path, FALSE);
	gtk_tree_path_free(path);

	preferences_register_notify(GB_SHOWHIDDEN, filebrowser_on_show_hidden_changed, NULL);
	preferences_register_notify(GB_SHOWHUMANSIZE, filebrowser_on_show_humansize_changed, NULL);
}


static void
filebrowser_setup_list(GtkTreeView *file_list)
{
	GB_LOG_FUNC
	g_return_if_fail(file_list != NULL);

	/* Create the list store for the file list */
    GtkListStore *store = gtk_list_store_new(
            FL_NUM_COLS, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_UINT64,
            G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    gtk_tree_view_set_model(file_list, GTK_TREE_MODEL(store));

	gtk_tree_sortable_set_default_sort_func(GTK_TREE_SORTABLE(store), filebrowser_list_sortfunc, NULL, NULL);
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(store),
            GTK_TREE_SORTABLE_DEFAULT_SORT_COLUMN_ID, GTK_SORT_ASCENDING);

    g_object_unref(store);

	/* 1st column which has an icon renderer and a text renderer packed in */
    GtkTreeViewColumn *col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_resizable(col, TRUE);
    gtk_tree_view_column_set_title(col, _("File"));
    GtkCellRenderer *renderer = gtk_cell_renderer_pixbuf_new();
    gtk_tree_view_column_pack_start(col, renderer, FALSE);
    gtk_tree_view_column_set_attributes(col, renderer, "pixbuf", FL_COL_ICON, NULL);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_set_attributes(col, renderer, "text", FL_COL_NAME, NULL);
    gtk_tree_view_append_column(file_list, col);

	const gboolean show_human_size = preferences_get_bool(GB_SHOWHUMANSIZE);

	/* 2nd column to add the size to */
	col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_resizable(col, TRUE);
	gtk_tree_view_column_set_title(col, _("Size"));
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_set_attributes(col, renderer, "text", FL_COL_SIZE, NULL);
	gtk_tree_view_column_set_visible(col, !show_human_size);
    gtk_tree_view_append_column(file_list, col);

	/* 3rd column to add the human readeable size to */
	col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_resizable(col, TRUE);
	gtk_tree_view_column_set_title(col, _("Size"));
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_set_attributes(col, renderer, "text", FL_COL_HUMANSIZE,NULL);
	gtk_tree_view_column_set_visible(col, show_human_size);
	gtk_tree_view_append_column(file_list, col);

	/* 4th column to add the mime type description to */
	col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_resizable(col, TRUE);
	gtk_tree_view_column_set_title(col, _("Type"));
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_set_attributes(col, renderer, "text", FL_COL_TYPE, NULL);
    gtk_tree_view_append_column(file_list, col);

	/* 5th hidden column to add the mime type to */
	col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_resizable(col, TRUE);
	gtk_tree_view_column_set_title(col, _("Mime"));
	gtk_tree_view_column_set_visible(col, FALSE);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_set_attributes(col, renderer, "text", FL_COL_MIME, NULL);
    gtk_tree_view_append_column(file_list, col);

	/* Set the selection mode of the file list */
    gtk_tree_selection_set_mode(gtk_tree_view_get_selection(file_list), GTK_SELECTION_MULTIPLE);

	/* Enable the file list as a drag source */
    gtk_drag_source_set(GTK_WIDGET(file_list), GDK_BUTTON1_MASK, target_entries,
            TARGET_COUNT, GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK);

	/* Connect the function to handle the drag data */
    g_signal_connect(file_list, "drag_data_get", G_CALLBACK(filebrowser_on_drag_data_get), store);

	/* connect the signal to handle right click */
	g_signal_connect (G_OBJECT(file_list), "button-press-event", G_CALLBACK(filebrowser_on_button_pressed), NULL);
	g_signal_connect(G_OBJECT(file_list), "row-activated",  G_CALLBACK(filebrowser_on_list_dbl_click), NULL);
}


void
filebrowser_refresh()
{
    GB_LOG_FUNC
    GladeXML *xml = gnomebaker_getxml();
    g_return_if_fail(xml != NULL);
    GtkWidget *dir_tree = glade_xml_get_widget(xml, widget_browser_dirtree);
    g_return_if_fail(dir_tree != NULL);
    GtkWidget *file_list =  glade_xml_get_widget(xml, widget_browser_filelist);
    g_return_if_fail(file_list != NULL);
    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(dir_tree));
    g_return_if_fail(sel != NULL);

    /* TODO - this is a hack to force the filebrowser to think the selection has
     * changed so it will reload the directory, think of something better */
    gtk_tree_path_free(dir_tree_selected);
    dir_tree_selected = NULL;
    g_signal_emit_by_name (sel, "changed", sel, file_list);
}


GtkSelectionData*
filebrowser_get_selection(gboolean from_tree)
{
    GB_LOG_FUNC

    GtkWidget *tree = NULL;
    if(from_tree)
        tree = glade_xml_get_widget(gnomebaker_getxml(), widget_browser_dirtree);
    else
        tree = glade_xml_get_widget(gnomebaker_getxml(), widget_browser_filelist);

    GtkSelectionData *selection_data = g_new0(GtkSelectionData, 1);
    filebrowser_on_drag_data_get(tree, NULL, selection_data, 0, 0, NULL);
    return selection_data;
}


void
filebrowser_new()
{
	GB_LOG_FUNC

    ROOT_LABEL = _(ROOT_LABEL);
    HOME_LABEL = _(HOME_LABEL);
    EMPTY_LABEL = _(EMPTY_LABEL);
    DIRECTORY = _(DIRECTORY);

	GtkTreeView *dir_tree =
		GTK_TREE_VIEW(glade_xml_get_widget(gnomebaker_getxml(), widget_browser_dirtree));
	GtkTreeView *file_list =
		GTK_TREE_VIEW(glade_xml_get_widget(gnomebaker_getxml(), widget_browser_filelist));

	g_return_if_fail(dir_tree != NULL);
	g_return_if_fail(file_list != NULL);

	filebrowser_setup_list(file_list);
    filebrowser_setup_tree(dir_tree, file_list);
    gtk_widget_show_all(GTK_WIDGET(dir_tree));
}
