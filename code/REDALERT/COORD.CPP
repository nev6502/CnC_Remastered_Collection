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

/* $Header: /CounterStrike/COORD.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : COORD.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : July 22, 1996 [JLB]                                          *
 *                                                                                             *
 * Support code to handle the coordinate system is located in this module.                     *
 * Routines here will be called QUITE frequently during play and must be                       *
 * as efficient as possible.                                                                   *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Cardinal_To_Fixed -- Converts cardinal numbers into a fixed point number.                 *
 *   Coord_Cell -- Convert a coordinate into a cell number.                                    *
 *   Coord_Move -- Moves a coordinate an arbitrary direction for an arbitrary distance         *
 *   Coord_Scatter -- Determines a random coordinate from an anchor point.                     *
 *   Coord_Spillage_List -- Calculate a spillage list for the dirty rectangle specified.       *
 *   Coord_Spillage_List -- Determines the offset list for cell spillage/occupation.           *
 *   Distance -- Determines the cell distance between two cells.                               *
 *   Distance -- Determines the lepton distance between two coordinates.                       *
 *   Distance -- Fetch distance between two target values.                                     *
 *   Fixed_To_Cardinal -- Converts a fixed point number into a cardinal number.                *
  * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

// Cosine lookup table
const __int16 cosTable[255] = { 
	0x0000, 0x0327, 0x064E, 0x0973, 0x0C98, 0x0FBA, 0x12DA, 0x15F7, 0x1911, 0x1C26, 0x1F38, 0x2244, 0x254B,	0x284D, 0x2B48, 
	0x2E3C, 0x3129, 0x340F, 0x36EC, 0x39C1, 0x3C8D, 0x3F50, 0x4209, 0x44B7, 0x475B, 0x49F3, 0x4C81, 0x4F02,	0x5177, 0x53DF,
	0x563A,	0x5888, 0x5AC9, 0x5CFB, 0x5F1E, 0x6133, 0x6339, 0x652F, 0x6715, 0x68EC, 0x6AB2, 0x6C68, 0x6E0D,	0x6FA0, 0x7123, 
	0x7293, 0x73F2, 0x753F, 0x767A, 0x77A2, 0x78B8, 0x79BB, 0x7AAB, 0x7B88, 0x7C51, 0x7D08, 0x7DAB, 0x7E3A,	0x7EB6, 0x7F1E,
	0x7F73, 0x7FB3, 0x7FE0, 0x7FF9, 0x7FFE, 0x7FEF, 0x7FCC, 0x7F95, 0x7F4B, 0x7EED, 0x7E7B, 0x7DF5, 0x7D5C,	0x7CAF, 0x7BEF, 
	0x7B1C, 0x7A35, 0x793C, 0x782F, 0x7710, 0x75DF, 0x749B, 0x7345, 0x71DD, 0x7064, 0x6ED9, 0x6D3C, 0x6B8F,	0x69D1, 0x6803, 
	0x6624, 0x6436, 0x6238, 0x602A, 0x5E0E, 0x5BE3, 0x59AA, 0x5763, 0x550E, 0x52AD, 0x503E, 0x4DC3, 0x4B3B,	0x48A8, 0x460A, 
	0x4361, 0x40AD, 0x3DF0, 0x3B29, 0x3858, 0x357F, 0x329D, 0x2FB4, 0x2CC3, 0x29CB, 0x26CD, 0x23C9, 0x20BF,	0x1DB0, 0x1A9C, 
	0x1784, 0x1469, 0x114A, 0x0E29, 0x0B06, 0x07E1, 0x04BA, 0x0193, 0xFE6D, 0xFB46, 0xF81F, 0xF4FA, 0xF1D7,	0xEEB6, 0xEB97, 
	0xE87C, 0xE564, 0xE250, 0xDF41, 0xDC37, 0xD933, 0xD635, 0xD33D, 0xD04C, 0xCD63, 0xCA81, 0xC7A8, 0xC4D7,	0xC210, 0xBF53, 
	0xBC9F, 0xB9F6, 0xB758, 0xB4C5, 0xB23D, 0xAFC2, 0xAD53, 0xAAF2, 0xA89D, 0xA656, 0xA41D, 0xA1F2, 0x9FD6,	0x9DC8, 0x9BCA, 
	0x99DC, 0x97FD, 0x962F, 0x9471, 0x92C4, 0x9127, 0x8F9C, 0x8E23, 0x8CBB, 0x8B65, 0x8A21, 0x88F0, 0x87D1,	0x86C4, 0x85CB, 
	0x84E4, 0x8411, 0x8351, 0x82A4, 0x820B, 0x8185, 0x8113, 0x80B5, 0x806B, 0x8034, 0x8011, 0x8002, 0x8007,	0x8020, 0x804D, 
	0x808D, 0x80E2, 0x814A, 0x81C6, 0x8255, 0x82F8, 0x83AF, 0x8478, 0x8555, 0x8645, 0x8748, 0x885E, 0x8986,	0x8AC1, 0x8C0E, 
	0x8D6D, 0x8EDD, 0x9060, 0x91F3, 0x9398, 0x954E, 0x9714, 0x98EB, 0x9AD1, 0x9CC7, 0x9ECD, 0xA0E2, 0xA305,	0xA537, 0xA778, 
	0xA9C6, 0xAC21, 0xAE89, 0xB0FE, 0xB37F, 0xB60D, 0xB8A5, 0xBB49, 0xBDF7, 0xC0B0, 0xC373, 0xC63F, 0xC914,	0xCBF1, 0xCED7, 
	0xD1C4, 0xD4B8, 0xD7B3, 0xDAB5, 0xDDBC, 0xE0C8, 0xE3DA, 0xE6EF, 0xEA09, 0xED26, 0xF046, 0xF368, 0xF68D,	0xF9B2, 0xFCD9,
};

// Sine lookup table
const __int16 sinTable[255] = {
	0x7FFF, 0x7FF5, 0x7FD7, 0x7FA5, 0x7F5F, 0x7F06, 0x7E99, 0x7E18, 0x7D84, 0x7CDC, 0x7C21, 0x7B52, 0x7A71, 0x797C, 0x7874,
	0x775A, 0x762D, 0x74EE, 0x739C, 0x7239, 0x70C4, 0x6F3D, 0x6DA5, 0x6BFC, 0x6A42, 0x6878, 0x669D, 0x64B3, 0x62B9, 0x60AF,
	0x5E97, 0x5C6F, 0x5A3A, 0x57F6, 0x55A5, 0x5346, 0x50DB, 0x4E63, 0x4BDE, 0x494E, 0x46B3, 0x440C, 0x415B, 0x3EA0, 0x3BDB,
	0x390D, 0x3636, 0x3356, 0x306F, 0x2D80, 0x2A8A, 0x278D, 0x248A, 0x2182, 0x1E74, 0x1B61, 0x184B, 0x1530, 0x1212, 0x0EF2,
	0x0BCF, 0x08AA, 0x0584, 0x025D, 0xFF37, 0xFC0F, 0xF8E9, 0xF5C3, 0xF29F, 0xEF7E, 0xEC5E, 0xE942, 0xE629, 0xE315, 0xE005,
	0xDCF9, 0xD9F4, 0xD6F4, 0xD3FA, 0xD108, 0xCE1C, 0xCB39, 0xC85D, 0xC58B, 0xC2C1, 0xC001, 0xBD4B, 0xBA9F, 0xB7FE, 0xB568,
	0xB2DE, 0xB060, 0xADEE, 0xAB89, 0xA931, 0xA6E6, 0xA4AA, 0xA27B, 0xA05B, 0x9E4A, 0x9C48, 0x9A56, 0x9873, 0x96A1, 0x94DF,
	0x932D, 0x918D, 0x8FFE, 0x8E80, 0x8D13, 0x8BB9, 0x8A70, 0x893A, 0x8817, 0x8706, 0x8607, 0x851C, 0x8444, 0x837F, 0x82CE,
	0x822F, 0x81A5, 0x812E, 0x80CB, 0x807B, 0x8040, 0x8018, 0x8004, 0x8004, 0x8018, 0x8040, 0x807B, 0x80CB, 0x812E, 0x81A5,
	0x822F, 0x82CE, 0x837F, 0x8444, 0x851C, 0x8607, 0x8706, 0x8817, 0x893A, 0x8A70, 0x8BB9, 0x8D13, 0x8E80, 0x8FFE, 0x918D,
	0x932D, 0x94DF, 0x96A1, 0x9873, 0x9A56, 0x9C48, 0x9E4A, 0xA05B, 0xA27B, 0xA4AA, 0xA6E6, 0xA931, 0xAB89, 0xADEE, 0xB060,
	0xB2DE, 0xB568, 0xB7FE, 0xBA9F, 0xBD4B, 0xC001, 0xC2C1, 0xC58B, 0xC85D, 0xCB39, 0xCE1C, 0xD108, 0xD3FA, 0xD6F4, 0xD9F4,
	0xDCF9, 0xE005, 0xE315, 0xE629, 0xE942, 0xEC5E, 0xEF7E, 0xF29F, 0xF5C3, 0xF8E9, 0xFC0F, 0xFF37, 0x025D, 0x0584, 0x08AA,
	0x0BCF, 0x0EF2, 0x1212, 0x1530, 0x184B, 0x1B61, 0x1E74, 0x2182, 0x248A, 0x278D, 0x2A8A, 0x2D80, 0x306F, 0x3356, 0x3636,
	0x390D, 0x3BDB, 0x3EA0, 0x415B, 0x440C, 0x46B3, 0x494E, 0x4BDE, 0x4E63, 0x50DB, 0x5346, 0x55A5, 0x57F6, 0x5A3A, 0x5C6F,
	0x5E97, 0x60AF, 0x62B9, 0x64B3, 0x669D, 0x6878, 0x6A42, 0x6BFC, 0x6DA5, 0x6F3D, 0x70C4, 0x7239, 0x739C, 0x74EE, 0x762D,
	0x775A, 0x7874, 0x797C, 0x7A71, 0x7B52, 0x7C21, 0x7CDC, 0x7D84, 0x7E18, 0x7E99, 0x7F06, 0x7F5F, 0x7FA5, 0x7FD7, 0x7FF5,
};

/***********************************************************************************************
 * Coord_Cell -- Convert a coordinate into a cell number.                                      *
 *                                                                                             *
 *    This routine will convert the specified coordinate value into a cell number. This is     *
 *    useful to determine the map index number into the cell array that corresponds to a       *
 *    particular coordinate.                                                                   *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to convert into a cell number.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the cell number that corresponds to the coordinate specified.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
CELL Coord_Cell(COORDINATE coord)
{
	CELL_COMPOSITE cell;
	cell.Cell = 0;
	cell.Sub.X = ((COORD_COMPOSITE &)coord).Sub.X.Sub.Cell;
	cell.Sub.Y = ((COORD_COMPOSITE &)coord).Sub.Y.Sub.Cell;
	return(cell.Cell);
//	return(XY_Cell(((COORD_COMPOSITE)coord).Sub.X, ((COORD_COMPOSITE)composite).Sub.Y));
}


/***********************************************************************************************
 * Distance -- Fetch distance between two target values.                                       *
 *                                                                                             *
 *    This routine will determine the lepton distance between the two specified target         *
 *    values.                                                                                  *
 *                                                                                             *
 * INPUT:   target1  -- First target value.                                                    *
 *                                                                                             *
 *          target2  -- Second target value.                                                   *
 *                                                                                             *
 * OUTPUT:  Returns with the lepton distance between the two target values.                    *
 *                                                                                             *
 * WARNINGS:   Be sure that the targets are legal before calling this routine. Otherwise, the  *
 *             return value is meaningless.                                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int Distance(TARGET target1, TARGET target2)
{
	return(Distance(As_Coord(target1), As_Coord(target2)));
}


/***********************************************************************************************
 * Distance -- Determines the lepton distance between two coordinates.                         *
 *                                                                                             *
 *    This routine is used to determine the distance between two coordinates. It uses the      *
 *    Dragon Strike method of distance determination and thus it is very fast.                 *
 *                                                                                             *
 * INPUT:   coord1   -- First coordinate.                                                      *
 *                                                                                             *
 *          coord2   -- Second coordinate.                                                     *
 *                                                                                             *
 * OUTPUT:  Returns the lepton distance between the two coordinates.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int Distance(COORDINATE coord1, COORDINATE coord2)
{
	int	diff1, diff2;

	diff1 = Coord_Y(coord1) - Coord_Y(coord2);
	if (diff1 < 0) diff1 = -diff1;
	diff2 = Coord_X(coord1) - Coord_X(coord2);
	if (diff2 < 0) diff2 = -diff2;
	if (diff1 > diff2) {
		return(diff1 + ((unsigned)diff2 / 2));
	}
	return(diff2 + ((unsigned)diff1 / 2));
}


/***********************************************************************************************
 * Coord_Spillage_List -- Determines the offset list for cell spillage/occupation.             *
 *                                                                                             *
 *    This routine will take an arbitrary position and object size and return with a list of   *
 *    cell offsets from the current cell for all cells that are overlapped by the object. The  *
 *    first cell offset is always zero, so to just get the adjacent spill cell list, add one   *
 *    to the return pointer.                                                                   *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to examine.                                                *
 *                                                                                             *
 *          maxsize -- The maximum width/height of the object (pixels).                        *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a spillage list.                                         *
 *                                                                                             *
 * WARNINGS:   The algorithm is limited to working with a maxsize of 48 or less. Larger values *
 *             will generate an incomplete overlap list.                                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/06/1993 JLB : Created.                                                                 *
 *   03/25/1994 JLB : Added width optimization.                                                *
 *   04/29/1994 JLB : Converted to C.                                                          *
 *   06/03/1994 JLB : Converted to general purpose spillage functionality.                     *
 *   01/07/1995 JLB : Manually calculates spillage list for large objects.                     *
 *=============================================================================================*/
short const * Coord_Spillage_List(COORDINATE coord, int maxsize)
{
	static short const _MoveSpillage[(int)FACING_COUNT+1][5] = {
		{0, -MAP_CELL_W, REFRESH_EOL, 0, 0},						// N
		{0, -MAP_CELL_W, 1, -(MAP_CELL_W-1), REFRESH_EOL},		// NE
		{0, 1, REFRESH_EOL, 0, 0},										// E
		{0, 1, MAP_CELL_W, MAP_CELL_W+1, REFRESH_EOL},			// SE
		{0, MAP_CELL_W, REFRESH_EOL, 0, 0},							// S
		{0, -1, MAP_CELL_W, MAP_CELL_W-1, REFRESH_EOL},			// SW
		{0, -1, REFRESH_EOL, 0, 0},									// W
		{0, -1, -MAP_CELL_W, -(MAP_CELL_W+1), REFRESH_EOL},	// NW
		{0, REFRESH_EOL, 0, 0, 0}										// non-moving.
	};
	static short _manual[10];
//;	00 = on axis
//;	01 = below axis
//;	10 = above axis
//;	11 = undefined
	static signed char const _SpillTable[16]	= {8,6,2,-1,0,7,1,-1,4,5,3,-1,-1,-1,-1,-1};
	int	index=0;
	int	x,y;

	/*
	**	For mondo-enourmo-gigundo objects, use a prebuilt mammoth table
	**	that covers a 5x5 square region.
	*/
	if (maxsize > ICON_PIXEL_W * 2) {
		static short const _gigundo[] = {
			-((2*MAP_CELL_W)-2),-((2*MAP_CELL_W)-1),-((2*MAP_CELL_W)),-((2*MAP_CELL_W)+1),-((2*MAP_CELL_W)+2),
			-((1*MAP_CELL_W)-2),-((1*MAP_CELL_W)-1),-((1*MAP_CELL_W)),-((1*MAP_CELL_W)+1),-((1*MAP_CELL_W)+2),
			-((0*MAP_CELL_W)-2),-((0*MAP_CELL_W)-1),-((0*MAP_CELL_W)),-((0*MAP_CELL_W)+1),-((0*MAP_CELL_W)+2),
			((1*MAP_CELL_W)-2),((1*MAP_CELL_W)-1),((1*MAP_CELL_W)),((1*MAP_CELL_W)+1),((1*MAP_CELL_W)+2),
			+((2*MAP_CELL_W)-2),+((2*MAP_CELL_W)-1),+((2*MAP_CELL_W)),+((2*MAP_CELL_W)+1),+((2*MAP_CELL_W)+2),
			REFRESH_EOL
		};
		return(&_gigundo[0]);
	}

	/*
	**	For very large objects, build the overlap list by hand. This is time consuming, but
	**	not nearly as time consuming as drawing even a single cell unnecessarily.
	*/
	if (maxsize > ICON_PIXEL_W) {
		maxsize = min(maxsize, (ICON_PIXEL_W*2))/2;

		x = (ICON_PIXEL_W * Coord_XLepton(coord)) / ICON_LEPTON_W;
		y = (ICON_PIXEL_H * Coord_YLepton(coord)) / ICON_LEPTON_H;
		int left = x-maxsize;
		int right = x+maxsize;
		int top = y-maxsize;
		int bottom = y+maxsize;

		_manual[index++] = 0;
		if (left < 0) _manual[index++] = -1;
		if (right >= ICON_PIXEL_W) _manual[index++] = 1;
		if (top < 0) _manual[index++] = -MAP_CELL_W;
		if (bottom >= ICON_PIXEL_H) _manual[index++] = MAP_CELL_W;
		if (left < 0 && top < 0) _manual[index++] = -(MAP_CELL_W+1);
		if (right >= ICON_PIXEL_W && bottom >= ICON_PIXEL_H) _manual[index++] = MAP_CELL_W+1;
		if (left < 0 && bottom >= ICON_PIXEL_H) _manual[index++] = MAP_CELL_W-1;
		if (right >= ICON_PIXEL_H && top < 0) _manual[index++] = -(MAP_CELL_W-1);
		_manual[index] = REFRESH_EOL;
		return(&_manual[0]);
	}

	/*
	**	Determine the number of leptons "leeway" allowed this unit.
	*/
	int posval = Pixel2Lepton[(ICON_PIXEL_W-maxsize)/2];

	x = Coord_XLepton(coord) - 0x0080;
	y = Coord_YLepton(coord) - 0x0080;
	if (y > posval) index |= 0x08;			// Spilling South.
	if (y < -posval) index |= 0x04;			// Spilling North.
	if (x > posval) index |= 0x02;			// Spilling East.
	if (x < -posval) index |= 0x01;			// Spilling West.

	return(&_MoveSpillage[_SpillTable[index]][0]);
}


/***********************************************************************************************
 * Coord_Spillage_List -- Calculate a spillage list for the dirty rectangle specified.         *
 *                                                                                             *
 *    Given a center coordinate and a dirty rectangle, calcuate a cell offset list for         *
 *    determining such things as overlap and redraw logic. Optionally, the center cell         *
 *    location will not be part of the list.                                                   *
 *                                                                                             *
 * INPUT:   coord -- The center coordinate that the dirty rectangle is based off of.           *
 *                                                                                             *
 *          rect  -- Reference to the dirty rectangle.                                         *
 *                                                                                             *
 *          nocenter -- If true, then the center cell offset will not be part of the spillage  *
 *                      list returned. This is handy when the center cell is known to be       *
 *                      processed by some other method and it can be safely and efficiently    *
 *                      ignored by the list generated.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the spillage list that corresponds to the data           *
 *          specified. This is a pointer to a static buffer and as such it will only be valid  *
 *          until the next time that this routine is called.                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/22/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * Coord_Spillage_List(COORDINATE coord, Rect const & rect, bool nocenter)
{
	if (!rect.Is_Valid()) {
		static short const _list[] = {REFRESH_EOL};
		return(_list);
	}

	CELL coordcell = Coord_Cell(coord);
	LEPTON x = Coord_X(coord);
	LEPTON y = Coord_Y(coord);

	/*
	**	Add the rectangle values to the coordinate in order to normalize the start and end
	**	corners of the rectangle. The values are now absolute to the real game world rather
	**	than relative to the coordinate.
	*/
	LEPTON_COMPOSITE startx;
	LEPTON_COMPOSITE starty;
	LEPTON_COMPOSITE endx;
	LEPTON_COMPOSITE endy;
	startx.Raw = (int)x + (short)Pixel_To_Lepton(rect.X);
	starty.Raw = (int)y + (short)Pixel_To_Lepton(rect.Y);
	endx.Raw = startx.Raw + Pixel_To_Lepton(rect.Width-1);
	endy.Raw = starty.Raw + Pixel_To_Lepton(rect.Height-1);

	/*
	**	Determine the upper left and lower right cell indexes. This is a simple conversion from
	**	their lepton counterpart. These cells values are used to form the bounding box for the
	**	map offset list.
	*/
	int cellx = startx.Sub.Cell;
	int cellx2 = endx.Sub.Cell;
	int celly = starty.Sub.Cell;
	int celly2 = endy.Sub.Cell;

	/*
	**	Generate the spillage list by counting off the rows and colums of the cells
	**	that are affected. This is easy since the upper left and lower right corner cells
	**	are known.
	*/
	int count = 0;
	static short _spillagelist[128];
	short * ptr = _spillagelist;
	for (int yy = celly; yy <= celly2; yy++) {
		for (int xx = cellx; xx <= cellx2; xx++) {
			short offset = (XY_Cell(xx, yy) - coordcell);
			if (!nocenter || offset != 0) {
				*ptr++ = offset;
				count++;
				if (count+2 >= ARRAY_SIZE(_spillagelist)) break;
			}
		}
		if (count+2 >= ARRAY_SIZE(_spillagelist)) break;
	}

	/*
	**	Cap the list with the end of list marker and then return a pointer
	**	to the completed list.
	*/
	*ptr = REFRESH_EOL;
	return(_spillagelist);
}


/***********************************************************************************************
 * Coord_Move -- Moves a coordinate an arbitrary direction for an arbitrary distance           *
 *                                                                                             *
 *    This function will move a coordinate in a using SIN and COS arithmetic.                  *
 *                                                                                             *
 * INPUT:   start    -- The starting coordinate.                                               *
 *                                                                                             *
 *          dir      -- The direction to move the coordinate.                                  *
 *                                                                                             *
 *          distance -- The distance to move the coordinate position (in leptons).             *
 *                                                                                             *
 * OUTPUT:  Returns the new coordinate position.                                               *
 *                                                                                             *
 * WARNINGS:   This routine uses multiplies -- use with caution.                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE Coord_Move(COORDINATE start, register DirType dir, unsigned short distance)
{
	Move_Point(*(short *)&start, *(((short *)&start)+1), dir, distance);
	return(start);
}


/***********************************************************************************************
 * Coord_Scatter -- Determines a random coordinate from an anchor point.                       *
 *                                                                                             *
 *    This routine will perform a scatter algorithm on the specified                           *
 *    anchor point in order to return with another coordinate that is                          *
 *    randomly nearby the original. Typical use of this would be for                           *
 *    missile targeting.                                                                       *
 *                                                                                             *
 * INPUT:   coord    -- This is the anchor coordinate.                                         *
 *                                                                                             *
 *          distance -- This is the distance in pixels that the scatter                        *
 *                      should fall within.                                                    *
 *                                                                                             *
 *          lock     -- bool; Convert the new coordinate into a center                         *
 *                      cell based coordinate?                                                 *
 *                                                                                             *
 * OUTPUT:  Returns with a new coordinate that is nearby the original.                         *
 *                                                                                             *
 * WARNINGS:   Maximum pixel scatter distance is 255.                                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/01/1992 JLB : Created.                                                                 *
 *   05/13/1992 JLB : Only uses Random().                                                      *
 *=============================================================================================*/
COORDINATE Coord_Scatter(COORDINATE coord, unsigned distance, bool lock)
{
	COORDINATE	newcoord;

	newcoord = Coord_Move(coord, Random_Pick(DIR_N, DIR_MAX), distance);

	if (newcoord & HIGH_COORD_MASK) newcoord = coord;

	if (lock) {
		newcoord = Coord_Snap(newcoord);
	}

	return(newcoord);
}

void Move_Point(short& x, short& y, register DirType dir, unsigned short distance)
{
	x += ( cosTable[dir] * distance ) / SHRT_MAX;
	y += -( sinTable[dir] * distance ) / SHRT_MAX;
}
