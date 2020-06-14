/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Low level software blitters.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#include <stdint.h>
#include "gbuffer.h"
#include "misc.h"

long __cdecl Buffer_To_Buffer(GraphicViewPortClass* vp, int x, int y, int w, int h, void* buffer, int length)
{
	int xstart = x;
	int ystart = y;
	int xend = x + w - 1;
	int yend = y + h - 1;

	int xoffset = 0;
	int yoffset = 0;

	// If we aren't drawing within the viewport, return
	if (!buffer || length <= 0 || xstart >= vp->Get_Width() || ystart >= vp->Get_Height() || xend < 0 || yend < 0) {
		return 0;
	}

	// Clipping
	if (xstart < 0) {
		xoffset = -xstart;
		xstart = 0;
	}

	if (ystart < 0) {
		yoffset += h * (-ystart);
		ystart = 0;
	}

	xend = min(xend, (int)vp->Get_Width() - 1);
	yend = min(yend, (int)vp->Get_Height() - 1);

	// Setup parameters for blit
	int pitch = vp->Get_Pitch() + vp->Get_Width() + vp->Get_XAdd();
	uint8_t* src = y * pitch + x + (uint8_t*)(vp->Get_Offset());
	uint8_t* dst = xoffset + w * yoffset + (uint8_t*)(buffer);
	// int src_pitch = x_pos + pitch - xend + 1;
	// int dst_pitch = x_pos + width - xend + 1;
	int lines = yend - ystart + 1;
	int blit_width = xend - xstart + 1;

	// Is our buffer large enough?
	if (lines * w <= length) {
		// blit
		while (lines--) {
			memcpy(dst, src, blit_width);
			src += pitch;
			dst += w;
		}
	}

	return 1;
}
