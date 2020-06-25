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
** 
**  Misc asm functions from ww lib
**  ST - 12/19/2018 1:20PM
** 
** 
** 
** 
** 
** 
** 
** 
** 
** 
** 
*/

#include <stdint.h>
#include "gbuffer.h"
#include "MISC.H"
#include "WSA.H"

IconCacheClass::IconCacheClass (void)
{
	IsCached			=FALSE;
	SurfaceLost		=FALSE;
	DrawFrequency	=0;
	//CacheSurface	=NULL;
	IconSource		=NULL;
}

IconCacheClass::~IconCacheClass (void)
{
}		  

IconCacheClass	CachedIcons[MAX_CACHED_ICONS];

extern "C"{
IconSetType		IconSetList[MAX_ICON_SETS];
short				IconCacheLookup[MAX_LOOKUP_ENTRIES];
}

int		CachedIconsDrawn=0;		//Counter of number of cache hits
int		UnCachedIconsDrawn=0;	//Counter of number of cache misses
BOOL	CacheMemoryExhausted;	//Flag set if we have run out of video RAM


void Invalidate_Cached_Icons (void) {}
void Restore_Cached_Icons (void) {}
void Register_Icon_Set (void *icon_data , BOOL pre_cache) {};

//
// Prototypes for assembly language procedures in STMPCACH.ASM
//
extern "C" void __cdecl Clear_Icon_Pointers (void) {};
extern "C" void __cdecl Cache_Copy_Icon (void const *icon_ptr ,void * , int) {};
extern "C" int __cdecl Is_Icon_Cached (void const *icon_data , int icon) {return -1;};
extern "C" int __cdecl Get_Icon_Index (void *icon_ptr) {return 0;};
extern "C" int __cdecl Get_Free_Index (void) {return 0;};
extern "C" BOOL __cdecl Cache_New_Icon (int icon_index, void *icon_ptr) {return -1;};
extern "C" int __cdecl Get_Free_Cache_Slot(void) {return -1;}

void IconCacheClass::Draw_It (void *dest_surface , int x_pixel, int y_pixel, int window_left , int window_top , int window_width , int window_height) {}


extern bool renderHDTexture;
extern	int	CachedIconsDrawn;
extern	int	UnCachedIconsDrawn;


/*
;***************************************************************************
;* VVC::DRAW_LINE -- Scales a virtual viewport to another virtual viewport *
;*                                                                         *
;* INPUT:	WORD sx_pixel 	- the starting x pixel position		   *
;*		WORD sy_pixel	- the starting y pixel position		   *
;*		WORD dx_pixel	- the destination x pixel position	   *
;*		WORD dy_pixel   - the destination y pixel position	   *
;*		WORD color      - the color of the line to draw		   *
;*                                                                         *
;* Bounds Checking: Compares sx_pixel, sy_pixel, dx_pixel and dy_pixel	   *
;*       with the graphic viewport it has been assigned to.		   *
;*                                                                         *
;* HISTORY:                                                                *
;*   06/16/1994 PWG : Created.                                             *
;*   08/30/1994 IML : Fixed clipping bug.				   *
;*=========================================================================*
	PROC	Buffer_Draw_Line C NEAR
	USES	eax,ebx,ecx,edx,esi,edi
*/

void __cdecl Buffer_Draw_Line(void *this_object, int sx, int sy, int dx, int dy, unsigned char color) {
	
}





/*
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Westwood 32 bit Library                  *
;*                                                                         *
;*                    File Name : DRAWLINE.ASM                             *
;*                                                                         *
;*                   Programmer : Phil W. Gorrow                           *
;*                                                                         *
;*                   Start Date : June 16, 1994                            *
;*                                                                         *
;*                  Last Update : August 30, 1994   [IML]                  *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   VVC::Scale -- Scales a virtual viewport to another virtual viewport   *
;*   Normal_Draw -- jump loc for drawing  scaled line of normal pixel      *
;*   __DRAW_LINE -- Assembly routine to draw a line                        *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *

IDEAL
P386
MODEL USE32 FLAT

INCLUDE ".\drawbuff.inc"
INCLUDE ".\gbuffer.inc"


CODESEG
*/


/*
;***************************************************************************
;* VVC::DRAW_LINE -- Scales a virtual viewport to another virtual viewport *
;*                                                                         *
;* INPUT:	WORD sx_pixel 	- the starting x pixel position		   *
;*		WORD sy_pixel	- the starting y pixel position		   *
;*		WORD dx_pixel	- the destination x pixel position	   *
;*		WORD dy_pixel   - the destination y pixel position	   *
;*		WORD color      - the color of the line to draw		   *
;*                                                                         *
;* Bounds Checking: Compares sx_pixel, sy_pixel, dx_pixel and dy_pixel	   *
;*       with the graphic viewport it has been assigned to.		   *
;*                                                                         *
;* HISTORY:                                                                *
;*   06/16/1994 PWG : Created.                                             *
;*   08/30/1994 IML : Fixed clipping bug.				   *
;*=========================================================================*
	PROC	Buffer_Draw_Line C NEAR
	USES	eax,ebx,ecx,edx,esi,edi

	;*==================================================================
	;* Define the arguements that the function takes.
	;*==================================================================
	ARG	this_object:DWORD	; associated graphic view port
	ARG	x1_pixel:DWORD		; the start x pixel position
	ARG	y1_pixel:DWORD		; the start y pixel position
	ARG	x2_pixel:DWORD		; the dest x pixel position
	ARG	y2_pixel:DWORD		; the dest y pixel position
	ARG	color:DWORD		; the color we are drawing

	;*==================================================================
	;* Define the local variables that we will use on the stack
	;*==================================================================
	LOCAL	clip_min_x:DWORD
	LOCAL	clip_max_x:DWORD
	LOCAL	clip_min_y:DWORD
	LOCAL	clip_max_y:DWORD
	LOCAL	clip_var:DWORD
	LOCAL	accum:DWORD
	LOCAL	bpr:DWORD

	;*==================================================================
	;* Take care of find the clip minimum and maximums
	;*==================================================================
	mov	ebx,[this_object]
	xor	eax,eax
	mov	[clip_min_x],eax
	mov	[clip_min_y],eax
	mov	eax,[(GraphicViewPort ebx).GVPWidth]
	mov	[clip_max_x],eax
	add	eax,[(GraphicViewPort ebx).GVPXAdd]
	add	eax,[(GraphicViewPort ebx).GVPPitch]
	mov	[bpr],eax
	mov	eax,[(GraphicViewPort ebx).GVPHeight]
	mov	[clip_max_y],eax

	;*==================================================================
	;* Adjust max pixels as they are tested inclusively.
	;*==================================================================
	dec	[clip_max_x]
	dec	[clip_max_y]

	;*==================================================================
	;* Set the registers with the data for drawing the line
	;*==================================================================
	mov	eax,[x1_pixel]		; eax = start x pixel position
	mov	ebx,[y1_pixel]		; ebx = start y pixel position
	mov	ecx,[x2_pixel]		; ecx = dest x pixel position
	mov	edx,[y2_pixel]		; edx = dest y pixel position

	;*==================================================================
	;* This is the section that "pushes" the line into bounds.
	;* I have marked the section with PORTABLE start and end to signify
	;* how much of this routine is 100% portable between graphics modes.
	;* It was just as easy to have variables as it would be for constants
	;* so the global vars ClipMaxX,ClipMinY,ClipMaxX,ClipMinY are used
	;* to clip the line (default is the screen)
	;* PORTABLE start
	;*==================================================================

	cmp	eax,[clip_min_x]
	jl	short ??clip_it
	cmp	eax,[clip_max_x]
	jg	short ??clip_it
	cmp	ebx,[clip_min_y]
	jl	short ??clip_it
	cmp	ebx,[clip_max_y]
	jg	short ??clip_it
	cmp	ecx,[clip_min_x]
	jl	short ??clip_it
	cmp	ecx,[clip_max_x]
	jg	short ??clip_it
	cmp	edx,[clip_min_y]
	jl	short ??clip_it
	cmp	edx,[clip_max_y]
	jle	short ??on_screen

	;*==================================================================
	;* Takes care off clipping the line.
	;*==================================================================
??clip_it:
	call	NEAR PTR ??set_bits
	xchg	eax,ecx
	xchg	ebx,edx
	mov	edi,esi
	call	NEAR PTR ??set_bits
	mov	[clip_var],edi
	or	[clip_var],esi
	jz	short ??on_screen
	test	edi,esi
	jne	short ??off_screen
	shl	esi,2
	call	[DWORD PTR cs:??clip_tbl+esi]
	jc	??clip_it
	xchg	eax,ecx
	xchg	ebx,edx
	shl	edi,2
	call	[DWORD PTR cs:??clip_tbl+edi]
	jmp	??clip_it

??on_screen:
	jmp	??draw_it

??off_screen:
	jmp	??out

	;*==================================================================
	;* Jump table for clipping conditions
	;*==================================================================
??clip_tbl	DD	??nada,??a_up,??a_dwn,??nada
		DD	??a_lft,??a_lft,??a_dwn,??nada
		DD	??a_rgt,??a_up,??a_rgt,??nada
		DD	??nada,??nada,??nada,??nada

??nada:
	clc
	retn

??a_up:
	mov	esi,[clip_min_y]
	call	NEAR PTR ??clip_vert
	stc
	retn

??a_dwn:
	mov	esi,[clip_max_y]
	neg	esi
	neg	ebx
	neg	edx
	call	NEAR PTR ??clip_vert
	neg	ebx
	neg	edx
	stc
	retn

	;*==================================================================
	;* xa'=xa+[(miny-ya)(xb-xa)/(yb-ya)]
	;*==================================================================
??clip_vert:
	push	edx
	push	eax
	mov	[clip_var],edx		; clip_var = yb
	sub	[clip_var],ebx		; clip_var = (yb-ya)
	neg	eax			; eax=-xa
	add	eax,ecx			; (ebx-xa)
	mov	edx,esi			; edx=miny
	sub	edx,ebx			; edx=(miny-ya)
	imul	edx
	idiv	[clip_var]
	pop	edx
	add	eax,edx
	pop	edx
	mov	ebx,esi
	retn

??a_lft:
	mov	esi,[clip_min_x]
	call	NEAR PTR ??clip_horiz
	stc
	retn

??a_rgt:
	mov	esi,[clip_max_x]
	neg	eax
	neg	ecx
	neg	esi
	call	NEAR PTR ??clip_horiz
	neg	eax
	neg	ecx
	stc
	retn

	;*==================================================================
	;* ya'=ya+[(minx-xa)(yb-ya)/(xb-xa)]
	;*==================================================================
??clip_horiz:
	push	edx
	mov	[clip_var],ecx		; clip_var = xb
	sub	[clip_var],eax		; clip_var = (xb-xa)
	sub	edx,ebx			; edx = (yb-ya)
	neg	eax			; eax = -xa
	add	eax,esi			; eax = (minx-xa)
	imul	edx			; eax = (minx-xa)(yb-ya)
	idiv	[clip_var]		; eax = (minx-xa)(yb-ya)/(xb-xa)
	add	ebx,eax			; ebx = xa+[(minx-xa)(yb-ya)/(xb-xa)]
	pop	edx
	mov	eax,esi
	retn

	;*==================================================================
	;* Sets the condition bits
	;*==================================================================
??set_bits:
	xor	esi,esi
	cmp	ebx,[clip_min_y]	; if y >= top its not up
	jge	short ??a_not_up
	or	esi,1

??a_not_up:
	cmp	ebx,[clip_max_y]	; if y <= bottom its not down
	jle	short ??a_not_down
	or	esi,2

??a_not_down:
	cmp	eax,[clip_min_x]   	; if x >= left its not left
	jge	short ??a_not_left
	or	esi,4

??a_not_left:
	cmp	eax,[clip_max_x]	; if x <= right its not right
	jle	short ??a_not_right
	or	esi,8

??a_not_right:
	retn

	;*==================================================================
	;* Draw the line to the screen.
	;* PORTABLE end
	;*==================================================================
??draw_it:
	sub	edx,ebx			; see if line is being draw down
	jnz	short ??not_hline	; if not then its not a hline
	jmp	short ??hline		; do special case h line

??not_hline:
	jg	short ??down		; if so there is no need to rev it
	neg	edx			; negate for actual pixel length
	xchg	eax,ecx			; swap x's to rev line draw
	sub	ebx,edx			; get old edx

??down:
	push	edx
	push	eax
	mov	eax,[bpr]
	mul	ebx
	mov	ebx,eax
	mov	eax,[this_object]
	add	ebx,[(GraphicViewPort eax).GVPOffset]
	pop	eax
	pop	edx

	mov	esi,1			; assume a right mover
	sub	ecx,eax			; see if line is right
	jnz	short ??not_vline	; see if its a vertical line
	jmp	??vline

??not_vline:
	jg	short ??right		; if so, the difference = length

??left:
	neg	ecx			; else negate for actual pixel length
	neg	esi			; negate counter to move left

??right:
	cmp	ecx,edx			; is it a horiz or vert line
	jge	short ??horiz		; if ecx > edx then |x|>|y| or horiz

??vert:
	xchg	ecx,edx			; make ecx greater and edx lesser
	mov	edi,ecx			; set greater
	mov	[accum],ecx		; set accumulator to 1/2 greater
	shr	[accum],1

	;*==================================================================
	;* at this point ...
	;* eax=xpos ; ebx=page line offset; ecx=counter; edx=lesser; edi=greater;
	;* esi=adder; accum=accumulator
	;* in a vertical loop the adder is conditional and the inc constant
	;*==================================================================
??vert_loop:
	add	ebx,eax
	mov	eax,[color]

??v_midloop:
	mov	[ebx],al
	dec	ecx
	jl	??out
	add	ebx,[bpr]
	sub	[accum],edx		; sub the lesser
	jge	??v_midloop		; any line could be new
	add	[accum],edi		; add greater for new accum
	add	ebx,esi			; next pixel over
	jmp	??v_midloop

??horiz:
	mov	edi,ecx			; set greater
	mov	[accum],ecx		; set accumulator to 1/2 greater
	shr	[accum],1

	;*==================================================================
	;* at this point ...
	;* eax=xpos ; ebx=page line offset; ecx=counter; edx=lesser; edi=greater;
	;* esi=adder; accum=accumulator
	;* in a vertical loop the adder is conditional and the inc constant
	;*==================================================================
??horiz_loop:
	add	ebx,eax
	mov	eax,[color]

??h_midloop:
	mov	[ebx],al
	dec	ecx				; dec counter
	jl	??out				; end of line
	add	ebx,esi
	sub     [accum],edx			; sub the lesser
	jge	??h_midloop
	add	[accum],edi			; add greater for new accum
	add	ebx,[bpr]			; goto next line
	jmp	??h_midloop

	;*==================================================================
	;* Special case routine for horizontal line draws
	;*==================================================================
??hline:
	cmp	eax,ecx			; make eax < ecx
	jl	short ??hl_ac
	xchg	eax,ecx

??hl_ac:
	sub	ecx,eax			; get len
	inc	ecx

	push	edx
	push	eax
	mov	eax,[bpr]
	mul	ebx
	mov	ebx,eax
	mov	eax,[this_object]
	add	ebx,[(GraphicViewPort eax).GVPOffset]
	pop	eax
	pop	edx
	add	ebx,eax
	mov	edi,ebx
	cmp	ecx,15
	jg	??big_line
	mov	al,[byte color]
	rep	stosb			; write as many words as possible
	jmp	short ??out		; get outt


??big_line:
	mov	al,[byte color]
	mov	ah,al
	mov     ebx,eax
	shl	eax,16
	mov	ax,bx
	test	edi,3
	jz	??aligned
	mov	[edi],al
	inc	edi
	dec	ecx
	test	edi,3
	jz	??aligned
	mov	[edi],al
	inc	edi
	dec	ecx
	test	edi,3
	jz	??aligned
	mov	[edi],al
	inc	edi
	dec	ecx

??aligned:
	mov	ebx,ecx
	shr	ecx,2
	rep	stosd
	mov	ecx,ebx
	and	ecx,3
	rep	stosb
	jmp	??out


	;*==================================================================
	;* a special case routine for vertical line draws
	;*==================================================================
??vline:
	mov	ecx,edx			; get length of line to draw
	inc	ecx
	add	ebx,eax
	mov	eax,[color]

??vl_loop:
	mov	[ebx],al		; store bit
	add	ebx,[bpr]
	dec	ecx
	jnz	??vl_loop

??out:
	ret
	ENDP	Buffer_Draw_Line


*/















/*

;***************************************************************************
;* GVPC::FILL_RECT -- Fills a rectangular region of a graphic view port	   *
;*                                                                         *
;* INPUT:	WORD the left hand x pixel position of region		   *
;*		WORD the upper x pixel position of region		   *
;*		WORD the right hand x pixel position of region		   *
;*		WORD the lower x pixel position of region		   *
;*		UBYTE the color (optional) to clear the view port to	   *
;*                                                                         *
;* OUTPUT:      none                                                       *
;*                                                                         *
;* NOTE:	This function is optimized to handle viewport with no XAdd *
;*		value.  It also handles DWORD aligning the destination	   *
;*		when speed can be gained by doing it.			   *
;* HISTORY:                                                                *
;*   06/07/1994 PWG : Created.                                             *
;*=========================================================================*
*/ 

/*
;******************************************************************************
; Much testing was done to determine that only when there are 14 or more bytes
; being copied does it speed the time it takes to do copies in this algorithm.
; For this reason and because 1 and 2 byte copies crash, is the special case
; used.  SKB 4/21/94.  Tested on 486 66mhz.  Copied by PWG 6/7/04.
*/ 
#define OPTIMAL_BYTE_COPY	14


void __cdecl Buffer_Fill_Rect(void *thisptr, int x, int y, int w, int h, unsigned char color)
{
	GraphicViewPortClass& vp = (*(GraphicViewPortClass *)thisptr);

	// If we aren't drawing within the viewport, return
	if (x >= vp.Get_Width() || y >= vp.Get_Height() || w < 0 || h < 0) {
		return;
	}

	// Clipping
	if (x < 0) {
		x = 0;
	}

	if (y < 0) {
		y = 0;
	}

	w = min(w, vp.Get_Width() - 1);
	h = min(h, vp.Get_Height() - 1);

	uint8_t* offset = (y * 4) * vp.Get_Full_Pitch() + (x * 4) + (uint8_t*)(vp.Get_Offset());
	int height = h - y + 1;
	int width = w - x + 1;

	for (int i = 0; i < height; ++i) {
		FastScanlinePaletteBlit(offset, color, width);
		offset += vp.Get_Full_Pitch() * 4;
	}
}




/*
;***************************************************************************
;* VVPC::CLEAR -- Clears a virtual viewport instance                       *
;*                                                                         *
;* INPUT:	UBYTE the color (optional) to clear the view port to	   *
;*                                                                         *
;* OUTPUT:      none                                                       *
;*                                                                         *
;* NOTE:	This function is optimized to handle viewport with no XAdd *
;*		value.  It also handles DWORD aligning the destination	   *
;*		when speed can be gained by doing it.			   *
;* HISTORY:                                                                *
;*   06/07/1994 PWG : Created.                                             *
;*   08/23/1994 SKB : Clear the direction flag to always go forward.       *
;*=========================================================================*
*/
void	__cdecl Buffer_Clear(void *this_object, unsigned char color)
{
	GraphicViewPortClass* cls = (GraphicViewPortClass*)this_object;
	if (cls->Get_Graphic_Buffer()->GetMemoryBuffer() == NULL)
		return;

	memset(cls->Get_Graphic_Buffer()->GetMemoryBuffer(), color, cls->Get_Width() * cls->Get_Height());
}

/*
;***************************************************************************
;* VVC::SCALE -- Scales a virtual viewport to another virtual viewport     *
;*                                                                         *
;* INPUT:                                                                  *
;*                                                                         *
;* OUTPUT:                                                                 *
;*                                                                         *
;* WARNINGS:                                                               *
;*                                                                         *
;* HISTORY:                                                                *
;*   06/16/1994 PWG : Created.                                             *
;*=========================================================================*
	PROC	Linear_Scale_To_Linear C NEAR
	USES	eax,ebx,ecx,edx,esi,edi
*/

// jmarshall - ported to c


unsigned char* Draw_Dropsample(const unsigned char* in, int inwidth, int inheight, int outwidth, int outheight) {
	int		i, j, k;
	const unsigned char* inrow;
	const unsigned char* pix1;
	unsigned char* out, * out_p;
	static unsigned char ViewportPixelBuffer[4096 * 4096 * 4];

	out = &ViewportPixelBuffer[0];
	out_p = out;

	int bpp = 1;
	if (renderHDTexture) {
		bpp = 4;
	}

	for (i = 0; i < outheight; i++, out_p += outwidth * bpp) {
		inrow = in + bpp * inwidth * (int)((i + 0.25) * inheight / outheight);
		for (j = 0; j < outwidth; j++) {
			k = j * inwidth / outwidth;
			pix1 = inrow + k * bpp;
			if (renderHDTexture) {
				out_p[j * 4 + 0] = pix1[0];
				out_p[j * 4 + 1] = pix1[1];
				out_p[j * 4 + 2] = pix1[2];
				out_p[j * 4 + 3] = pix1[3];
			}
			else {
				out_p[j * 1 + 0] = pix1[0];
			}
			//out_p[j * 3 + 1] = pix1[1];
			//out_p[j * 3 + 2] = pix1[2];
		}
	}

	return out;
}


/*
==============
R_CopyImage
==============
*/
void Nearest_CopyImage(unsigned char* source, int sourceX, int sourceY, int sourceWidth, unsigned char* dest, int destX, int destY, int destWidth, int width, int height, BOOL trans)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{

			int destPos = ((destWidth * (y + destY)) * 4) + ((x + destX) * 4);
			int sourcePos = (sourceWidth * (y + (sourceY))) + (x + (sourceX));
			if (renderHDTexture) {
				sourcePos = ((sourceWidth * (y + sourceY)) * 4) + ((x + sourceX) * 4);

				// This needs some work!
				if (source[0] == 0 && source[1] == 0 && source[2] == 0 && trans)
					continue;

				dest[destPos + 0] = source[sourcePos + 0];
				dest[destPos + 1] = source[sourcePos + 1];
				dest[destPos + 2] = source[sourcePos + 2];
				dest[destPos + 3] = 255;
			}
			else
			{
				unsigned char c = source[sourcePos];

				if (c == 0 && trans) {
					continue;
				}

				dest[destPos + 0] = backbuffer_palette[(c * 3) + 0];
				dest[destPos + 1] = backbuffer_palette[(c * 3) + 1];
				dest[destPos + 2] = backbuffer_palette[(c * 3) + 2];
				dest[destPos + 3] = 255;
			}
		}
	}
}


BOOL __cdecl Linear_Scale_To_Linear(void* this_object, void* dest, int src_x, int src_y, int dst_x, int dst_y, int src_width, int src_height, int dst_width, int dst_height, BOOL trans, char* remap)
{
	GraphicViewPortClass* viewportClass = (GraphicViewPortClass*)this_object;
	GraphicViewPortClass* destViewportClass = (GraphicViewPortClass*)dest;

	unsigned char* viewportBuffer = ((unsigned char*)viewportClass->Get_Offset());

	// Scale the GraphicViewPortClass to dest_x, dest_y
	unsigned char* scaled_buffer = Draw_Dropsample((unsigned char*)viewportBuffer, viewportClass->Get_Width(), viewportClass->Get_Height(), dst_width, dst_height);

	// Blit the scaled_buffer to dest.
	Nearest_CopyImage(scaled_buffer, src_x, src_y, dst_width, (unsigned char*)destViewportClass->Get_Offset(), dst_x, dst_y, destViewportClass->Get_Width(), dst_width, dst_height, trans);

	return true;
}

BOOL __cdecl Linear_Blit_To_Linear(void* this_object, void* dest, int x_pixel, int y_pixel, int dest_x0, int dest_y0, int pixel_width, int pixel_height, BOOL trans)
{
	GraphicViewPortClass* viewportClass = (GraphicViewPortClass*)this_object;
	GraphicViewPortClass* destViewportClass = (GraphicViewPortClass*)dest;
	bool needsUnlock = false;
	bool sourceNeedsUnlock = false;

	unsigned char* viewportBuffer = NULL;
	viewportBuffer = ((unsigned char*)viewportClass->Get_Offset());

	unsigned char* destBuffer = NULL;
	destBuffer = ((unsigned char*)destViewportClass->Get_Offset());

	//if (destViewportClass->Get_Graphic_Buffer()->Get_Buffer() != NULL) {
	//	destBuffer = (byte*)destViewportClass->Get_Graphic_Buffer()->Get_Buffer();
	//}
	//else {
	//	destViewportClass->Get_Graphic_Buffer()->Lock();
	//	destBuffer = ((byte*)destViewportClass->Get_Graphic_Buffer()->GetMemoryBuffer());
	//	needsUnlock = true;
	//}
	//
	//if (viewportBuffer == NULL) {
	//	viewportClass->Get_Graphic_Buffer()->Lock();
	//	viewportBuffer = ((byte*)viewportClass->Get_Graphic_Buffer()->GetMemoryBuffer());
	//	sourceNeedsUnlock = true;
	//}

	// Blit the buffer to dest.
	Nearest_CopyImage(viewportBuffer, x_pixel, y_pixel, viewportClass->Get_Width(), (unsigned char*)destBuffer, dest_x0, dest_y0, destViewportClass->Get_Width(), pixel_width, pixel_height, trans);

	//if (needsUnlock) {
	//	destViewportClass->Get_Graphic_Buffer()->Unlock();
	//}
	//
	//if (sourceNeedsUnlock) {
	//	viewportClass->Get_Graphic_Buffer()->Unlock();
	//}
	return true;
}

BOOL __cdecl Linear_Blit_To_Linear_Pal(void* this_object, void* dest, int src_x, int src_y, int dst_x, int dst_y, int w, int h, BOOL use_key)
{
	GraphicViewPortClass& src_vp = *(GraphicViewPortClass*)this_object;
	GraphicViewPortClass& dst_vp = *(GraphicViewPortClass*)dest;

	uint8_t* src = (uint8_t*)(src_vp.Get_Offset());
	uint8_t* dst = (uint8_t*)(dst_vp.Get_Offset());
	int src_pitch = src_vp.Get_Full_Pitch();
	int dst_pitch = dst_vp.Get_Full_Pitch();

	if (src_x >= src_vp.Get_Width() || src_y >= src_vp.Get_Height() || dst_x >= dst_vp.Get_Width()
		|| dst_y >= dst_vp.Get_Height() || h < 0 || w < 1) {
		return 0;
	}

	src_x = max(0, src_x);
	src_y = max(0, src_y);
	dst_x = max(0, dst_x);
	dst_y = max(0, dst_y);

	h = (dst_y + h) > dst_vp.Get_Height() ? dst_vp.Get_Height() - 1 - dst_y : h;
	w = (dst_x + w) > dst_vp.Get_Width() ? dst_vp.Get_Width() - 1 - dst_x : w;

	// move our pointers to the start locations
	src += src_x + src_y * src_pitch;
	dst += dst_x + dst_y * dst_pitch;

	// If src is before dst, we run the risk of overlapping memory regions so we
	// need to move src and dst to the last line and work backwards
	if (src < dst) {
		uint8_t* esrc = src + (h - 1) * src_pitch;
		uint8_t* edst = dst + (h - 1) * dst_pitch;
		if (use_key) {
			char key_colour = 0;
			while (h-- != 0) {
				// Can't optimize as we need to check every pixel against key colour :(
				for (int i = w - 1; i >= 0; --i) {
					if (esrc[i] != key_colour) {
						edst[i] = esrc[i];
					}
				}

				edst -= dst_pitch;
				esrc -= src_pitch;
			}
		}
		else {
			while (h-- != 0) {
				memmove(edst, esrc, w);
				edst -= dst_pitch;
				esrc -= src_pitch;
			}
		}
	}
	else {
		if (use_key) {
			uint8_t key_colour = 0;
			while (h-- != 0) {
				// Can't optimize as we need to check every pixel against key colour :(
				for (int i = 0; i < w; ++i) {
					if (src[i] != key_colour) {
						dst[i] = src[i];
					}
				}

				dst += dst_pitch;
				src += src_pitch;
			}
		}
		else {
			while (h-- != 0) {
				memmove(dst, src, w);
				dst += dst_pitch;
				src += src_pitch;
			}
		}
	}
	return true;
}

// jmarshall end





















unsigned int LastIconset = 0;
unsigned int StampPtr = 0;	//	DD	0	; Pointer to icon data.

unsigned int IsTrans = 0;	//		DD	0	; Pointer to transparent icon flag table.

unsigned int MapPtr = 0;	//		DD	0	; Pointer to icon map.
unsigned int IconWidth = 0;	//	DD	0	; Width of icon in pixels.
unsigned int IconHeight = 0;	//	DD	0	; Height of icon in pixels.
unsigned int IconSize = 0;		//	DD	0	; Number of bytes for each icon data.
unsigned int IconCount = 0;	//	DD	0	; Number of icons in the set.



#if (0)
LastIconset	DD	0	; Pointer to last iconset initialized.
StampPtr	DD	0	; Pointer to icon data.

IsTrans		DD	0	; Pointer to transparent icon flag table.

MapPtr		DD	0	; Pointer to icon map.
IconWidth	DD	0	; Width of icon in pixels.
IconHeight	DD	0	; Height of icon in pixels.
IconSize	DD	0	; Number of bytes for each icon data.
IconCount	DD	0	; Number of icons in the set.


GLOBAL C	Buffer_Draw_Stamp:near
GLOBAL C	Buffer_Draw_Stamp_Clip:near

; 256 color icon system.
#endif


/*
;***********************************************************
; INIT_STAMPS
;
; VOID cdecl Init_Stamps(VOID *icondata);
;
; This routine initializes the stamp data.
; Bounds Checking: NONE
;
;*
*/ 
extern "C" void __cdecl Init_Stamps(unsigned int icondata)
{

	
}


	 VOID __cdecl Buffer_Draw_Line(void *thisptr, int sx, int sy, int dx, int dy, unsigned char color);
	 VOID __cdecl Buffer_Fill_Rect(void *thisptr, int sx, int sy, int dx, int dy, unsigned char color);
	 VOID __cdecl Buffer_Remap(void * thisptr, int sx, int sy, int width, int height, void *remap);
	 VOID __cdecl Buffer_Fill_Quad(void * thisptr, VOID *span_buff, int x0, int y0, int x1, int y1,
							 	int x2, int y2, int x3, int y3, int color);
	 void __cdecl Buffer_Draw_Stamp(void const *thisptr, void const *icondata, int icon, int x_pixel, int y_pixel, void const *remap);
	 void __cdecl Buffer_Draw_Stamp_Clip(void const *thisptr, void const *icondata, int icon, int x_pixel, int y_pixel, void const *remap, int ,int,int,int);
	 void * __cdecl Get_Font_Palette_Ptr ( void );


/*
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Westwood 32 bit Library                  *
;*                                                                         *
;*                    File Name : REMAP.ASM                                *
;*                                                                         *
;*                   Programmer : Phil W. Gorrow                           *
;*                                                                         *
;*                   Start Date : July 1, 1994                             *
;*                                                                         *
;*                  Last Update : July 1, 1994   [PWG]                     *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
*/


VOID __cdecl Buffer_Remap(void * this_object, int sx, int sy, int width, int height, void *remap)
{

}















/*
; **************************************************************************
; **   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   *
; **************************************************************************
; *                                                                        *
; *                 Project Name : WSA Support routines			   *
; *                                                                        *
; *                    File Name : XORDELTA.ASM                            *
; *                                                                        *
; *                   Programmer : Scott K. Bowen			   *
; *                                                                        *
; *                  Last Update :May 23, 1994   [SKB]                     *
; *                                                                        *
; *------------------------------------------------------------------------*
; * Functions:                                                             *
;*   Apply_XOR_Delta -- Apply XOR delta data to a buffer.                  *
;*   Apply_XOR_Delta_To_Page_Or_Viewport -- Calls the copy or the XOR funti*
;*   Copy_Delta_buffer -- Copies XOR Delta Data to a section of a page.    *
;*   XOR_Delta_Buffer -- Xor's the data in a XOR Delta format to a page.   *
; * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*

IDEAL
P386
MODEL USE32 FLAT
*/


/*
LOCALS ??

; These are used to call Apply_XOR_Delta_To_Page_Or_Viewport() to setup flags parameter.  If
; These change, make sure and change their values in wsa.cpp.
DO_XOR		equ	0
DO_COPY		equ	1
TO_VIEWPORT	equ	0
TO_PAGE		equ	2

;
; Routines defined in this module
;
;
; UWORD Apply_XOR_Delta(UWORD page_seg, BYTE *delta_ptr);
; PUBLIC Apply_XOR_Delta_To_Page_Or_Viewport(UWORD page_seg, BYTE *delta_ptr, WORD width, WORD copy)
;
;	PROC	C XOR_Delta_Buffer
;	PROC	C Copy_Delta_Buffer
;

GLOBAL 	C Apply_XOR_Delta:NEAR
GLOBAL 	C Apply_XOR_Delta_To_Page_Or_Viewport:NEAR
*/

#define DO_XOR			0
#define DO_COPY		1
#define TO_VIEWPORT	0
#define TO_PAGE		2

void __cdecl XOR_Delta_Buffer(int nextrow);
void __cdecl Copy_Delta_Buffer(int nextrow);


/*
;***************************************************************************
;* APPLY_XOR_DELTA -- Apply XOR delta data to a linear buffer.             *
;*   AN example of this in C is at the botton of the file commented out.   *
;*                                                                         *
;* INPUT:  BYTE *target - destination buffer.                              *
;*         BYTE *delta - xor data to be delta uncompress.                  *
;*                                                                         *
;* OUTPUT:                                                                 *
;*                                                                         *
;* WARNINGS:                                                               *
;*                                                                         *
;* HISTORY:                                                                *
;*   05/23/1994 SKB : Created.                                             *
;*=========================================================================*
*/
unsigned int __cdecl Apply_XOR_Delta(char *target, char *delta)
{
	int8_t* putp = (int8_t*)(target);
	int8_t* getp = (int8_t*)(delta);
	uint16_t count = 0;
	int8_t value = 0;
	int8_t cmd = 0;

	while (true) {
		// captainslog_debug("XOR_Delta Put pos: %u, Get pos: %u.... ", putp - static_cast<int8_t*>(dest), getp -
		// static_cast<int8_t*>(source));
		cmd = *getp++;
		count = cmd;
		bool xorval = false;

		if (cmd >= 0) {
			// 0b00000000
			if (cmd == 0) {
				count = *getp++ & 0xFF;
				value = *getp++;
				xorval = true;
				// captainslog_debug("0b00000000 Val Count %d ", count);
				// 0b0???????
			}
			else {
				// captainslog_debug("0b0??????? XOR Count %d ", count);
			}
		}
		else {
			// 0b1??????? remove most significant bit
			count &= 0x7F;
			if (count != 0) {
				putp += count;
				// captainslog_debug("0b1??????? Skip Count %d", count);
				continue;
			}

			count = (*getp & 0xFF) + (*(getp + 1) << 8);
			getp += 2;

			// captainslog_debug("Eval %u ", count);

			// 0b10000000 0 0
			if (count == 0) {
				// captainslog_debug("0b10000000 Count %d to end delta", count);
				return 0;
			}

			// 0b100000000 0?
			if ((count & 0x8000) == 0) {
				putp += count;
				// captainslog_debug("0b100000000 0? Skip Count %d", count);
				continue;
			}
			else {
				// 0b10000000 11
				if (count & 0x4000) {
					count &= 0x3FFF;
					value = *getp++;
					// captainslog_debug("0b10000000 11 Val Count %d ", count);
					xorval = true;
					// 0b10000000 10
				}
				else {
					count &= 0x3FFF;
					// captainslog_debug("0b10000000 10 XOR Count %d ", count);
				}
			}
		}

		if (xorval) {
			// captainslog_debug("XOR Val %d", value);
			for (; count > 0; --count) {
				*putp++ ^= value;
			}
		}
		else {
			// captainslog_debug("XOR Source to Dest");
			for (; count > 0; --count) {
				*putp++ ^= *getp++;
			}
		}
	}
	return 0;
}


void Copy_Delta_Buffer(int width, void* offset, void* delta, int pitch)
{
	int8_t* putp = static_cast<int8_t*>(offset);
	int8_t* getp = static_cast<int8_t*>(delta);
	int8_t value = 0;
	int8_t cmd = 0;
	int length = 0;
	int count = 0;

	while (true) {
		// captainslog_debug("XOR_Delta Put pos: %u, Get pos: %u.... ", putp - static_cast<int8_t*>(dest), getp -
		// static_cast<int8_t*>(source));
		cmd = *getp++;
		count = cmd;
		bool xorval = false;

		if (cmd >= 0) {
			// 0b00000000
			if (cmd == 0) {
				count = *getp++ & 0xFF;
				value = *getp++;
				xorval = true;
				// captainslog_debug("0b00000000 Val Count %d ", count);
				// 0b0???????
			}
			else {
				// captainslog_debug("0b0??????? XOR Count %d ", count);
			}
		}
		else {
			// 0b1??????? remove most significant bit
			count &= 0x7F;
			if (count != 0) {
				putp -= length;
				length += count;

				while (length >= width) {
					length -= width;
					putp += pitch;
				}

				putp += length;
				// captainslog_debug("0b1??????? Skip Count %d", count);
				continue;
			}

			count = (*getp & 0xFF) + (*(getp + 1) << 8);
			getp += 2;

			// captainslog_debug("Eval %u ", count);

			// 0b10000000 0 0
			if (count == 0) {
				// captainslog_debug("0b10000000 Count %d to end delta", count);
				return;
			}

			// 0b100000000 0?
			if ((count & 0x8000) == 0) {
				putp -= length;
				length += count;

				while (length >= width) {
					length -= width;
					putp += pitch;
				}

				putp += length;
				// captainslog_debug("0b100000000 0? Skip Count %d", count);
				continue;
			}
			else {
				// 0b10000000 11
				if (count & 0x4000) {
					count &= 0x3FFF;
					value = *getp++;
					// captainslog_debug("0b10000000 11 Val Count %d ", count);
					xorval = true;
					// 0b10000000 10
				}
				else {
					count &= 0x3FFF;
					// captainslog_debug("0b10000000 10 XOR Count %d ", count);
				}
			}
		}

		if (xorval) {
			// captainslog_debug("XOR Val %d", value);
			for (; count > 0; --count) {
				*putp++ = value;
				++length;

				if (length == width) {
					length = 0;
					putp += pitch - width;
				}
			}
		}
		else {
			// captainslog_debug("XOR Source to Dest");
			for (; count > 0; --count) {
				*putp++ = *getp++;
				++length;

				if (length == width) {
					length = 0;
					putp += pitch - width;
				}
			}
		}
	}
}

void XOR_Delta_Buffer(int width, void* offset, void* delta, int pitch)
{
	int8_t* putp = static_cast<int8_t*>(offset);
	int8_t* getp = static_cast<int8_t*>(delta);
	int8_t value = 0;
	int8_t cmd = 0;
	int length = 0;
	int count = 0;

	while (true) {
		// captainslog_debug("XOR_Delta Put pos: %u, Get pos: %u.... ", putp - static_cast<int8_t*>(dest), getp -
		// static_cast<int8_t*>(source));
		cmd = *getp++;
		count = cmd;
		bool xorval = false;

		if (cmd >= 0) {
			// 0b00000000
			if (cmd == 0) {
				count = *getp++ & 0xFF;
				value = *getp++;
				xorval = true;
				// captainslog_debug("0b00000000 Val Count %d ", count);
				// 0b0???????
			}
			else {
				// captainslog_debug("0b0??????? XOR Count %d ", count);
			}
		}
		else {
			// 0b1??????? remove most significant bit
			count &= 0x7F;
			if (count != 0) {
				putp -= length;
				length += count;

				while (length >= width) {
					length -= width;
					putp += pitch;
				}

				putp += length;
				// captainslog_debug("0b1??????? Skip Count %d", count);
				continue;
			}

			count = (*getp & 0xFF) + (*(getp + 1) << 8);
			getp += 2;

			// captainslog_debug("Eval %u ", count);

			// 0b10000000 0 0
			if (count == 0) {
				// captainslog_debug("0b10000000 Count %d to end delta", count);
				return;
			}

			// 0b100000000 0?
			if ((count & 0x8000) == 0) {
				putp -= length;
				length += count;

				while (length >= width) {
					length -= width;
					putp += pitch;
				}

				putp += length;
				// captainslog_debug("0b100000000 0? Skip Count %d", count);
				continue;
			}
			else {
				// 0b10000000 11
				if (count & 0x4000) {
					count &= 0x3FFF;
					value = *getp++;
					// captainslog_debug("0b10000000 11 Val Count %d ", count);
					xorval = true;
					// 0b10000000 10
				}
				else {
					count &= 0x3FFF;
					// captainslog_debug("0b10000000 10 XOR Count %d ", count);
				}
			}
		}

		if (xorval) {
			// captainslog_debug("XOR Val %d", value);
			for (; count > 0; --count) {
				*putp++ ^= value;
				++length;

				if (length == width) {
					length = 0;
					putp += pitch - width;
				}
			}
		}
		else {
			// captainslog_debug("XOR Source to Dest");
			for (; count > 0; --count) {
				*putp++ ^= *getp++;
				++length;

				if (length == width) {
					length = 0;
					putp += pitch - width;
				}
			}
		}
	}
}

/*
;----------------------------------------------------------------------------

;***************************************************************************
;* APPLY_XOR_DELTA_To_Page_Or_Viewport -- Calls the copy or the XOR funtion.           *
;*                                                                         *
;*									   *
;* 	This funtion is call to either xor or copy XOR_Delta data onto a   *
;*	page instead of a buffer.  The routine will set up the registers   *
;*	need for the actual routines that will perform the copy or xor.	   *
;*									   *
;*	The registers are setup as follows :				   *
;*		es:edi - destination segment:offset onto page.		   *
;*		ds:esi - source buffer segment:offset of delta data.	   *
;*		dx,cx,ax - are all zeroed out before entry.		   *
;*                                                                         *
;* INPUT:                                                                  *
;*                                                                         *
;* OUTPUT:                                                                 *
;*                                                                         *
;* WARNINGS:                                                               *
;*                                                                         *
;* HISTORY:                                                                *
;*   03/09/1992  SB : Created.                                             *
;*=========================================================================*
*/

void __cdecl Apply_XOR_Delta_To_Page_Or_Viewport(void *offset, void *delta, int width, int pitch, int copy)
{
	if (copy) {
		Copy_Delta_Buffer(width, offset, delta, pitch);
	}
	else {
		XOR_Delta_Buffer(width, offset, delta, pitch);
	}
}




















/*
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Westwood Library                         *
;*                                                                         *
;*                    File Name : FADING.ASM                               *
;*                                                                         *
;*                   Programmer : Joe L. Bostic                            *
;*                                                                         *
;*                   Start Date : August 20, 1993                          *
;*                                                                         *
;*                  Last Update : August 20, 1993   [JLB]                  *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *

IDEAL
P386
MODEL USE32 FLAT

GLOBAL	C Build_Fading_Table	:NEAR

	CODESEG

;***********************************************************
; BUILD_FADING_TABLE
;
; void *Build_Fading_Table(void *palette, void *dest, long int color, long int frac);
;
; This routine will create the fading effect table used to coerce colors
; from toward a common value.  This table is used when Fading_Effect is
; active.
;
; Bounds Checking: None
;*
*/
void * __cdecl Build_Fading_Table(void const *palette, void const * fade_table, long int color, long int frac)
{
	if (fade_table == nullptr) {
		return nullptr;
	}

	uint8_t* table = (uint8_t*)(fade_table);
	uint8_t const* pal = reinterpret_cast<const uint8_t*>(&palette);

	// First entry always maps to itself for transparency
	*table++ = 0;

	for (int i = 1; i < 256; i++) {
		// Create adjusted versions of the palette entries based on reference
		// colour and fraction to adjust by.
		uint8_t red = pal[3 * i + 0] - (((pal[3 * i + 0] - pal[3 * color + 0]) * (frac / 2)) >> 7);
		uint8_t grn = pal[3 * i + 1] - (((pal[3 * i + 1] - pal[3 * color + 1]) * (frac / 2)) >> 7);
		uint8_t blu = pal[3 * i + 2] - (((pal[3 * i + 2] - pal[3 * color + 2]) * (frac / 2)) >> 7);
		uint8_t closest = color;
		unsigned min_diff = 0xFFFFFFFF;

		// For each palette entry calculate difference from adjusted version of
		// target color and find closest colour to use from palette.
		for (int j = 1; j < 256; ++j) {
			if (j == i) {
				continue;
			}

			unsigned diff = 0;

			diff += (pal[3 * j + 0] - red) * (pal[3 * j + 0] - red);
			diff += (pal[3 * j + 1] - grn) * (pal[3 * j + 1] - grn);
			diff += (pal[3 * j + 2] - blu) * (pal[3 * j + 2] - blu);

			if (!diff) {
				closest = j;
				break;
			}

			if (diff <= min_diff) {
				min_diff = diff;
				closest = j;
			}
		}

		*table++ = closest;
	}

	return (void *)fade_table;
}
























/*
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Westwood Library                         *
;*                                                                         *
;*                    File Name : PAL.ASM                                  *
;*                                                                         *
;*                   Programmer : Joe L. Bostic                            *
;*                                                                         *
;*                   Start Date : May 30, 1992                             *
;*                                                                         *
;*                  Last Update : April 27, 1994   [BR]                    *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   Set_Palette_Range -- Sets changed values in the palette.              *
;*   Bump_Color -- adjusts specified color in specified palette            *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
;********************** Model & Processor Directives ************************
IDEAL
P386
MODEL USE32 FLAT


;include "keyboard.inc"
FALSE = 0
TRUE  = 1

;****************************** Declarations ********************************
GLOBAL 		C Set_Palette_Range:NEAR
GLOBAL 		C Bump_Color:NEAR
GLOBAL  	C CurrentPalette:BYTE:768
GLOBAL		C PaletteTable:byte:1024


;********************************** Data ************************************
LOCALS ??

	DATASEG

CurrentPalette	DB	768 DUP(255)	; copy of current values of DAC regs
PaletteTable	DB	1024 DUP(0)

IFNDEF LIB_EXTERNS_RESOLVED
VertBlank	DW	0		; !!!! this should go away
ENDIF


;********************************** Code ************************************
	CODESEG
*/

extern "C" unsigned char CurrentPalette[768] = {255};	//	DB	768 DUP(255)	; copy of current values of DAC regs
extern "C" unsigned char PaletteTable[1024] = {0};		//	DB	1024 DUP(0)


/*
;***************************************************************************
;* SET_PALETTE_RANGE -- Sets a palette range to the new pal                *
;*                                                                         *
;* INPUT:                                                                  *
;*                                                                         *
;* OUTPUT:                                                                 *
;*                                                                         *
;* PROTO:                                                                  *
;*                                                                         *
;* WARNINGS:	This routine is optimized for changing a small number of   *
;*		colors in the palette.
;*                                                                         *
;* HISTORY:                                                                *
;*   03/07/1995 PWG : Created.                                             *
;*=========================================================================*
*/
void __cdecl Set_Palette_Range(void *palette)
{
	memcpy(CurrentPalette, palette, 768);
	Set_DD_Palette(palette);

	/*
	PROC	Set_Palette_Range C NEAR
	ARG	palette:DWORD

	GLOBAL	Set_DD_Palette_:near
	GLOBAL	Wait_Vert_Blank_:near
	
	pushad
	mov	esi,[palette]
	mov	ecx,768/4
	mov	edi,offset CurrentPalette
	cld
	rep	movsd
	;call	Wait_Vert_Blank_
	mov	eax,[palette]
	push	eax
	call	Set_DD_Palette_
	pop	eax
	popad
	ret
	*/
}











/*
;***************************************************************************
;**     C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S       **
;***************************************************************************
;*                                                                         *
;*                 Project Name : GraphicViewPortClass			   *
;*                                                                         *
;*                    File Name : PUTPIXEL.ASM                             *
;*                                                                         *
;*                   Programmer : Phil Gorrow				   *
;*                                                                         *
;*                   Start Date : June 7, 1994				   *
;*                                                                         *
;*                  Last Update : June 8, 1994   [PWG]                     *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   VVPC::Put_Pixel -- Puts a pixel on a virtual viewport                 *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *

IDEAL
P386
MODEL USE32 FLAT

INCLUDE ".\drawbuff.inc"
INCLUDE ".\gbuffer.inc"


CODESEG
*/

/*
;***************************************************************************
;* VVPC::PUT_PIXEL -- Puts a pixel on a virtual viewport                   *
;*                                                                         *
;* INPUT:	WORD the x position for the pixel relative to the upper    *
;*			left corner of the viewport			   *
;*		WORD the y pos for the pixel relative to the upper left	   *
;*			corner of the viewport				   *
;*		UBYTE the color of the pixel to write			   *
;*                                                                         *
;* OUTPUT:      none                                                       *
;*                                                                         *
;* WARNING:	If pixel is to be placed outside of the viewport then	   *
;*		this routine will abort.				   *
;*									   *
;* HISTORY:                                                                *
;*   06/08/1994 PWG : Created.                                             *
;*=========================================================================*
	PROC	Buffer_Put_Pixel C near
	USES	eax,ebx,ecx,edx,edi
*/

void __cdecl Buffer_Put_Pixel(void * this_object, int x_pixel, int y_pixel, unsigned char color)
{
			  	
}








/*
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Support Library                          *
;*                                                                         *
;*                    File Name : cliprect.asm                             *
;*                                                                         *
;*                   Programmer : Julio R Jerez                            *
;*                                                                         *
;*                   Start Date : Mar, 2 1995                              *
;*                                                                         *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;* int Clip_Rect ( int * x , int * y , int * dw , int * dh , 		   *
;*	       	   int width , int height ) ;          			   *
;* int Confine_Rect ( int * x , int * y , int * dw , int * dh , 	   *
;*	       	      int width , int height ) ;          		   *
;*									   *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *


IDEAL
P386
MODEL USE32 FLAT

GLOBAL	 C Clip_Rect	:NEAR
GLOBAL	 C Confine_Rect	:NEAR

CODESEG

;***************************************************************************
;* Clip_Rect -- clip a given rectangle against a given window		   *
;*                                                                         *
;* INPUT:   &x , &y , &w , &h  -> Pointer to rectangle being clipped       *
;*          width , height     -> dimension of clipping window             *
;*                                                                         *
;* OUTPUT: a) Zero if the rectangle is totally contained by the 	   *
;*	      clipping window.						   *
;*	   b) A negative value if the rectangle is totally outside the     *
;*            the clipping window					   *
;*	   c) A positive value if the rectangle	was clipped against the	   *
;*	      clipping window, also the values pointed by x, y, w, h will  *
;*	      be modified to new clipped values	 			   *
;*									   *
;*   05/03/1995 JRJ : added comment                                        *
;*=========================================================================*
; int Clip_Rect (int* x, int* y, int* dw, int* dh, int width, int height);          			   *
*/

extern "C" int __cdecl Clip_Rect ( int * x , int * y , int * w , int * h , int clip_w, int clip_h)
{		
	int xstart = *x;
	int ystart = *y;
	int yend = *y + *h - 1;
	int xend = *x + *w - 1;
	int result = 0;

	// If we aren't drawing within bounds, return -1
	if (xstart >= clip_w || ystart >= clip_h || xend < 0 || yend < 0) {
		return -1;
	}

	if (xstart < 0) {
		xstart = 0;
		result = 1;
	}

	if (ystart < 0) {
		ystart = 0;
		result = 1;
	}

	if (xend >= clip_w - 1) {
		xend = clip_w - 1;
		result = 1;
	}

	if (yend >= clip_h - 1) {
		yend = clip_h - 1;
		result = 1;
	}

	*x = xstart;
	*y = ystart;
	*w = xend - xstart + 1;
	*h = yend - ystart + 1;

	return result;
}

/*
;***************************************************************************
;* Confine_Rect -- clip a given rectangle against a given window	   *
;*                                                                         *
;* INPUT:   &x,&y,w,h    -> Pointer to rectangle being clipped       *
;*          width,height     -> dimension of clipping window             *
;*                                                                         *
;* OUTPUT: a) Zero if the rectangle is totally contained by the 	   *
;*	      clipping window.						   *
;*	   c) A positive value if the rectangle	was shifted in position    *
;*	      to fix inside the clipping window, also the values pointed   *
;*	      by x, y, will adjusted to a new values	 		   *
;*									   *
;*  NOTE:  this function make not attempt to verify if the rectangle is	   *
;*	   bigger than the clipping window and at the same time wrap around*
;*	   it. If that is the case the result is meaningless		   *
;*=========================================================================*
; int Confine_Rect (int* x, int* y, int dw, int dh, int width, int height);          			   *
*/

extern "C" int __cdecl Confine_Rect ( int * x_pos , int * y_pos , int x , int y , int w , int h )
{
	BOOL confined = false;

	if ((-*x_pos & (*x_pos + x - w - 1)) >= 0) {
		if (*x_pos > 0) {
			*x_pos -= *x_pos + x - w;
		}
		else {
			*x_pos = 0;
		}

		confined = true;
	}

	if ((-*y_pos & (*y_pos + y - h - 1)) >= 0) {
		if (*y_pos > 0) {
			*y_pos -= *y_pos + y - h;
		}
		else {
			*y_pos = 0;
		}

		confined = true;
	}

	return confined;
}

/*
;***************************************************************************
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Westwood 32 bit Library                  *
;*                                                                         *
;*                    File Name : TOPAGE.ASM                               *
;*                                                                         *
;*                   Programmer : Phil W. Gorrow                           *
;*                                                                         *
;*                   Start Date : June 8, 1994                             *
;*                                                                         *
;*                  Last Update : June 15, 1994   [PWG]                    *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   Buffer_To_Page -- Copies a linear buffer to a virtual viewport	   *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *

IDEAL
P386
MODEL USE32 FLAT

TRANSP	equ  0


INCLUDE ".\drawbuff.inc"
INCLUDE ".\gbuffer.inc"

CODESEG

;***************************************************************************
;* VVC::TOPAGE -- Copies a linear buffer to a virtual viewport		   *
;*                                                                         *
;* INPUT:	WORD	x_pixel		- x pixel on viewport to copy from *
;*		WORD	y_pixel 	- y pixel on viewport to copy from *
;*		WORD	pixel_width	- the width of copy region	   *
;*		WORD	pixel_height	- the height of copy region	   *
;*		BYTE *	src		- buffer to copy from		   *
;*		VVPC *  dest		- virtual viewport to copy to	   *
;*                                                                         *
;* OUTPUT:      none                                                       *
;*                                                                         *
;* WARNINGS:    Coordinates and dimensions will be adjusted if they exceed *
;*	        the boundaries.  In the event that no adjustment is 	   *
;*	        possible this routine will abort.  If the size of the 	   *
;*		region to copy exceeds the size passed in for the buffer   *
;*		the routine will automatically abort.			   *
;*									   *
;* HISTORY:                                                                *
;*   06/15/1994 PWG : Created.                                             *
;*=========================================================================*
 */ 

extern "C" long __cdecl Buffer_To_Page(int x, int y, int w, int h, void *buffer, void *dest)
{
	GraphicViewPortClass& vp = *(GraphicViewPortClass*)dest;
	// from chronoshift
	int xstart = x;
	int ystart = y;
	int xend = x + w - 1;
	int yend = y + h - 1;

	int xoffset = 0;
	int yoffset = 0;

	// If we aren't drawing within the viewport, return
	if (!buffer || xstart >= vp.Get_Width() || ystart >= vp.Get_Height() || xend < 0 || yend < 0) {
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

	xend = min(xend, vp.Get_Width() - 1);
	yend = min(yend, vp.Get_Height() - 1);

	int pitch = vp.Get_Pitch() + vp.Get_Width() + vp.Get_XAdd();
	uint8_t* dst = (y * 4) * pitch + (x * 4) + (uint8_t*)(vp.Get_Offset());
	uint8_t* src = xoffset + w * yoffset + static_cast<uint8_t*>(buffer);
	// int dst_pitch = x_pos + pitch - xend;
	// int src_pitch = x_pos + width - xend;
	int lines = yend - ystart + 1;
	int blit_width = xend - xstart + 1;

	// blit
	while (lines--) {
		FastScanlinePaletteBlit(dst, src, blit_width);
		src += w;
		dst += pitch * 4;
	}

	return 0;
}

			//ENDP	Buffer_To_Page
		//END









/*

;***************************************************************************
;* VVPC::GET_PIXEL -- Gets a pixel from the current view port		   *
;*                                                                         *
;* INPUT:	WORD the x pixel on the screen.				   *
;*		WORD the y pixel on the screen.				   *
;*                                                                         *
;* OUTPUT:      UBYTE the pixel at the specified location		   *
;*                                                                         *
;* WARNING:	If pixel is to be placed outside of the viewport then	   *
;*		this routine will abort.				   *
;*                                                                         *
;* HISTORY:                                                                *
;*   06/07/1994 PWG : Created.                                             *
;*=========================================================================*
	PROC	Buffer_Get_Pixel C near
	USES	ebx,ecx,edx,edi

	ARG    	this_object:DWORD				; this is a member function
	ARG	x_pixel:DWORD				; x position of pixel to set
	ARG	y_pixel:DWORD				; y position of pixel to set
*/

extern "C" int __cdecl Buffer_Get_Pixel(void * this_object, int x, int y)
{
	GraphicViewPortClass& vp = *((GraphicViewPortClass *)this_object);
	if (x < (unsigned)vp.Get_Width() && y < (unsigned)vp.Get_Height()) {
		return *((uint8_t*)(vp.Get_Offset()) + x + y * vp.Get_Full_Pitch());
	}
}


