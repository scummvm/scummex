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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/scaler.cpp,v 1.1 2003/09/27 09:53:42 yoshizf Exp $
 *
 */

#include "scaler.h"

void AdvMame2x(const uint8 *srcPtr, uint32 srcPitch, uint8 *dstPtr, uint32 dstPitch,
							 int width, int height) {
	const uint32 nextlineSrc = srcPitch / sizeof(uint8);
	const uint8 *p = srcPtr;

	const uint32 nextlineDst = dstPitch / sizeof(uint8);
	uint8 *q = dstPtr;
	
	uint8 A, B, C;
	uint8 D, E, F;
	uint8 G, H, I;

	while (height--) {
		B = *(p - 1 - nextlineSrc);
		E = *(p - 1);
		H = *(p - 1 + nextlineSrc);
		C = *(p - nextlineSrc);
		F = *(p);
		I = *(p + nextlineSrc);
		for (int i = 0; i < width; ++i) {
			p++;
			A = B; B = C; C = *(p - nextlineSrc);
			D = E; E = F; F = *(p);
			G = H; H = I; I = *(p + nextlineSrc);

			*(q) = D == B && B != F && D != H ? D : E;
			*(q + 1) = B == F && B != D && F != H ? F : E;
			*(q + nextlineDst) = D == H && D != B && H != F ? D : E;
			*(q + nextlineDst + 1) = H == F && D != H && B != F ? F : E;
			q += 2;
		}
		p += nextlineSrc - width;
		q += (nextlineDst - width) << 1;
	}
}

void AdvMame3x(const uint8 *srcPtr, uint32 srcPitch, uint8 *dstPtr, uint32 dstPitch,
							 int width, int height) {
	const uint32 nextlineSrc = srcPitch / sizeof(uint8);
	const uint8 *p = srcPtr;

	const uint32 nextlineDst = dstPitch / sizeof(uint8);
	uint8 *q = dstPtr;
	
	uint8 A, B, C;
	uint8 D, E, F;
	uint8 G, H, I;

	while (height--) {
		B = *(p - 1 - nextlineSrc);
		E = *(p - 1);
		H = *(p - 1 + nextlineSrc);
		C = *(p - nextlineSrc);
		F = *(p);
		I = *(p + nextlineSrc);
		for (int i = 0; i < width; ++i) {
			p++;
			A = B; B = C; C = *(p - nextlineSrc);
			D = E; E = F; F = *(p);
			G = H; H = I; I = *(p + nextlineSrc);

			*(q) = D == B && B != F && D != H ? D : E;
			*(q + 1) = E;
			*(q + 2) = B == F && B != D && F != H ? F : E;
			*(q + nextlineDst) = E;
			*(q + nextlineDst + 1) = E;
			*(q + nextlineDst + 2) = E;
			*(q + 2 * nextlineDst) = D == H && D != B && H != F ? D : E;
			*(q + 2 * nextlineDst + 1) = E;
			*(q + 2 * nextlineDst + 2) = H == F && D != H && B != F ? F : E;
			q += 3;
		}
		p += nextlineSrc - width;
		q += (nextlineDst - width) * 3;
	}
}

void scale(int scale, const uint8 *srcPtr, uint32 srcPitch, uint8 *dstPtr, uint32 dstPitch, int width, int height) {
	switch (scale) {
		case 2 : 
			AdvMame2x(srcPtr, srcPitch, dstPtr, dstPitch, width, height);
			break;
		case 3 :
			AdvMame3x(srcPtr, srcPitch, dstPtr, dstPitch, width, height);
			break;
	}
}
