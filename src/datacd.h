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
 * File: datacd.h
 * Created by: Luke Biddell <Email>
 * Created on: Sun May  9 15:16:08 2004
 */

#ifndef _DATACD_H_
#define _DATACD_H_

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
	DATACD_COL_ICON = 0,
    DATACD_COL_FILE,
	DATACD_COL_SIZE,
	DATACD_COL_PATH,	
    DATACD_NUM_COLS
};


void datacd_setup_list(GtkTreeView * filelist);
void datacd_on_remove_clicked(GtkWidget *menuitem, gpointer userdata);
void datacd_on_clear_clicked(GtkWidget *menuitem, gpointer userdata);
void datacd_on_drag_data_received(GtkWidget * widget, GdkDragContext * context,
    						  gint x, gint y, GtkSelectionData * seldata,
    						  guint info, guint time, gpointer userdata);
#endif	/*_DATACD_H_*/
