/*
 * Copyright (C) 2012  VN Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * Description: IRCUT Driver for Mozart EVM and RDK
 *
 * $History: ircut.h $
 *
 * *****************  Version 1.0.0.3  *****************
 * User: SC. FanChiang       Date: 13/01/14   Time: 14:30p
 * 1. Recompile for kernel symbol change, support mozart_kernel-1.30
 *
 * *****************  Version 1.0.0.2  *****************
 * User: SC. FanChiang       Date: 12/10/03   Time: 18:15p
 * 1. CHANGE IRCUT GPIO to 10,11
 * 2. Rename GPIO_IRCUT, UART_IRCUT
 * 
 * *****************  Version 1.0.0.1  *****************
 * User: SC. FanChiang       Date: 12/08/07   Time: 11:38a
 * First Release.
 * 
 *
 */
#ifndef __IRCUT_H__
#define __IRCUT_H__

#ifndef __KERNEL__
#define __KERNEL__
#endif

#ifndef MODULE
#define MODULE
#endif

#include "global_codec.h"
#include "errordef.h"
#include "typedef.h"

#define IRCUT_CLOSE		0
#define IRCUT_OPEN		1
#define IRCUT_STANDBY	2
#define IRCUT_VERSION MAKEFOURCC(1, 0, 0, 3)
#define IRCUT_ID_VERSION "1.0.0.3"

#endif //IRCUT_H__
