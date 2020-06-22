//
// Copyright 2020 Electronic Arts Inc.
//
// TiberianDawn.DLL and RedAlert.dll and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection


/*
** 
**   Misc. assembly code moved from headers
** 
**   jmarshall: This file heavily uses Chronoshift code to replace inline assembly functions
**   https://github.com/TheAssemblyArmada/Chronoshift
** 
** 
*/

#include "FUNCTION.H"



extern "C" void __cdecl Mem_Copy(void const *source, void *dest, unsigned long bytes_to_copy)
{
	memcpy(dest, source, bytes_to_copy);
}			  

/*
;***************************************************************************
;* Desired_Facing256 -- Desired facing algorithm 0..255 resolution.        *
;*                                                                         *
;*    This is a desired facing algorithm that has a resolution of 0        *
;*    through 255.                                                         *
;*                                                                         *
;* INPUT:   srcx,srcy   -- Source coordinate.                              *
;*                                                                         *
;*          dstx,dsty   -- Destination coordinate.                         *
;*                                                                         *
;* OUTPUT:  Returns with the desired facing to face the destination        *
;*          coordinate from the position of the source coordinate.  North  *
;*          is 0, East is 64, etc.                                         *
;*                                                                         *
;* WARNINGS:   This routine is slower than the other forms of desired      *
;*             facing calculation.  Use this routine when accuracy is      *
;*             required.                                                   *
;*                                                                         *
;* HISTORY:                                                                *
;*   12/24/1991 JLB : Adapted.                                             *
;*=========================================================================*/

int __cdecl Desired_Facing256(LONG x1, LONG y1, LONG x2, LONG y2)
{
	int8_t unk1 = 0;

	int x_diff = x2 - x1;

	if (x_diff < 0) {
		x_diff = -x_diff;
		unk1 = -64;
	}

	int y_diff = y1 - y2;

	if (y_diff < 0) {
		unk1 ^= 64;
		y_diff = -y_diff;
	}

	int s_diff;
	unsigned l_diff;

	if (x_diff != 0 || y_diff != 0) {
		if (x_diff >= y_diff) {
			s_diff = y_diff;
			l_diff = x_diff;
		}
		else {
			s_diff = x_diff;
			l_diff = y_diff;
		}

		unsigned unk2 = 32 * s_diff / l_diff;
		int ranged_dir = unk1 & 64;

		if (x_diff > y_diff) {
			ranged_dir = ranged_dir ^ 64;
		}

		if (ranged_dir != 0) {
			unk2 = ranged_dir - unk2 - 1;
		}

		return (DirType)((unk2 + unk1) & 255);
	}

	return 255;
}		 












/*

;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Support Library                          *
;*                                                                         *
;*                    File Name : FACING8.ASM                              *
;*                                                                         *
;*                   Programmer : Joe L. Bostic                            *
;*                                                                         *
;*                   Start Date : May 8, 1991                              *
;*                                                                         *
;*                  Last Update : February 6, 1995  [BWG]                  *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   Desired_Facing8 -- Determines facing to reach a position.             *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *


IDEAL
P386
MODEL USE32 FLAT

GLOBAL	 C Desired_Facing8	:NEAR
;	INCLUDE	"wwlib.i"

	DATASEG

; 8 direction desired facing lookup table.  Build the index according
; to the following bits:
;
; bit 3 = Is y2 < y1?
; bit 2 = Is x2 < x1?
; bit 1 = Is the ABS(x2-x1) < ABS(y2-y1)?
; bit 0 = Is the facing closer to a major axis?
//NewFacing8	DB	1,2,1,0,7,6,7,0,3,2,3,4,5,6,5,4

//	CODESEG
*/

/*
;***************************************************************************
;* DESIRED_FACING8 -- Determines facing to reach a position.               *
;*                                                                         *
;*    This routine will return with the most desirable facing to reach     *
;*    one position from another.  It is accurate to a resolution of 0 to   *
;*    7.                                                                   *
;*                                                                         *
;* INPUT:       x1,y1   -- Position of origin point.                       *
;*                                                                         *
;*              x2,y2   -- Position of target.                             *
;*                                                                         *
;* OUTPUT:      Returns desired facing as a number from 0..255 with an     *
;*              accuracy of 32 degree increments.                          *
;*                                                                         *
;* WARNINGS:    If the two coordinates are the same, then -1 will be       *
;*              returned.  It is up to you to handle this case.            *
;*                                                                         *
;* HISTORY:                                                                *
;*   07/15/1991 JLB : Documented.                                          *
;*   08/08/1991 JLB : Same position check.                                 *
;*   08/14/1991 JLB : New algorithm                                        *
;*   02/06/1995 BWG : Convert to 32-bit                                    *
;*=========================================================================*
*/
int __cdecl Desired_Facing8(long x1, long y1, long x2, long y2)
{
	
	int xdiff = 0;
	int ydiff = 0;
	char dirtype = 0;

	xdiff = x2 - x1;

	if (xdiff < 0) {
		dirtype = -64;
		xdiff = -xdiff;
	}

	ydiff = y1 - y2;

	if (ydiff < 0) {
		dirtype ^= 0x40;
		ydiff = -ydiff;
	}

	unsigned int lower_diff;

	if (xdiff >= ydiff) {
		lower_diff = ydiff;
		ydiff = xdiff;
	}
	else {
		lower_diff = xdiff;
	}

	char ranged_dir;

	if (((unsigned)(ydiff + 1) >> 1) > lower_diff) {
		ranged_dir = ((unsigned)dirtype) & 64;

		if (xdiff == ydiff) {
			ranged_dir ^= 64;
		}

		return (DirType)(dirtype + ranged_dir);
	}

	return (DirType)(dirtype + 32);
}












/*
;***********************************************************************************************
;* Cardinal_To_Fixed -- Converts cardinal numbers into a fixed point number.                   *
;*                                                                                             *
;*    This utility function will convert cardinal numbers into a fixed point fraction. The     *
;*    use of fixed point numbers occurs throughout the product -- since it is a convenient     *
;*    tool. The fixed point number is based on the formula:                                    *
;*                                                                                             *
;*       result = cardinal / base                                                              *
;*                                                                                             *
;*    The accuracy of the fixed point number is limited to 1/65536 as the lowest and up to     *
;*    65536 as the largest.                                                                    *
;*                                                                                             *
;* INPUT:   base     -- The key number to base the fraction about.                             *
;*                                                                                             *
;*          cardinal -- The other number (hey -- what do you call it?)                         *
;*                                                                                             *
;* OUTPUT:  Returns with the fixed point number of the "cardinal" parameter as it relates      *
;*          to the "base" parameter.                                                           *
;*                                                                                             *
;* WARNINGS:   none                                                                            *
;*                                                                                             *
;* HISTORY:                                                                                    *
;*   02/17/1995 BWG : Created.                                                                 *
;*=============================================================================================*/

unsigned int __cdecl Cardinal_To_Fixed(unsigned base, unsigned cardinal)
{
	// jmarshall: this is from me, hope this is right.
	unsigned int result; // eax

	result = -1;
	if (base)
		result = (cardinal << 16) / base;
	return result;
}

#if (0)
	PROC	Cardinal_To_Fixed C near
	USES	ebx, edx

	ARG	base:DWORD
	ARG	cardinal:DWORD

	mov	eax,0FFFFh		; establish default return value

	mov	ebx,[base]
	or	ebx,ebx
	jz	near ??retneg1		; if base==0, return 65535

	mov	eax,[cardinal]		; otherwise, return (cardinal*256)/base
	shl	eax,8
	xor	edx,edx
	div	ebx

??retneg1:
	ret

	ENDP	Cardinal_To_Fixed
#endif

/*
;***********************************************************************************************
;* Fixed_To_Cardinal -- Converts a fixed point number into a cardinal number.                  *
;*                                                                                             *
;*    Use this routine to convert a fixed point number into a cardinal number.                 *
;*                                                                                             *
;* INPUT:   base     -- The base number that the original fixed point number was created from. *
;*                                                                                             *
;*          fixed    -- The fixed point number to convert.                                     *
;*                                                                                             *
;* OUTPUT:  Returns with the reconverted number.                                               *
;*                                                                                             *
;* WARNINGS:   none                                                                            *
;*                                                                                             *
;* HISTORY:                                                                                    *
;*   02/17/1995 BWG : Created.                                                                 *
;*=============================================================================================*/

unsigned int __cdecl Fixed_To_Cardinal(unsigned base, unsigned fixed)
{
	// jmarshall: this one is from me I hope its right.
	return (fixed * base + 0x8000) >> 16;
}

// jmarshall: this is from chronoshift
void __cdecl Set_Bit(void * array, int bit, int value)
{
	// Don't try and handle a negative value
	if (bit < 0) {
		return;
	}

	uint8_t* byte_array = (uint8_t*)array;

	if (value) {
		byte_array[bit / 8] |= 1 << (bit % 8);
	}
	else {
		byte_array[bit / 8] &= ~(1 << (bit % 8));
	}
}


int __cdecl Get_Bit(void const * array, int bit)
{
	// If negative it is out of range so can't be set
	if (bit < 0) {
		return false;
	}

	const uint8_t* byte_array = (const uint8_t*)array;

	return byte_array[bit / 8] & (1 << (bit % 8)) ? true : false;
}

/**
 * @brief Scans through the data and returns the position of the first set bit found.
 */
int First_True_Bit(const void* array, int size)
{
	const uint8_t* byte_array = (const uint8_t*)array;

	int bytenum = 0;
	int bitnum = 0;

	// Find the first none zero byte as it must contain the first bit.
	for (bytenum = 0; bytenum < size; ++bytenum) {
		if (byte_array[bytenum] != 0) {
			break;
		}
	}

	if (bytenum >= size) {
		return 8 * bytenum;
	}

	// Scan through the bits of the byte until we find the first set bit.
	for (bitnum = 0; bitnum < 8; ++bitnum) {
		if (Get_Bit(&byte_array[bytenum], bitnum)) {
			break;
		}
	}

	return 8 * bytenum + bitnum;
}

/**
 * @brief Scans through the data and returns the position of the first clear bit found.
 */
int First_False_Bit(const void* array, int size)
{
	const uint8_t* byte_array = (const uint8_t*)array;

	int bytenum = 0;
	int bitnum = 0;

	// Find the first byte with an unset bit as it must contain the first bit.
	for (bytenum = 0; bytenum < size; ++bytenum) {
		if (byte_array[bytenum] != 0xFF) {
			break;
		}
	}

	if (bytenum >= size) {
		return 8 * bytenum;
	}

	// Scan through the bits until we find one that isn't set.
	for (bitnum = 0; bitnum < 8; ++bitnum) {
		if (!Get_Bit(&byte_array[bytenum], bitnum)) {
			break;
		}
	}

	return 8 * bytenum + bitnum;
}

int __cdecl Bound(int original, int min, int max)
{		
	// jmarshall: this is just a clamp, todo simplify me!
	int result; // eax
	int v4; // ebx
	int v5; // ecx

	result = original;
	v4 = min;
	v5 = max;
	if (min >= max)
	{
		v4 = max;
		v5 = min;
	}
	if (original <= v4)
		result = v4;
	if (result >= v5)
		result = v5;
	return result;
}







/*

CELL __cdecl Coord_Cell(COORDINATE coord)
{
	__asm {
		mov	eax, coord
		mov	ebx,eax
		shr	eax,010h
		xor	al,al
		shr	eax,2
		or		al,bh
	}

}



*/





/*
;***********************************************************
; SHAKE_SCREEN
;
; VOID Shake_Screen(int shakes);
;
; This routine shakes the screen the number of times indicated.
;
; Bounds Checking: None
;
;*
*/ 
void __cdecl Shake_Screen(int shakes)
{
	// PG_TO_FIX	
	// Need a different solution for shaking the screen
	shakes;
}



#if (0)
GLOBAL	C Shake_Screen	:NEAR

	CODESEG

;***********************************************************
; SHAKE_SCREEN
;
; VOID Shake_Screen(int shakes);
;
; This routine shakes the screen the number of times indicated.
;
; Bounds Checking: None
;
;*
	PROC	Shake_Screen C near
	USES	ecx, edx

	ARG	shakes:DWORD
 ret

	mov	ecx,[shakes]

;;; push es
;;; mov ax,40h
;;; mov es,ax
;;; mov dx,[es:63h]
;;; pop es
	mov	eax,[0463h]		; get CRTC I/O port
	mov	dx,ax
	add	dl,6			; video status port

??top_loop:

??start_retrace:
	in	al,dx
	test	al,8
	jz	??start_retrace

??end_retrace:
	in	al,dx
	test	al,8
	jnz	??end_retrace

	cli
	sub	dl,6			; dx = 3B4H or 3D4H

	mov	ah,01			; top word of start address
	mov	al,0Ch
	out	dx,al
	xchg	ah,al
	inc	dx
	out	dx,al
	xchg	ah,al
	dec	dx

	mov	ah,040h			; bottom word = 40 (140h)
	inc	al
	out	dx,al
	xchg	ah,al
	inc	dx
	out	dx,al
	xchg	ah,al

	sti
	add	dl,5

??start_retrace2:
	in	al,dx
	test	al,8
	jz	??start_retrace2

??end_retrace2:
	in	al,dx
	test	al,8
	jnz	??end_retrace2

??start_retrace3:
	in	al,dx
	test	al,8
	jz	??start_retrace3

??end_retrace3:
	in	al,dx
	test	al,8
	jnz	??end_retrace3

	cli
	sub	dl,6			; dx = 3B4H or 3D4H

	mov	ah,0
	mov	al,0Ch
	out	dx,al
	xchg	ah,al
	inc	dx
	out	dx,al
	xchg	ah,al
	dec	dx

	mov	ah,0
	inc	al
	out	dx,al
	xchg	ah,al
	inc	dx
	out	dx,al
	xchg	ah,al

	sti
	add	dl,5

	loop	??top_loop

	ret

	ENDP	Shake_Screen

;***********************************************************

	END

#endif














/*

;***************************************************************************
;* Conquer_Build_Fading_Table -- Builds custom shadow/light fading table.  *
;*                                                                         *
;*    This routine is used to build a special fading table for C&C.  There *
;*    are certain colors that get faded to and cannot be faded again.      *
;*    With this rule, it is possible to draw a shadow multiple times and   *
;*    not have it get any lighter or darker.                               *
;*                                                                         *
;* INPUT:   palette  -- Pointer to the 768 byte IBM palette to build from. *
;*                                                                         *
;*          dest     -- Pointer to the 256 byte remap table.               *
;*                                                                         *
;*          color    -- Color index of the color to "fade to".             *
;*                                                                         *
;*          frac     -- The fraction to fade to the specified color        *
;*                                                                         *
;* OUTPUT:  Returns with pointer to the remap table.                       *
;*                                                                         *
;* WARNINGS:   none                                                        *
;*                                                                         *
;* HISTORY:                                                                *
;*   10/07/1992 JLB : Created.                                             *
;*=========================================================================*/

// This function is from Chronoshift.
void * __cdecl Conquer_Build_Fading_Table(void const *paletteptr, void *fade_table, int color, int frac)
{	
	const PaletteClass& palette = *((const PaletteClass*)paletteptr);
	if (fade_table) {
		uint8_t* dst = static_cast<uint8_t*>(fade_table);
		RGBClass const* target_col = &palette[color];

		for (int i = 0; i < 256; ++i) {
			if (i <= 240 && i) {
				RGBClass tmp = palette[i];
				tmp.Adjust(frac, *target_col);

				int index = 0;
				int prevdiff = -1;

				for (int j = 240; j < 255; ++j) {
					int difference = palette[j].Difference(tmp);

					if (prevdiff == -1 || difference < prevdiff) {
						index = j;
						prevdiff = difference;
					}
				}

				dst[i] = index;
			}
			else {
				dst[i] = i;
			}
		}
	}

	return fade_table;
}






extern "C" long __cdecl Reverse_Long(long number)
{
#if 0 // jmarshall
	__asm {
		mov	eax,dword ptr [number]
		xchg	al,ah
		ror	eax,16
		xchg	al,ah
	}
#else
	assert("Reverse_Long not implmeneted");
	return number;
#endif
}


extern "C" short __cdecl Reverse_Short(short number)
{
#if 0 // jmarshall
	__asm {
		mov	ax,[number]
		xchg	ah,al
	}
#else
	assert("Reverse_Short not implmeneted");
	return number;
#endif
}	



extern "C" long __cdecl Swap_Long(long number)
{
#if 0 // jmarshall
	__asm {
		mov	eax,dword ptr [number]
		ror	eax,16
	}
#else
	assert("Reverse_Short not implmeneted");
	return number;
#endif
}








/*



;***************************************************************************
;* strtrim -- Remove the trailing white space from a string.               *
;*                                                                         *
;*    Use this routine to remove white space characters from the beginning *
;*    and end of the string.        The string is modified in place by     *
;*    this routine.                                                        *
;*                                                                         *
;* INPUT:   buffer   -- Pointer to the string to modify.                   *
;*                                                                         *
;* OUTPUT:     none                                                        *
;*                                                                         *
;* WARNINGS:   none                                                        *
;*                                                                         *
;* HISTORY:                                                                *
;*   10/07/1992 JLB : Created.                                             *
;*=========================================================================*
; VOID cdecl strtrim(BYTE *buffer);
	global C	strtrim :NEAR
	PROC	strtrim C near
	USES	ax, edi, esi

	ARG	buffer:DWORD		; Pointer to string to modify.
*/
void __cdecl strtrim(char *buffer)
{
	char* v1; // esi
	char v2; // al
	char* v3; // edi
	char v4; // al
	char* v5; // edi

	if (buffer)
	{
		v1 = buffer;
		do
		{
			do
				v2 = *v1++;
			while (v2 == 32);
		} while (v2 == 9);
		*buffer = v2;
		v3 = buffer + 1;
		do
		{
			v4 = *v1++;
			*v3++ = v4;
		} while (v4);
		v5 = v3 - 1;
		do
		{
			do
				*v5-- = 0;
			while (*v5 == 32);
		} while (*v5 == 9);
	}
}


/*
;***************************************************************************
;* Fat_Put_Pixel -- Draws a fat pixel.                                     *
;*                                                                         *
;*    Use this routine to draw a "pixel" that is bigger than 1 pixel       *
;*    across.  This routine is faster than drawing a similar small shape   *
;*    and faster than calling Fill_Rect.                                   *
;*                                                                         *
;* INPUT:   x,y       -- Screen coordinates to draw the pixel's upper      *
;*                       left corner.                                      *
;*                                                                         *
;*          color     -- The color to render the pixel in.                 *
;*                                                                         *
;*          size      -- The number of pixels width of the big "pixel".    *
;*                                                                         *
;*          page      -- The pointer to a GraphicBuffer class or something *
;*                                                                         *
;* OUTPUT:  none                                                           *
;*                                                                         *
;* WARNINGS:   none                                                        *
;*                                                                         *
;* HISTORY:                                                                *
;*   03/17/1994 JLB : Created.                                             *
;*=========================================================================*
; VOID cdecl Fat_Put_Pixel(long x, long y, long color, long size, void *page)
	global C	Fat_Put_Pixel:NEAR
	PROC	Fat_Put_Pixel C near
	USES	eax, ebx, ecx, edx, edi, esi

	ARG	x:DWORD		; X coordinate of upper left pixel corner.
	ARG	y:DWORD		; Y coordinate of upper left pixel corner.
	ARG	color:DWORD	; Color to use for the "pixel".
	ARG	siz:DWORD	; Size of "pixel" to plot (square).
	ARG	gpage:DWORD	; graphic page address to plot onto
*/

void __cdecl Fat_Put_Pixel(int x, int y, int color, int siz, GraphicViewPortClass &gpage)
{

}











/*
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Westwood Library                         *
;*                                                                         *
;*                    File Name : CRC.ASM                                  *
;*                                                                         *
;*                   Programmer : Joe L. Bostic                            *
;*                                                                         *
;*                   Start Date : June 12, 1992                            *
;*                                                                         *
;*                  Last Update : February 10, 1995 [BWG]                  *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *

IDEAL
P386
MODEL USE32 FLAT

GLOBAL	C Calculate_CRC	:NEAR

	CODESEG
*/
/*
extern "C" long __cdecl Calculate_CRC(void *buffer, long length)
{
	unsigned long crc;

	unsigned long local_length = (unsigned long) length;

	__asm {
			; Load pointer to data block.
			mov	[crc],0
			pushad
			mov	esi,[buffer]
			cld

			; Clear CRC to default (NULL) value.
			xor	ebx,ebx

			//; Fetch the length of the data block to CRC.
			
			mov	ecx,[local_length]

			jecxz	short fini

			; Prepare the length counters.
			mov	edx,ecx
			and	dl,011b
			shr	ecx,2

			; Perform the bulk of the CRC scanning.
			jecxz	short remainder2
		accumloop:
			lodsd
			rol	ebx,1
			add	ebx,eax
			loop	accumloop

			; Handle the remainder bytes.
		remainder2:
			or	dl,dl
			jz	short fini
			mov	ecx,edx
			xor	eax,eax

			and 	ecx,0FFFFh
			push	ecx
		nextbyte:
			lodsb
			ror	eax,8
			loop	nextbyte
			pop	ecx
			neg	ecx
			add	ecx,4
			shl	ecx,3
			ror	eax,cl

		;nextbyte:
		;	shl	eax,8
		;	lodsb
		;	loop	nextbyte
			rol	ebx,1
			add	ebx,eax

		fini:
			mov	[crc],ebx
			popad
			mov	eax,[crc]
			//ret
	}
}


*/



extern "C" void __cdecl Set_Palette_Range(void *palette)
{
	if (palette == NULL) {
		return;
	}

	memcpy(CurrentPalette, palette, 768);
	Set_DD_Palette(palette);
}