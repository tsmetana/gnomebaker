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
 * File: devices.h
 * Created by: luke <luke@dhcp-45-369>
 * Created on: Mon Feb 24 21:51:18 2003
 */

#ifndef _DEVICES_H_
#define _DEVICES_H_

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>



gboolean devices_probe_busses();
gboolean devices_init();
void devices_populate_optionmenu(GtkWidget* option_menu, const gchar* defaultselect);
gchar* devices_get_device_config(const gchar* devicekey, const gchar* deviceitem);

#endif	/*_DEVICES_H_*/
