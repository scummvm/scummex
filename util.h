/* ScummEX - Viewer for Scumm data files
 * Copyright (C) 2003 Adrien Mercier
 * Copyright (C) 2003 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/util.h,v 1.1 2003/09/28 21:56:34 yoshizf Exp $
 *
 */

#ifndef UTIL_H
#define UTIL_H

#include "scummsys.h"
#include "stdafx.h"

template<typename T> inline T ABS (T x)		{ return (x>=0) ? x : -x; }
template<typename T> inline T MIN (T a, T b)	{ return (a<b) ? a : b; }
template<typename T> inline T MAX (T a, T b)	{ return (a>b) ? a : b; }

#define ARRAYSIZE(x) ((int)(sizeof(x) / sizeof(x[0])))

void warning(const char *s, ...);
void debug(int level, const char *s, ...);
void error(const char *s, ...);

#endif
