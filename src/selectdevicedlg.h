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
 * File: selectdevicedlg.h
 * Copyright: luke biddell
 * Created on: Sun Jan 16 00:42:55 2005
 */

#ifndef _SELECTDEVICEDLG_H_
#define _SELECTDEVICEDLG_H_

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif


#include <gnome.h>
#include <glade/glade.h>


GtkWidget *selectdevicedlg_new(void);
void selectdevicedlg_delete(GtkWidget *self);


#endif	/*_SELECTDEVICEDLG_H_*/
