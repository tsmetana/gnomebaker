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
 * File: backend.h
 * Copyright: Andreas Kühntopf <andreas@kuehntopf.org>
 */

#ifndef _BACKEND_H_
#define _BACKEND_H_
 
#include "gbcommon.h"

enum backend {
	BACKEND_CDRECORD,
	BACKEND_WODIM
};

 
gboolean backend_is_backend_supported(enum backend b);


#endif /* _BACKEND_H_ */
