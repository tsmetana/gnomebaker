/***************************************************************************
 *            datadvd.h
 *
 *  Sat Nov 27 15:29:28 2004
 *  Copyright  2004  User
 *  Email
 ****************************************************************************/

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
 
#ifndef _DATADVD_H_
#define _DATADVD_H_

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <glade/glade.h>
#include <gnome.h>


enum
{    
	DATADVD_COL_ICON = 0,
    DATADVD_COL_FILE,
	DATADVD_COL_SIZE,
	DATADVD_COL_PATH,	
    DATADVD_NUM_COLS
};


void datadvd_setup_list(GtkTreeView * filelist);
void datadvd_on_remove_clicked(GtkWidget *menuitem, gpointer userdata);
void datadvd_on_clear_clicked(GtkWidget *menuitem, gpointer userdata);
void datadvd_on_drag_data_received(GtkWidget * widget, GdkDragContext * context,
    						  gint x, gint y, GtkSelectionData * seldata,
    						  guint info, guint time, gpointer userdata);
#endif	/*_DATADVD_H_*/
