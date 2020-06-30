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

/* $Header: /CounterStrike/CDATA.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CDATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 16, 1994                                                 *
 *                                                                                             *
 *                  Last Update : July 6, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TemplateTypeClass::As_Reference -- Fetches a reference to the template specified.         *
 *   TemplateTypeClass::Create_And_Place -- Creates and places a template object on the map.   *
 *   TemplateTypeClass::Create_One_Of -- Creates an object of this template type.              *
 *   TemplateTypeClass::Display -- Displays a generic representation of template.              *
 *   TemplateTypeClass::From_Name -- Determine template from ASCII name.                       *
 *   TemplateTypeClass::Init -- Loads graphic data for templates.                              *
 *   TemplateTypeClass::Land_Type -- Determines land type from template and icon number.       *
 *   TemplateTypeClass::Occupy_List -- Determines occupation list.                             *
 *   TemplateTypeClass::One_Time -- Performs one-time initialization                           *
 *   TemplateTypeClass::Prep_For_Add -- Prepares to add template to scenario.                  *
 *   TemplateTypeClass::TemplateTypeClass -- Constructor for template type objects.            *
 *   TemplateTypeClass::operator delete -- Deletes a template type object.                     *
 *   TemplateTypeClass::operator new -- Allocates a template type from special heap.           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"image.h"

static TemplateTypeClass const Empty(
	TEMPLATE_CLEAR1,
	THEATERF_TEMPERATE|THEATERF_SNOW|THEATERF_INTERIOR,
	"CLEAR01",
	TXT_CLEAR
);
static TemplateTypeClass const Clear(
	TEMPLATE_CLEAR1,
	THEATERF_TEMPERATE|THEATERF_SNOW|THEATERF_INTERIOR,
	"CLEAR01",
	TXT_CLEAR
);
static TemplateTypeClass const Road01(
	TEMPLATE_ROAD01,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"CLEAR01",
	TXT_ROAD
);
static TemplateTypeClass const Road02(
	TEMPLATE_ROAD02,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC02",
	TXT_ROAD
);
static TemplateTypeClass const Road03(
	TEMPLATE_ROAD03,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC03",
	TXT_ROAD
);
static TemplateTypeClass const Road04(
	TEMPLATE_ROAD04,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC04",
	TXT_ROAD
);
static TemplateTypeClass const Road05(
	TEMPLATE_ROAD05,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC05",
	TXT_ROAD
);
static TemplateTypeClass const Road06(
	TEMPLATE_ROAD06,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC06",
	TXT_ROAD
);
static TemplateTypeClass const Road07(
	TEMPLATE_ROAD07,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC07",
	TXT_ROAD
);
static TemplateTypeClass const Road08(
	TEMPLATE_ROAD08,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC08",
	TXT_ROAD
);
static TemplateTypeClass const Road09(
	TEMPLATE_ROAD09,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC09",
	TXT_ROAD
);
static TemplateTypeClass const Road10(
	TEMPLATE_ROAD10,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC10",
	TXT_ROAD
);
static TemplateTypeClass const Road11(
	TEMPLATE_ROAD11,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC11",
	TXT_ROAD
);
static TemplateTypeClass const Road12(
	TEMPLATE_ROAD12,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC12",
	TXT_ROAD
);
static TemplateTypeClass const Road13(
	TEMPLATE_ROAD13,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC13",
	TXT_ROAD
);
static TemplateTypeClass const Road14(
	TEMPLATE_ROAD14,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC14",
	TXT_ROAD
);
static TemplateTypeClass const Road15(
	TEMPLATE_ROAD15,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC15",
	TXT_ROAD
);
static TemplateTypeClass const Road16(
	TEMPLATE_ROAD16,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC16",
	TXT_ROAD
);
static TemplateTypeClass const Road17(
	TEMPLATE_ROAD17,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC17",
	TXT_ROAD
);
static TemplateTypeClass const Road18(
	TEMPLATE_ROAD18,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC18",
	TXT_ROAD
);
static TemplateTypeClass const Road19(
	TEMPLATE_ROAD19,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC19",
	TXT_ROAD
);
static TemplateTypeClass const Road20(
	TEMPLATE_ROAD20,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC20",
	TXT_ROAD
);
static TemplateTypeClass const Road21(
	TEMPLATE_ROAD21,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC21",
	TXT_ROAD
);
static TemplateTypeClass const Road22(
	TEMPLATE_ROAD22,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC22",
	TXT_ROAD
);
static TemplateTypeClass const Road23(
	TEMPLATE_ROAD23,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC23",
	TXT_ROAD
);
static TemplateTypeClass const Road24(
	TEMPLATE_ROAD24,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC24",
	TXT_ROAD
);
static TemplateTypeClass const Road25(
	TEMPLATE_ROAD25,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC01",
	TXT_ROAD
);
static TemplateTypeClass const Road26(
	TEMPLATE_ROAD26,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC02",
	TXT_ROAD
);
static TemplateTypeClass const Road27(
	TEMPLATE_ROAD27,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC02",
	TXT_ROAD
);
static TemplateTypeClass const Road28(
	TEMPLATE_ROAD28,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC03",
	TXT_ROAD
);
static TemplateTypeClass const Road29(
	TEMPLATE_ROAD29,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC04",
	TXT_ROAD
);
static TemplateTypeClass const Road30(
	TEMPLATE_ROAD30,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC05",
	TXT_ROAD
);
static TemplateTypeClass const Road31(
	TEMPLATE_ROAD31,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC06",
	TXT_ROAD
);
static TemplateTypeClass const Road32(
	TEMPLATE_ROAD32,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC07",
	TXT_ROAD
);
static TemplateTypeClass const Road33(
	TEMPLATE_ROAD33,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC08",
	TXT_ROAD
);
static TemplateTypeClass const Road34(
	TEMPLATE_ROAD34,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC09",
	TXT_ROAD
);
static TemplateTypeClass const Road35(
	TEMPLATE_ROAD35,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC10",
	TXT_ROAD
);
static TemplateTypeClass const Road36(
	TEMPLATE_ROAD36,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC11",
	TXT_ROAD
);
static TemplateTypeClass const Road37(
	TEMPLATE_ROAD37,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC12",
	TXT_ROAD
);
static TemplateTypeClass const Road38(
	TEMPLATE_ROAD38,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC13",
	TXT_ROAD
);
static TemplateTypeClass const Road39(
	TEMPLATE_ROAD39,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC14",
	TXT_ROAD
);
static TemplateTypeClass const Road40(
	TEMPLATE_ROAD40,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC15",
	TXT_ROAD
);
static TemplateTypeClass const Road41(
	TEMPLATE_ROAD41,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC16",
	TXT_ROAD
);
static TemplateTypeClass const Road42(
	TEMPLATE_ROAD42,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC17",
	TXT_ROAD
);
static TemplateTypeClass const Road43(
	TEMPLATE_ROAD43,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC18",
	TXT_ROAD
);
static TemplateTypeClass const Road44(
	TEMPLATE_ROAD44,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC19",
	TXT_ROAD
);
static TemplateTypeClass const Road45(
	TEMPLATE_ROAD45,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC20",
	TXT_ROAD
);
static TemplateTypeClass const Water(
	TEMPLATE_WATER,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WATER01",
	TXT_WATER
);
static TemplateTypeClass const Water2(
	TEMPLATE_WATER2,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WATER02",
	TXT_WATER
);
static TemplateTypeClass const Shore01(
	TEMPLATE_SHORE01,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE01",
	TXT_SHORE
);
static TemplateTypeClass const Shore02(
	TEMPLATE_SHORE02,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE02",
	TXT_SHORE
);
static TemplateTypeClass const Shore03(
	TEMPLATE_SHORE03,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE03",
	TXT_SHORE
);
static TemplateTypeClass const Shore04(
	TEMPLATE_SHORE04,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE04",
	TXT_SHORE
);
static TemplateTypeClass const Shore05(
	TEMPLATE_SHORE05,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE05",
	TXT_SHORE
);
static TemplateTypeClass const Shore06(
	TEMPLATE_SHORE06,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE06",
	TXT_SHORE
);
static TemplateTypeClass const Shore07(
	TEMPLATE_SHORE07,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE07",
	TXT_SHORE
);
static TemplateTypeClass const Shore08(
	TEMPLATE_SHORE08,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE08",
	TXT_SHORE
);
static TemplateTypeClass const Shore09(
	TEMPLATE_SHORE09,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE09",
	TXT_SHORE
);
static TemplateTypeClass const Shore10(
	TEMPLATE_SHORE10,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE10",
	TXT_SHORE
);
static TemplateTypeClass const Shore11(
	TEMPLATE_SHORE11,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE11",
	TXT_SHORE
);
static TemplateTypeClass const Shore12(
	TEMPLATE_SHORE12,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE12",
	TXT_SHORE
);
static TemplateTypeClass const Shore13(
	TEMPLATE_SHORE13,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE13",
	TXT_SHORE
);
static TemplateTypeClass const Shore14(
	TEMPLATE_SHORE14,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE14",
	TXT_SHORE
);
static TemplateTypeClass const Shore15(
	TEMPLATE_SHORE15,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE15",
	TXT_SHORE
);
static TemplateTypeClass const Shore16(
	TEMPLATE_SHORE16,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE16",
	TXT_SHORE
);
static TemplateTypeClass const Shore17(
	TEMPLATE_SHORE17,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE17",
	TXT_SHORE
);
static TemplateTypeClass const Shore18(
	TEMPLATE_SHORE18,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE18",
	TXT_SHORE
);
static TemplateTypeClass const Shore19(
	TEMPLATE_SHORE19,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE19",
	TXT_SHORE
);
static TemplateTypeClass const Shore20(
	TEMPLATE_SHORE20,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE20",
	TXT_SHORE
);
static TemplateTypeClass const Shore21(
	TEMPLATE_SHORE21,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE21",
	TXT_SHORE
);
static TemplateTypeClass const Shore22(
	TEMPLATE_SHORE22,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE22",
	TXT_SHORE
);
static TemplateTypeClass const Shore23(
	TEMPLATE_SHORE23,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE23",
	TXT_SHORE
);
static TemplateTypeClass const Shore24(
	TEMPLATE_SHORE24,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE24",
	TXT_SHORE
);
static TemplateTypeClass const Shore25(
	TEMPLATE_SHORE25,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE25",
	TXT_SHORE
);
static TemplateTypeClass const Shore26(
	TEMPLATE_SHORE26,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE26",
	TXT_SHORE
);
static TemplateTypeClass const Shore27(
	TEMPLATE_SHORE27,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE27",
	TXT_SHORE
);
static TemplateTypeClass const Shore28(
	TEMPLATE_SHORE28,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE28",
	TXT_SHORE
);
static TemplateTypeClass const Shore29(
	TEMPLATE_SHORE29,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE29",
	TXT_SHORE
);
static TemplateTypeClass const Shore30(
	TEMPLATE_SHORE30,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE30",
	TXT_SHORE
);
static TemplateTypeClass const Shore31(
	TEMPLATE_SHORE31,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE31",
	TXT_SHORE
);
static TemplateTypeClass const Shore32(
	TEMPLATE_SHORE32,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE32",
	TXT_SHORE
);
static TemplateTypeClass const Shore33(
	TEMPLATE_SHORE33,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE33",
	TXT_SHORE
);
static TemplateTypeClass const Shore34(
	TEMPLATE_SHORE34,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE4",
	TXT_SHORE
);
static TemplateTypeClass const Shore35(
	TEMPLATE_SHORE35,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE35",
	TXT_SHORE
);
static TemplateTypeClass const Shore36(
	TEMPLATE_SHORE36,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE36",
	TXT_SHORE
);
static TemplateTypeClass const Shore37(
	TEMPLATE_SHORE37,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE37",
	TXT_SHORE
);
static TemplateTypeClass const Shore38(
	TEMPLATE_SHORE38,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE38",
	TXT_SHORE
);
static TemplateTypeClass const Shore39(
	TEMPLATE_SHORE39,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE39",
	TXT_SHORE
);
static TemplateTypeClass const Shore40(
	TEMPLATE_SHORE40,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE40",
	TXT_SHORE
);
static TemplateTypeClass const Shore41(
	TEMPLATE_SHORE41,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE41",
	TXT_SHORE
);
// jmarshall
static TemplateTypeClass const Bridge1x(
	TEMPLATE_BRIDGE1X,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge1(
	TEMPLATE_BRIDGE1,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge1h(
	TEMPLATE_BRIDGE1H,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge1d(
	TEMPLATE_BRIDGE1D,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2x(
	TEMPLATE_BRIDGE2X,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2(
	TEMPLATE_BRIDGE2,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2h(
	TEMPLATE_BRIDGE2H,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2d(
	TEMPLATE_BRIDGE2D,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge1ax(
	TEMPLATE_BRIDGE_1AX,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge1a(
	TEMPLATE_BRIDGE_1A,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge1b(
	TEMPLATE_BRIDGE_1B,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge1c(
	TEMPLATE_BRIDGE_1C,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2ax(
	TEMPLATE_BRIDGE_2AX,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2a(
	TEMPLATE_BRIDGE_2A,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2b(
	TEMPLATE_BRIDGE_2B,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2c(
	TEMPLATE_BRIDGE_2C,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge3a(
	TEMPLATE_BRIDGE_3A,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge3b(
	TEMPLATE_BRIDGE_3B,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge3c(
	TEMPLATE_BRIDGE_3C,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge3d(
	TEMPLATE_BRIDGE_3D,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge3e(
	TEMPLATE_BRIDGE_3E,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge3f(
	TEMPLATE_BRIDGE_3F,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const ShoreCliff01(
	TEMPLATE_SHORECLIFF01,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff02(
	TEMPLATE_SHORECLIFF02,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF02",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff03(
	TEMPLATE_SHORECLIFF03,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF03",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff04(
	TEMPLATE_SHORECLIFF04,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF04",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff05(
	TEMPLATE_SHORECLIFF05,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF05",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff06(
	TEMPLATE_SHORECLIFF06,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF06",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff07(
	TEMPLATE_SHORECLIFF07,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF07",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff08(
	TEMPLATE_SHORECLIFF08,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF08",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff09(
	TEMPLATE_SHORECLIFF09,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF09",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff10(
	TEMPLATE_SHORECLIFF10,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF10",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff11(
	TEMPLATE_SHORECLIFF11,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF11",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff12(
	TEMPLATE_SHORECLIFF12,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF12",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff13(
	TEMPLATE_SHORECLIFF13,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF13",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff14(
	TEMPLATE_SHORECLIFF14,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF14",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff15(
	TEMPLATE_SHORECLIFF15,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF15",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff16(
	TEMPLATE_SHORECLIFF16,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF16",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff17(
	TEMPLATE_SHORECLIFF17,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF17",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff18(
	TEMPLATE_SHORECLIFF18,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF18",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff19(
	TEMPLATE_SHORECLIFF19,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF19",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff20(
	TEMPLATE_SHORECLIFF20,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF20",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff21(
	TEMPLATE_SHORECLIFF21,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF21",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff22(
	TEMPLATE_SHORECLIFF22,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF22",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff23(
	TEMPLATE_SHORECLIFF23,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF23",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff24(
	TEMPLATE_SHORECLIFF24,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF24",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff25(
	TEMPLATE_SHORECLIFF25,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF25",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff26(
	TEMPLATE_SHORECLIFF26,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF26",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff27(
	TEMPLATE_SHORECLIFF27,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF27",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff28(
	TEMPLATE_SHORECLIFF28,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF28",
	TXT_SHORE
);

static TemplateTypeClass const Rough01(
	TEMPLATE_ROUGH01,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"RF01",
	TXT_ROCK
);
static TemplateTypeClass const Rough02(
	TEMPLATE_ROUGH02,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"RF02",
	TXT_ROCK
);
static TemplateTypeClass const Rough03(
	TEMPLATE_ROUGH03,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"RF03",
	TXT_ROCK
);
static TemplateTypeClass const Rough04(
	TEMPLATE_ROUGH04,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"RF04",
	TXT_ROCK
);
static TemplateTypeClass const Rough05(
	TEMPLATE_ROUGH05,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"RF05",
	TXT_ROCK
);
static TemplateTypeClass const Rough06(
	TEMPLATE_ROUGH06,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"RF06",
	TXT_ROCK
);
static TemplateTypeClass const Rough07(
	TEMPLATE_ROUGH07,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"RF07",
	TXT_ROCK
);
static TemplateTypeClass const Rough08(
	TEMPLATE_ROUGH08,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"RF08",
	TXT_ROCK
);
static TemplateTypeClass const Rough09(
	TEMPLATE_ROUGH09,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"RF09",
	TXT_ROCK
);
static TemplateTypeClass const Rough10(
	TEMPLATE_ROUGH10,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"RF10",
	TXT_ROCK
);
static TemplateTypeClass const Rough11(
	TEMPLATE_ROUGH11,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"RF11",
	TXT_ROCK
);
static TemplateTypeClass const RiverCliff01(
	TEMPLATE_RIVERCLIFF01,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"RC01",
	TXT_RIVER
);
static TemplateTypeClass const RiverCliff02(
	TEMPLATE_RIVERCLIFF02,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"RC02",
	TXT_RIVER
);
static TemplateTypeClass const RiverCliff03(
	TEMPLATE_RIVERCLIFF03,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"RC03",
	TXT_RIVER
);
static TemplateTypeClass const RiverCliff04(
	TEMPLATE_RIVERCLIFF04,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"RC04",
	TXT_RIVER
);

static TemplateTypeClass const F01(
	TEMPLATE_F01,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"F01",
	TXT_RIVER
);
static TemplateTypeClass const F02(
	TEMPLATE_F02,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"F02",
	TXT_RIVER
);
static TemplateTypeClass const F03(
	TEMPLATE_F03,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"F03",
	TXT_RIVER
);
static TemplateTypeClass const F04(
	TEMPLATE_F04,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"F04",
	TXT_RIVER
);
static TemplateTypeClass const F05(
	TEMPLATE_F05,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"F05",
	TXT_RIVER
);
static TemplateTypeClass const F06(
	TEMPLATE_F06,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"F06",
	TXT_RIVER
);

static TemplateTypeClass const ARRO0001(
	TEMPLATE_ARRO0001,
	THEATERF_INTERIOR,
	"ARRO0001",
	TXT_INTERIOR
);
static TemplateTypeClass const ARRO0002(
	TEMPLATE_ARRO0002,
	THEATERF_INTERIOR,
	"ARRO0002",
	TXT_INTERIOR
);
static TemplateTypeClass const ARRO0003(
	TEMPLATE_ARRO0003,
	THEATERF_INTERIOR,
	"ARRO0003",
	TXT_INTERIOR
);
static TemplateTypeClass const ARRO0004(
	TEMPLATE_ARRO0004,
	THEATERF_INTERIOR,
	"ARRO0004",
	TXT_INTERIOR
);
static TemplateTypeClass const ARRO0005(
	TEMPLATE_ARRO0005,
	THEATERF_INTERIOR,
	"ARRO0005",
	TXT_INTERIOR
);
static TemplateTypeClass const ARRO0006(
	TEMPLATE_ARRO0006,
	THEATERF_INTERIOR,
	"ARRO0006",
	TXT_INTERIOR
);
static TemplateTypeClass const ARRO0007(
	TEMPLATE_ARRO0007,
	THEATERF_INTERIOR,
	"ARRO0007",
	TXT_INTERIOR
);
static TemplateTypeClass const ARRO0008(
	TEMPLATE_ARRO0008,
	THEATERF_INTERIOR,
	"ARRO0008",
	TXT_INTERIOR
);
static TemplateTypeClass const ARRO0009(
	TEMPLATE_ARRO0009,
	THEATERF_INTERIOR,
	"ARRO0009",
	TXT_INTERIOR
);
static TemplateTypeClass const ARRO0010(
	TEMPLATE_ARRO0010,
	THEATERF_INTERIOR,
	"ARRO0010",
	TXT_INTERIOR
);
static TemplateTypeClass const ARRO0011(
	TEMPLATE_ARRO0011,
	THEATERF_INTERIOR,
	"ARRO0011",
	TXT_INTERIOR
);
static TemplateTypeClass const ARRO0012(
	TEMPLATE_ARRO0012,
	THEATERF_INTERIOR,
	"ARRO0012",
	TXT_INTERIOR
);
static TemplateTypeClass const ARRO0013(
	TEMPLATE_ARRO0013,
	THEATERF_INTERIOR,
	"ARRO0013",
	TXT_INTERIOR
);
static TemplateTypeClass const ARRO0014(
	TEMPLATE_ARRO0014,
	THEATERF_INTERIOR,
	"ARRO0014",
	TXT_INTERIOR
);
static TemplateTypeClass const ARRO0015(
	TEMPLATE_ARRO0015,
	THEATERF_INTERIOR,
	"ARRO0015",
	TXT_INTERIOR
);
static TemplateTypeClass const FLOR0001(
	TEMPLATE_FLOR0001,
	THEATERF_INTERIOR,
	"FLOR0001",
	TXT_INTERIOR
);
static TemplateTypeClass const FLOR0002(
	TEMPLATE_FLOR0002,
	THEATERF_INTERIOR,
	"FLOR0002",
	TXT_INTERIOR
);
static TemplateTypeClass const FLOR0003(
	TEMPLATE_FLOR0003,
	THEATERF_INTERIOR,
	"FLOR0003",
	TXT_INTERIOR
);
static TemplateTypeClass const FLOR0004(
	TEMPLATE_FLOR0004,
	THEATERF_INTERIOR,
	"FLOR0004",
	TXT_INTERIOR
);
static TemplateTypeClass const FLOR0005(
	TEMPLATE_FLOR0005,
	THEATERF_INTERIOR,
	"FLOR0005",
	TXT_INTERIOR
);
static TemplateTypeClass const FLOR0006(
	TEMPLATE_FLOR0006,
	THEATERF_INTERIOR,
	"FLOR0006",
	TXT_INTERIOR
);
static TemplateTypeClass const FLOR0007(
	TEMPLATE_FLOR0007,
	THEATERF_INTERIOR,
	"FLOR0007",
	TXT_INTERIOR
);
static TemplateTypeClass const GFLR0001(
	TEMPLATE_GFLR0001,
	THEATERF_INTERIOR,
	"GFLR0001",
	TXT_INTERIOR
);
static TemplateTypeClass const GFLR0002(
	TEMPLATE_GFLR0002,
	THEATERF_INTERIOR,
	"GFLR0002",
	TXT_INTERIOR
);
static TemplateTypeClass const GFLR0003(
	TEMPLATE_GFLR0003,
	THEATERF_INTERIOR,
	"GFLR0003",
	TXT_INTERIOR
);
static TemplateTypeClass const GFLR0004(
	TEMPLATE_GFLR0004,
	THEATERF_INTERIOR,
	"GFLR0004",
	TXT_INTERIOR
);
static TemplateTypeClass const GFLR0005(
	TEMPLATE_GFLR0005,
	THEATERF_INTERIOR,
	"GFLR0005",
	TXT_INTERIOR
);
static TemplateTypeClass const GSTR0001(
	TEMPLATE_GSTR0001,
	THEATERF_INTERIOR,
	"GSTR0001",
	TXT_INTERIOR
);
static TemplateTypeClass const GSTR0002(
	TEMPLATE_GSTR0002,
	THEATERF_INTERIOR,
	"GSTR0002",
	TXT_INTERIOR
);
static TemplateTypeClass const GSTR0003(
	TEMPLATE_GSTR0003,
	THEATERF_INTERIOR,
	"GSTR0003",
	TXT_INTERIOR
);
static TemplateTypeClass const GSTR0004(
	TEMPLATE_GSTR0004,
	THEATERF_INTERIOR,
	"GSTR0004",
	TXT_INTERIOR
);
static TemplateTypeClass const GSTR0005(
	TEMPLATE_GSTR0005,
	THEATERF_INTERIOR,
	"GSTR0005",
	TXT_INTERIOR
);
static TemplateTypeClass const GSTR0006(
	TEMPLATE_GSTR0006,
	THEATERF_INTERIOR,
	"GSTR0006",
	TXT_INTERIOR
);
static TemplateTypeClass const GSTR0007(
	TEMPLATE_GSTR0007,
	THEATERF_INTERIOR,
	"GSTR0007",
	TXT_INTERIOR
);
static TemplateTypeClass const GSTR0008(
	TEMPLATE_GSTR0008,
	THEATERF_INTERIOR,
	"GSTR0008",
	TXT_INTERIOR
);
static TemplateTypeClass const GSTR0009(
	TEMPLATE_GSTR0009,
	THEATERF_INTERIOR,
	"GSTR0009",
	TXT_INTERIOR
);
static TemplateTypeClass const GSTR0010(
	TEMPLATE_GSTR0010,
	THEATERF_INTERIOR,
	"GSTR0010",
	TXT_INTERIOR
);
static TemplateTypeClass const GSTR0011(
	TEMPLATE_GSTR0011,
	THEATERF_INTERIOR,
	"GSTR0011",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0001(
	TEMPLATE_LWAL0001,
	THEATERF_INTERIOR,
	"LWAL0001",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0002(
	TEMPLATE_LWAL0002,
	THEATERF_INTERIOR,
	"LWAL0002",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0003(
	TEMPLATE_LWAL0003,
	THEATERF_INTERIOR,
	"LWAL0003",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0004(
	TEMPLATE_LWAL0004,
	THEATERF_INTERIOR,
	"LWAL0004",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0005(
	TEMPLATE_LWAL0005,
	THEATERF_INTERIOR,
	"LWAL0005",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0006(
	TEMPLATE_LWAL0006,
	THEATERF_INTERIOR,
	"LWAL0006",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0007(
	TEMPLATE_LWAL0007,
	THEATERF_INTERIOR,
	"LWAL0007",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0008(
	TEMPLATE_LWAL0008,
	THEATERF_INTERIOR,
	"LWAL0008",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0009(
	TEMPLATE_LWAL0009,
	THEATERF_INTERIOR,
	"LWAL0009",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0010(
	TEMPLATE_LWAL0010,
	THEATERF_INTERIOR,
	"LWAL0010",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0011(
	TEMPLATE_LWAL0011,
	THEATERF_INTERIOR,
	"LWAL0011",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0012(
	TEMPLATE_LWAL0012,
	THEATERF_INTERIOR,
	"LWAL0012",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0013(
	TEMPLATE_LWAL0013,
	THEATERF_INTERIOR,
	"LWAL0013",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0014(
	TEMPLATE_LWAL0014,
	THEATERF_INTERIOR,
	"LWAL0014",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0015(
	TEMPLATE_LWAL0015,
	THEATERF_INTERIOR,
	"LWAL0015",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0016(
	TEMPLATE_LWAL0016,
	THEATERF_INTERIOR,
	"LWAL0016",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0017(
	TEMPLATE_LWAL0017,
	THEATERF_INTERIOR,
	"LWAL0017",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0018(
	TEMPLATE_LWAL0018,
	THEATERF_INTERIOR,
	"LWAL0018",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0019(
	TEMPLATE_LWAL0019,
	THEATERF_INTERIOR,
	"LWAL0019",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0020(
	TEMPLATE_LWAL0020,
	THEATERF_INTERIOR,
	"LWAL0020",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0021(
	TEMPLATE_LWAL0021,
	THEATERF_INTERIOR,
	"LWAL0021",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0022(
	TEMPLATE_LWAL0022,
	THEATERF_INTERIOR,
	"LWAL0022",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0023(
	TEMPLATE_LWAL0023,
	THEATERF_INTERIOR,
	"LWAL0023",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0024(
	TEMPLATE_LWAL0024,
	THEATERF_INTERIOR,
	"LWAL0024",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0025(
	TEMPLATE_LWAL0025,
	THEATERF_INTERIOR,
	"LWAL0025",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0026(
	TEMPLATE_LWAL0026,
	THEATERF_INTERIOR,
	"LWAL0026",
	TXT_INTERIOR
);
static TemplateTypeClass const LWAL0027(
	TEMPLATE_LWAL0027,
	THEATERF_INTERIOR,
	"LWAL0027",
	TXT_INTERIOR
);
static TemplateTypeClass const STRP0001(
	TEMPLATE_STRP0001,
	THEATERF_INTERIOR,
	"STRP0001",
	TXT_INTERIOR
);
static TemplateTypeClass const STRP0002(
	TEMPLATE_STRP0002,
	THEATERF_INTERIOR,
	"STRP0002",
	TXT_INTERIOR
);
static TemplateTypeClass const STRP0003(
	TEMPLATE_STRP0003,
	THEATERF_INTERIOR,
	"STRP0003",
	TXT_INTERIOR
);
static TemplateTypeClass const STRP0004(
	TEMPLATE_STRP0004,
	THEATERF_INTERIOR,
	"STRP0004",
	TXT_INTERIOR
);
static TemplateTypeClass const STRP0005(
	TEMPLATE_STRP0005,
	THEATERF_INTERIOR,
	"STRP0005",
	TXT_INTERIOR
);
static TemplateTypeClass const STRP0006(
	TEMPLATE_STRP0006,
	THEATERF_INTERIOR,
	"STRP0006",
	TXT_INTERIOR
);
static TemplateTypeClass const STRP0007(
	TEMPLATE_STRP0007,
	THEATERF_INTERIOR,
	"STRP0007",
	TXT_INTERIOR
);
static TemplateTypeClass const STRP0008(
	TEMPLATE_STRP0008,
	THEATERF_INTERIOR,
	"STRP0008",
	TXT_INTERIOR
);
static TemplateTypeClass const STRP0009(
	TEMPLATE_STRP0009,
	THEATERF_INTERIOR,
	"STRP0009",
	TXT_INTERIOR
);
static TemplateTypeClass const STRP0010(
	TEMPLATE_STRP0010,
	THEATERF_INTERIOR,
	"STRP0010",
	TXT_INTERIOR
);
static TemplateTypeClass const STRP0011(
	TEMPLATE_STRP0011,
	THEATERF_INTERIOR,
	"STRP0011",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0001(
	TEMPLATE_WALL0001,
	THEATERF_INTERIOR,
	"WALL0001",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0002(
	TEMPLATE_WALL0002,
	THEATERF_INTERIOR,
	"WALL0002",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0003(
	TEMPLATE_WALL0003,
	THEATERF_INTERIOR,
	"WALL0003",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0004(
	TEMPLATE_WALL0004,
	THEATERF_INTERIOR,
	"WALL0004",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0005(
	TEMPLATE_WALL0005,
	THEATERF_INTERIOR,
	"WALL0005",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0006(
	TEMPLATE_WALL0006,
	THEATERF_INTERIOR,
	"WALL0006",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0007(
	TEMPLATE_WALL0007,
	THEATERF_INTERIOR,
	"WALL0007",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0008(
	TEMPLATE_WALL0008,
	THEATERF_INTERIOR,
	"WALL0008",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0009(
	TEMPLATE_WALL0009,
	THEATERF_INTERIOR,
	"WALL0009",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0010(
	TEMPLATE_WALL0010,
	THEATERF_INTERIOR,
	"WALL0010",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0011(
	TEMPLATE_WALL0011,
	THEATERF_INTERIOR,
	"WALL0011",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0012(
	TEMPLATE_WALL0012,
	THEATERF_INTERIOR,
	"WALL0012",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0013(
	TEMPLATE_WALL0013,
	THEATERF_INTERIOR,
	"WALL0013",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0014(
	TEMPLATE_WALL0014,
	THEATERF_INTERIOR,
	"WALL0014",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0015(
	TEMPLATE_WALL0015,
	THEATERF_INTERIOR,
	"WALL0015",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0016(
	TEMPLATE_WALL0016,
	THEATERF_INTERIOR,
	"WALL0016",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0017(
	TEMPLATE_WALL0017,
	THEATERF_INTERIOR,
	"WALL0017",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0018(
	TEMPLATE_WALL0018,
	THEATERF_INTERIOR,
	"WALL0018",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0019(
	TEMPLATE_WALL0019,
	THEATERF_INTERIOR,
	"WALL0019",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0020(
	TEMPLATE_WALL0020,
	THEATERF_INTERIOR,
	"WALL0020",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0021(
	TEMPLATE_WALL0021,
	THEATERF_INTERIOR,
	"WALL0021",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0022(
	TEMPLATE_WALL0022,
	THEATERF_INTERIOR,
	"WALL0022",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0023(
	TEMPLATE_WALL0023,
	THEATERF_INTERIOR,
	"WALL0023",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0024(
	TEMPLATE_WALL0024,
	THEATERF_INTERIOR,
	"WALL0024",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0025(
	TEMPLATE_WALL0025,
	THEATERF_INTERIOR,
	"WALL0025",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0026(
	TEMPLATE_WALL0026,
	THEATERF_INTERIOR,
	"WALL0026",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0027(
	TEMPLATE_WALL0027,
	THEATERF_INTERIOR,
	"WALL0027",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0028(
	TEMPLATE_WALL0028,
	THEATERF_INTERIOR,
	"WALL0028",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0029(
	TEMPLATE_WALL0029,
	THEATERF_INTERIOR,
	"WALL0029",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0030(
	TEMPLATE_WALL0030,
	THEATERF_INTERIOR,
	"WALL0030",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0031(
	TEMPLATE_WALL0031,
	THEATERF_INTERIOR,
	"WALL0031",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0032(
	TEMPLATE_WALL0032,
	THEATERF_INTERIOR,
	"WALL0032",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0033(
	TEMPLATE_WALL0033,
	THEATERF_INTERIOR,
	"WALL0033",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0034(
	TEMPLATE_WALL0034,
	THEATERF_INTERIOR,
	"WALL0034",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0035(
	TEMPLATE_WALL0035,
	THEATERF_INTERIOR,
	"WALL0035",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0036(
	TEMPLATE_WALL0036,
	THEATERF_INTERIOR,
	"WALL0036",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0037(
	TEMPLATE_WALL0037,
	THEATERF_INTERIOR,
	"WALL0037",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0038(
	TEMPLATE_WALL0038,
	THEATERF_INTERIOR,
	"WALL0038",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0039(
	TEMPLATE_WALL0039,
	THEATERF_INTERIOR,
	"WALL0039",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0040(
	TEMPLATE_WALL0040,
	THEATERF_INTERIOR,
	"WALL0040",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0041(
	TEMPLATE_WALL0041,
	THEATERF_INTERIOR,
	"WALL0041",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0042(
	TEMPLATE_WALL0042,
	THEATERF_INTERIOR,
	"WALL0042",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0043(
	TEMPLATE_WALL0043,
	THEATERF_INTERIOR,
	"WALL0043",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0044(
	TEMPLATE_WALL0044,
	THEATERF_INTERIOR,
	"WALL0044",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0045(
	TEMPLATE_WALL0045,
	THEATERF_INTERIOR,
	"WALL0045",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0046(
	TEMPLATE_WALL0046,
	THEATERF_INTERIOR,
	"WALL0046",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0047(
	TEMPLATE_WALL0047,
	THEATERF_INTERIOR,
	"WALL0047",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0048(
	TEMPLATE_WALL0048,
	THEATERF_INTERIOR,
	"WALL0048",
	TXT_INTERIOR
);
static TemplateTypeClass const WALL0049(
	TEMPLATE_WALL0049,
	THEATERF_INTERIOR,
	"WALL0049",
	TXT_INTERIOR
);

//static TemplateTypeClass const Xtra0001(
//	TEMPLATE_XTRA0001,
//	THEATERF_INTERIOR,
//	"XTRA0001",
//	TXT_INTERIOR
//);
//static TemplateTypeClass const Xtra0002(
//	TEMPLATE_XTRA0002,
//	THEATERF_INTERIOR,
//	"XTRA0002",
//	TXT_INTERIOR
//);
//static TemplateTypeClass const Xtra0003(
//	TEMPLATE_XTRA0003,
//	THEATERF_INTERIOR,
//	"XTRA0003",
//	TXT_INTERIOR
//);
//static TemplateTypeClass const Xtra0004(
//	TEMPLATE_XTRA0004,
//	THEATERF_INTERIOR,
//	"XTRA0004",
//	TXT_INTERIOR
//);
//static TemplateTypeClass const Xtra0005(
//	TEMPLATE_XTRA0005,
//	THEATERF_INTERIOR,
//	"XTRA0005",
//	TXT_INTERIOR
//);
//static TemplateTypeClass const Xtra0006(
//	TEMPLATE_XTRA0006,
//	THEATERF_INTERIOR,
//	"XTRA0006",
//	TXT_INTERIOR
//);
//static TemplateTypeClass const Xtra0007(
//	TEMPLATE_XTRA0007,
//	THEATERF_INTERIOR,
//	"XTRA0007",
//	TXT_INTERIOR
//);
//static TemplateTypeClass const Xtra0008(
//	TEMPLATE_XTRA0008,
//	THEATERF_INTERIOR,
//	"XTRA0008",
//	TXT_INTERIOR
//);
//static TemplateTypeClass const Xtra0009(
//	TEMPLATE_XTRA0009,
//	THEATERF_INTERIOR,
//	"XTRA0009",
//	TXT_INTERIOR
//);
//static TemplateTypeClass const Xtra0010(
//	TEMPLATE_XTRA0010,
//	THEATERF_INTERIOR,
//	"XTRA0010",
//	TXT_INTERIOR
//);
//static TemplateTypeClass const Xtra0011(
//	TEMPLATE_XTRA0011,
//	THEATERF_INTERIOR,
//	"XTRA0011",
//	TXT_INTERIOR
//);
//static TemplateTypeClass const Xtra0012(
//	TEMPLATE_XTRA0012,
//	THEATERF_INTERIOR,
//	"XTRA0012",
//	TXT_INTERIOR
//);
//static TemplateTypeClass const Xtra0013(
//	TEMPLATE_XTRA0013,
//	THEATERF_INTERIOR,
//	"XTRA0013",
//	TXT_INTERIOR
//);
//static TemplateTypeClass const Xtra0014(
//	TEMPLATE_XTRA0014,
//	THEATERF_INTERIOR,
//	"XTRA0014",
//	TXT_INTERIOR
//);
//static TemplateTypeClass const Xtra0015(
//	TEMPLATE_XTRA0015,
//	THEATERF_INTERIOR,
//	"XTRA0015",
//	TXT_INTERIOR
//);
//static TemplateTypeClass const Xtra0016(
//	TEMPLATE_XTRA0016,
//	THEATERF_INTERIOR,
//	"XTRA0016",
//	TXT_INTERIOR
//);

#ifdef FIXIT_ANTS
static TemplateTypeClass const AntHill(
	TEMPLATE_HILL01,
	THEATERF_TEMPERATE,
	"HILL01",
	TXT_ROCK
);
#endif

/***********************************************************************************************
 * TemplateTypeClass::TemplateTypeClass -- Constructor for template type objects.              *
 *                                                                                             *
 *    This is the constructor for the template types.                                          *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TemplateTypeClass::TemplateTypeClass(
	TemplateType iconset,
	int theater,
	char const * ininame,
	int fullname) :
		ObjectTypeClass(
			RTTI_TEMPLATETYPE,
			int(iconset),
			false,
			true,
			false,
			false,
			true,
			true,
			false,
			fullname,
			ininame),
	Type(iconset),
	Theater(theater),
	Width(0),
	Height(0)
{
}


/***********************************************************************************************
 * TemplateTypeClass::operator new -- Allocates a template type from special heap.             *
 *                                                                                             *
 *    This allocates a template type object from the special heap used for that purpose.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the newly allocated template type object. If no object   *
 *          could be allocated, then NULL is returned.                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * TemplateTypeClass::operator new(size_t)
{
	return(TemplateTypes.Alloc());
}


/***********************************************************************************************
 * TemplateTypeClass::operator delete -- Deletes a template type object.                       *
 *                                                                                             *
 *    This routine will return a template type object back to the special heap it was          *
 *    allocated from.                                                                          *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the template type object to free.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TemplateTypeClass::operator delete(void * ptr)
{
	TemplateTypes.Free((TemplateTypeClass *)ptr);
}


static void _Watcom_Ugh_Hack(void)
{
	(void)new TemplateTypeClass(Road37);			//	TEMPLATE_ROAD37
	(void)new TemplateTypeClass(Road38);			//	TEMPLATE_ROAD38
	(void)new TemplateTypeClass(Road39);			//	TEMPLATE_ROAD39
	(void)new TemplateTypeClass(Road40);			//	TEMPLATE_ROAD40
	(void)new TemplateTypeClass(Road41);			//	TEMPLATE_ROAD41
	(void)new TemplateTypeClass(Road42);			//	TEMPLATE_ROAD42
	(void)new TemplateTypeClass(Road43);			//	TEMPLATE_ROAD43
	(void)new TemplateTypeClass(Rough01);			// TEMPLATE_ROUGH01
	(void)new TemplateTypeClass(Rough02);			// TEMPLATE_ROUGH02
	(void)new TemplateTypeClass(Rough03);			// TEMPLATE_ROUGH03
	(void)new TemplateTypeClass(Rough04);			// TEMPLATE_ROUGH04
	(void)new TemplateTypeClass(Rough05);			// TEMPLATE_ROUGH05
	(void)new TemplateTypeClass(Rough06);			// TEMPLATE_ROUGH06
	(void)new TemplateTypeClass(Rough07);			// TEMPLATE_ROUGH07
	(void)new TemplateTypeClass(Rough08);			// TEMPLATE_ROUGH08
	(void)new TemplateTypeClass(Rough09);			// TEMPLATE_ROUGH09
	(void)new TemplateTypeClass(Rough10);			// TEMPLATE_ROUGH10
	(void)new TemplateTypeClass(Rough11);			// TEMPLATE_ROUGH11
	(void)new TemplateTypeClass(Road44);			//	TEMPLATE_ROAD44
	(void)new TemplateTypeClass(Road45);			//	TEMPLATE_ROAD45
//	(void)new TemplateTypeClass(River14);			//	TEMPLATE_RIVER14
//	(void)new TemplateTypeClass(River15);			//	TEMPLATE_RIVER15
	(void)new TemplateTypeClass(RiverCliff01);	//	TEMPLATE_RIVERCLIFF01
	(void)new TemplateTypeClass(RiverCliff02);	//	TEMPLATE_RIVERCLIFF02
	(void)new TemplateTypeClass(RiverCliff03);	//	TEMPLATE_RIVERCLIFF03
	(void)new TemplateTypeClass(RiverCliff04);	//	TEMPLATE_RIVERCLIFF04
	(void)new TemplateTypeClass(Bridge1a);			//	TEMPLATE_BRIDGE_1A
	(void)new TemplateTypeClass(Bridge1b);			//	TEMPLATE_BRIDGE_1B
	(void)new TemplateTypeClass(Bridge1c);			//	TEMPLATE_BRIDGE_1C
	(void)new TemplateTypeClass(Bridge2a);			//	TEMPLATE_BRIDGE_2A
	(void)new TemplateTypeClass(Bridge2b);			//	TEMPLATE_BRIDGE_2B
	(void)new TemplateTypeClass(Bridge2c);			//	TEMPLATE_BRIDGE_2C
	(void)new TemplateTypeClass(Bridge3a);			//	TEMPLATE_BRIDGE_3A
	(void)new TemplateTypeClass(Bridge3b);			//	TEMPLATE_BRIDGE_3B
	(void)new TemplateTypeClass(Bridge3c);			//	TEMPLATE_BRIDGE_3C
	(void)new TemplateTypeClass(Bridge3d);			//	TEMPLATE_BRIDGE_3D
	(void)new TemplateTypeClass(Bridge3e);			//	TEMPLATE_BRIDGE_3E
	(void)new TemplateTypeClass(Bridge3f);			//	TEMPLATE_BRIDGE_3F
	(void)new TemplateTypeClass(F01);				//	TEMPLATE_F01
	(void)new TemplateTypeClass(F02);				//	TEMPLATE_F02
	(void)new TemplateTypeClass(F03);				//	TEMPLATE_F03
	(void)new TemplateTypeClass(F04);				//	TEMPLATE_F04
	(void)new TemplateTypeClass(F05);				//	TEMPLATE_F05
	(void)new TemplateTypeClass(F06);				//	TEMPLATE_F06
	(void)new TemplateTypeClass(ARRO0001);			//	TEMPLATE_ARRO0001
	(void)new TemplateTypeClass(ARRO0002);			//	TEMPLATE_ARRO0002
	(void)new TemplateTypeClass(ARRO0003);			//	TEMPLATE_ARRO0003
	(void)new TemplateTypeClass(ARRO0004);			//	TEMPLATE_ARRO0004
	(void)new TemplateTypeClass(ARRO0005);			//	TEMPLATE_ARRO0005
	(void)new TemplateTypeClass(ARRO0006);			//	TEMPLATE_ARRO0006
	(void)new TemplateTypeClass(ARRO0007);			//	TEMPLATE_ARRO0007
	(void)new TemplateTypeClass(ARRO0008);			//	TEMPLATE_ARRO0008
	(void)new TemplateTypeClass(ARRO0009);			//	TEMPLATE_ARRO0009
	(void)new TemplateTypeClass(ARRO0010);			//	TEMPLATE_ARRO0010
	(void)new TemplateTypeClass(ARRO0011);			//	TEMPLATE_ARRO0011
	(void)new TemplateTypeClass(ARRO0012);			//	TEMPLATE_ARRO0012
	(void)new TemplateTypeClass(ARRO0013);			//	TEMPLATE_ARRO0013
	(void)new TemplateTypeClass(ARRO0014);			//	TEMPLATE_ARRO0014
	(void)new TemplateTypeClass(ARRO0015);			//	TEMPLATE_ARRO0015
	(void)new TemplateTypeClass(FLOR0001);			//	TEMPLATE_FLOR0001
	(void)new TemplateTypeClass(FLOR0002);			//	TEMPLATE_FLOR0002
	(void)new TemplateTypeClass(FLOR0003);			//	TEMPLATE_FLOR0003
	(void)new TemplateTypeClass(FLOR0004);			//	TEMPLATE_FLOR0004
	(void)new TemplateTypeClass(FLOR0005);			//	TEMPLATE_FLOR0005
	(void)new TemplateTypeClass(FLOR0006);			//	TEMPLATE_FLOR0006
	(void)new TemplateTypeClass(FLOR0007);			//	TEMPLATE_FLOR0007
	(void)new TemplateTypeClass(GFLR0001);			//	TEMPLATE_GFLR0001
	(void)new TemplateTypeClass(GFLR0002);			//	TEMPLATE_GFLR0002
	(void)new TemplateTypeClass(GFLR0003);			//	TEMPLATE_GFLR0003
	(void)new TemplateTypeClass(GFLR0004);			//	TEMPLATE_GFLR0004
	(void)new TemplateTypeClass(GFLR0005);			//	TEMPLATE_GFLR0005
	(void)new TemplateTypeClass(GSTR0001);			//	TEMPLATE_GSTR0001
	(void)new TemplateTypeClass(GSTR0002);			//	TEMPLATE_GSTR0002
	(void)new TemplateTypeClass(GSTR0003);			//	TEMPLATE_GSTR0003
	(void)new TemplateTypeClass(GSTR0004);			//	TEMPLATE_GSTR0004
	(void)new TemplateTypeClass(GSTR0005);			//	TEMPLATE_GSTR0005
	(void)new TemplateTypeClass(GSTR0006);			//	TEMPLATE_GSTR0006
	(void)new TemplateTypeClass(GSTR0007);			//	TEMPLATE_GSTR0007
	(void)new TemplateTypeClass(GSTR0008);			//	TEMPLATE_GSTR0008
	(void)new TemplateTypeClass(GSTR0009);			//	TEMPLATE_GSTR0009
	(void)new TemplateTypeClass(GSTR0010);			//	TEMPLATE_GSTR0010
	(void)new TemplateTypeClass(GSTR0011);			//	TEMPLATE_GSTR0011
	(void)new TemplateTypeClass(LWAL0001);			//	TEMPLATE_LWAL0001
	(void)new TemplateTypeClass(LWAL0002);			//	TEMPLATE_LWAL0002
	(void)new TemplateTypeClass(LWAL0003);			//	TEMPLATE_LWAL0003
	(void)new TemplateTypeClass(LWAL0004);			//	TEMPLATE_LWAL0004
	(void)new TemplateTypeClass(LWAL0005);			//	TEMPLATE_LWAL0005
	(void)new TemplateTypeClass(LWAL0006);			//	TEMPLATE_LWAL0006
	(void)new TemplateTypeClass(LWAL0007);			//	TEMPLATE_LWAL0007
	(void)new TemplateTypeClass(LWAL0008);			//	TEMPLATE_LWAL0008
	(void)new TemplateTypeClass(LWAL0009);			//	TEMPLATE_LWAL0009
	(void)new TemplateTypeClass(LWAL0010);			//	TEMPLATE_LWAL0010
	(void)new TemplateTypeClass(LWAL0011);			//	TEMPLATE_LWAL0011
	(void)new TemplateTypeClass(LWAL0012);			//	TEMPLATE_LWAL0012
	(void)new TemplateTypeClass(LWAL0013);			//	TEMPLATE_LWAL0013
	(void)new TemplateTypeClass(LWAL0014);			//	TEMPLATE_LWAL0014
	(void)new TemplateTypeClass(LWAL0015);			//	TEMPLATE_LWAL0015
	(void)new TemplateTypeClass(LWAL0016);			//	TEMPLATE_LWAL0016
	(void)new TemplateTypeClass(LWAL0017);			//	TEMPLATE_LWAL0017
	(void)new TemplateTypeClass(LWAL0018);			//	TEMPLATE_LWAL0018
	(void)new TemplateTypeClass(LWAL0019);			//	TEMPLATE_LWAL0019
	(void)new TemplateTypeClass(LWAL0020);			//	TEMPLATE_LWAL0020
	(void)new TemplateTypeClass(LWAL0021);			//	TEMPLATE_LWAL0021
	(void)new TemplateTypeClass(LWAL0022);			//	TEMPLATE_LWAL0022
	(void)new TemplateTypeClass(LWAL0023);			//	TEMPLATE_LWAL0023
	(void)new TemplateTypeClass(LWAL0024);			//	TEMPLATE_LWAL0024
	(void)new TemplateTypeClass(LWAL0025);			//	TEMPLATE_LWAL0025
	(void)new TemplateTypeClass(LWAL0026);			//	TEMPLATE_LWAL0026
	(void)new TemplateTypeClass(LWAL0027);			//	TEMPLATE_LWAL0027
	(void)new TemplateTypeClass(STRP0001);			//	TEMPLATE_STRP0001
	(void)new TemplateTypeClass(STRP0002);			//	TEMPLATE_STRP0002
	(void)new TemplateTypeClass(STRP0003);			//	TEMPLATE_STRP0003
	(void)new TemplateTypeClass(STRP0004);			//	TEMPLATE_STRP0004
	(void)new TemplateTypeClass(STRP0005);			//	TEMPLATE_STRP0005
	(void)new TemplateTypeClass(STRP0006);			//	TEMPLATE_STRP0006
	(void)new TemplateTypeClass(STRP0007);			//	TEMPLATE_STRP0007
	(void)new TemplateTypeClass(STRP0008);			//	TEMPLATE_STRP0008
	(void)new TemplateTypeClass(STRP0009);			//	TEMPLATE_STRP0009
	(void)new TemplateTypeClass(STRP0010);			//	TEMPLATE_STRP0010
	(void)new TemplateTypeClass(STRP0011);			//	TEMPLATE_STRP0011
	(void)new TemplateTypeClass(WALL0001);			//	TEMPLATE_WALL0001
	(void)new TemplateTypeClass(WALL0002);			//	TEMPLATE_WALL0002
	(void)new TemplateTypeClass(WALL0003);			//	TEMPLATE_WALL0003
	(void)new TemplateTypeClass(WALL0004);			//	TEMPLATE_WALL0004
	(void)new TemplateTypeClass(WALL0005);			//	TEMPLATE_WALL0005
	(void)new TemplateTypeClass(WALL0006);			//	TEMPLATE_WALL0006
	(void)new TemplateTypeClass(WALL0007);			//	TEMPLATE_WALL0007
	(void)new TemplateTypeClass(WALL0008);			//	TEMPLATE_WALL0008
	(void)new TemplateTypeClass(WALL0009);			//	TEMPLATE_WALL0009
	(void)new TemplateTypeClass(WALL0010);			//	TEMPLATE_WALL0010
	(void)new TemplateTypeClass(WALL0011);			//	TEMPLATE_WALL0011
	(void)new TemplateTypeClass(WALL0012);			//	TEMPLATE_WALL0012
	(void)new TemplateTypeClass(WALL0013);			//	TEMPLATE_WALL0013
	(void)new TemplateTypeClass(WALL0014);			//	TEMPLATE_WALL0014
	(void)new TemplateTypeClass(WALL0015);			//	TEMPLATE_WALL0015
	(void)new TemplateTypeClass(WALL0016);			//	TEMPLATE_WALL0016
	(void)new TemplateTypeClass(WALL0017);			//	TEMPLATE_WALL0017
	(void)new TemplateTypeClass(WALL0018);			//	TEMPLATE_WALL0018
	(void)new TemplateTypeClass(WALL0019);			//	TEMPLATE_WALL0019
	(void)new TemplateTypeClass(WALL0020);			//	TEMPLATE_WALL0020
	(void)new TemplateTypeClass(WALL0021);			//	TEMPLATE_WALL0021
	(void)new TemplateTypeClass(WALL0022);			//	TEMPLATE_WALL0022
	(void)new TemplateTypeClass(WALL0023);			//	TEMPLATE_WALL0023
	(void)new TemplateTypeClass(WALL0024);			//	TEMPLATE_WALL0024
	(void)new TemplateTypeClass(WALL0025);			//	TEMPLATE_WALL0025
	(void)new TemplateTypeClass(WALL0026);			//	TEMPLATE_WALL0026
	(void)new TemplateTypeClass(WALL0027);			//	TEMPLATE_WALL0027
	(void)new TemplateTypeClass(WALL0028);			//	TEMPLATE_WALL0028
	(void)new TemplateTypeClass(WALL0029);			//	TEMPLATE_WALL0029
	(void)new TemplateTypeClass(WALL0030);			//	TEMPLATE_WALL0030
	(void)new TemplateTypeClass(WALL0031);			//	TEMPLATE_WALL0031
	(void)new TemplateTypeClass(WALL0032);			//	TEMPLATE_WALL0032
	(void)new TemplateTypeClass(WALL0033);			//	TEMPLATE_WALL0033
	(void)new TemplateTypeClass(WALL0034);			//	TEMPLATE_WALL0034
	(void)new TemplateTypeClass(WALL0035);			//	TEMPLATE_WALL0035
	(void)new TemplateTypeClass(WALL0036);			//	TEMPLATE_WALL0036
	(void)new TemplateTypeClass(WALL0037);			//	TEMPLATE_WALL0037
	(void)new TemplateTypeClass(WALL0038);			//	TEMPLATE_WALL0038
	(void)new TemplateTypeClass(WALL0039);			//	TEMPLATE_WALL0039
	(void)new TemplateTypeClass(WALL0040);			//	TEMPLATE_WALL0040
	(void)new TemplateTypeClass(WALL0041);			//	TEMPLATE_WALL0041
	(void)new TemplateTypeClass(WALL0042);			//	TEMPLATE_WALL0042
	(void)new TemplateTypeClass(WALL0043);			//	TEMPLATE_WALL0043
	(void)new TemplateTypeClass(WALL0044);			//	TEMPLATE_WALL0044
	(void)new TemplateTypeClass(WALL0045);			//	TEMPLATE_WALL0045
	(void)new TemplateTypeClass(WALL0046);			//	TEMPLATE_WALL0046
	(void)new TemplateTypeClass(WALL0047);			//	TEMPLATE_WALL0047
	(void)new TemplateTypeClass(WALL0048);			//	TEMPLATE_WALL0048
	(void)new TemplateTypeClass(WALL0049);			//	TEMPLATE_WALL0049
	(void)new TemplateTypeClass(Bridge1h);			//	TEMPLATE_BRIDGE1H
	(void)new TemplateTypeClass(Bridge2h);			//	TEMPLATE_BRIDGE2H

	(void)new TemplateTypeClass(Bridge1ax);		//	TEMPLATE_BRIDGE_1AX
	(void)new TemplateTypeClass(Bridge2ax);		//	TEMPLATE_BRIDGE_2AX
	(void)new TemplateTypeClass(Bridge1x);			//	TEMPLATE_BRIDGE1X
	(void)new TemplateTypeClass(Bridge2x);			//	TEMPLATE_BRIDGE2X

	//(void)new TemplateTypeClass(Xtra0001);			//	TEMPLATE_XTRA0001
	//(void)new TemplateTypeClass(Xtra0002);			//	TEMPLATE_XTRA0002
	//(void)new TemplateTypeClass(Xtra0003);			//	TEMPLATE_XTRA0003
	//(void)new TemplateTypeClass(Xtra0004);			//	TEMPLATE_XTRA0004
	//(void)new TemplateTypeClass(Xtra0005);			//	TEMPLATE_XTRA0005
	//(void)new TemplateTypeClass(Xtra0006);			//	TEMPLATE_XTRA0006
	//(void)new TemplateTypeClass(Xtra0007);			//	TEMPLATE_XTRA0007
	//(void)new TemplateTypeClass(Xtra0008);			//	TEMPLATE_XTRA0008
	//(void)new TemplateTypeClass(Xtra0009);			//	TEMPLATE_XTRA0009
	//(void)new TemplateTypeClass(Xtra0010);			//	TEMPLATE_XTRA0010
	//(void)new TemplateTypeClass(Xtra0011);			//	TEMPLATE_XTRA0011
	//(void)new TemplateTypeClass(Xtra0012);			//	TEMPLATE_XTRA0012
	//(void)new TemplateTypeClass(Xtra0013);			//	TEMPLATE_XTRA0013
	//(void)new TemplateTypeClass(Xtra0014);			//	TEMPLATE_XTRA0014
	//(void)new TemplateTypeClass(Xtra0015);			//	TEMPLATE_XTRA0015
	//(void)new TemplateTypeClass(Xtra0016);			//	TEMPLATE_XTRA0016

#ifdef FIXIT_ANTS
	(void)new TemplateTypeClass(AntHill);			//	TEMPLATE_ROAD36
#endif
}



void TemplateTypeClass::Init_Heap(void)
{
	/*
	**	These template type class objects must be allocated in the exact order that they
	**	are specified in the TemplateType enumeration. This is necessary because the heap
	**	allocation block index serves double duty as the type number index.
	*/
	(void)new TemplateTypeClass(Clear);				// TEMPLATE_CLEAR1
	(void)new TemplateTypeClass(Water);				// TEMPLATE_WATER
	(void)new TemplateTypeClass(Water2);			// TEMPLATE_WATER2
	(void)new TemplateTypeClass(Shore01);			// TEMPLATE_SHORE1
	(void)new TemplateTypeClass(Shore02);			// TEMPLATE_SHORE2
	(void)new TemplateTypeClass(Shore03);			// TEMPLATE_SHORE3
	(void)new TemplateTypeClass(Shore04);			// TEMPLATE_SHORE4
	(void)new TemplateTypeClass(Shore05);			// TEMPLATE_SHORE5
	(void)new TemplateTypeClass(Shore06);			// TEMPLATE_SHORE6
	(void)new TemplateTypeClass(Shore07);			// TEMPLATE_SHORE7
	(void)new TemplateTypeClass(Shore08);			// TEMPLATE_SHORE8
	(void)new TemplateTypeClass(Shore09);			// TEMPLATE_SHORE9
	(void)new TemplateTypeClass(Shore10);			// TEMPLATE_SHORE10
	(void)new TemplateTypeClass(Shore11);			//	TEMPLATE_SHORE11
	(void)new TemplateTypeClass(Shore12);			// TEMPLATE_SHORE12
	(void)new TemplateTypeClass(Shore13);			// TEMPLATE_SHORE13
	(void)new TemplateTypeClass(Shore14);			// TEMPLATE_SHORE14
	(void)new TemplateTypeClass(Shore15);			// TEMPLATE_SHORE15
	(void)new TemplateTypeClass(Shore16);			//	TEMPLATE_SHORE16
	(void)new TemplateTypeClass(Shore17);			//	TEMPLATE_SHORE17
	(void)new TemplateTypeClass(Shore18);			//	TEMPLATE_SHORE18
	(void)new TemplateTypeClass(Shore19);			// TEMPLATE_SHORE19
	(void)new TemplateTypeClass(Shore20);			// TEMPLATE_SHORE20
	(void)new TemplateTypeClass(Shore21);			// TEMPLATE_SHORE21
	(void)new TemplateTypeClass(Shore22);			//	TEMPLATE_SHORE22
	(void)new TemplateTypeClass(Shore23);			// TEMPLATE_SHORE23
	(void)new TemplateTypeClass(Shore24);			//	TEMPLATE_SHORE24
	(void)new TemplateTypeClass(Shore25);			//	TEMPLATE_SHORE25
	(void)new TemplateTypeClass(Shore26);			//	TEMPLATE_SHORE26
	(void)new TemplateTypeClass(Shore27);			//	TEMPLATE_SHORE27
	(void)new TemplateTypeClass(Shore28);			//	TEMPLATE_SHORE28
	(void)new TemplateTypeClass(Shore29);			//	TEMPLATE_SHORE29
	(void)new TemplateTypeClass(Shore30);			//	TEMPLATE_SHORE30
	(void)new TemplateTypeClass(Shore31);			//	TEMPLATE_SHORE31
	(void)new TemplateTypeClass(Shore32);			// TEMPLATE_SHORE32
	(void)new TemplateTypeClass(Shore33);			// TEMPLATE_SHORE33
	(void)new TemplateTypeClass(Shore34);			//	TEMPLATE_SHORE34
	(void)new TemplateTypeClass(Shore35);			//	TEMPLATE_SHORE35
	(void)new TemplateTypeClass(Shore36);			//	TEMPLATE_SHORE36
	(void)new TemplateTypeClass(Shore37);			//	TEMPLATE_SHORE37
	(void)new TemplateTypeClass(Shore38);			//	TEMPLATE_SHORE38
	(void)new TemplateTypeClass(Shore39);			//	TEMPLATE_SHORE39
	(void)new TemplateTypeClass(Shore40);			//	TEMPLATE_SHORE40
	(void)new TemplateTypeClass(Shore41);			//	TEMPLATE_SHORE41
	(void)new TemplateTypeClass(ShoreCliff01);	//	TEMPLATE_SHORECLIFF01
	(void)new TemplateTypeClass(ShoreCliff02);	//	TEMPLATE_SHORECLIFF02
	(void)new TemplateTypeClass(ShoreCliff03);	//	TEMPLATE_SHORECLIFF03
	(void)new TemplateTypeClass(ShoreCliff04);	//	TEMPLATE_SHORECLIFF04
	(void)new TemplateTypeClass(ShoreCliff05);	//	TEMPLATE_SHORECLIFF05
	(void)new TemplateTypeClass(ShoreCliff06);	//	TEMPLATE_SHORECLIFF06
	(void)new TemplateTypeClass(ShoreCliff07);	//	TEMPLATE_SHORECLIFF07
	(void)new TemplateTypeClass(ShoreCliff08);	//	TEMPLATE_SHORECLIFF08
	(void)new TemplateTypeClass(ShoreCliff09);	//	TEMPLATE_SHORECLIFF09
	(void)new TemplateTypeClass(ShoreCliff10);	//	TEMPLATE_SHORECLIFF10
	(void)new TemplateTypeClass(ShoreCliff11);	//	TEMPLATE_SHORECLIFF11
	(void)new TemplateTypeClass(ShoreCliff12);	//	TEMPLATE_SHORECLIFF12
	(void)new TemplateTypeClass(ShoreCliff13);	//	TEMPLATE_SHORECLIFF13
	(void)new TemplateTypeClass(ShoreCliff14);	//	TEMPLATE_SHORECLIFF14
	(void)new TemplateTypeClass(ShoreCliff15);	//	TEMPLATE_SHORECLIFF15
	(void)new TemplateTypeClass(ShoreCliff16);	//	TEMPLATE_SHORECLIFF16
	(void)new TemplateTypeClass(ShoreCliff17);	//	TEMPLATE_SHORECLIFF17
	(void)new TemplateTypeClass(ShoreCliff18);	//	TEMPLATE_SHORECLIFF18
	(void)new TemplateTypeClass(ShoreCliff19);	//	TEMPLATE_SHORECLIFF19
	(void)new TemplateTypeClass(ShoreCliff20);	//	TEMPLATE_SHORECLIFF20
	(void)new TemplateTypeClass(ShoreCliff21);	//	TEMPLATE_SHORECLIFF21
	(void)new TemplateTypeClass(ShoreCliff22);	//	TEMPLATE_SHORECLIFF22
	(void)new TemplateTypeClass(ShoreCliff23);	//	TEMPLATE_SHORECLIFF23
	(void)new TemplateTypeClass(ShoreCliff24);	//	TEMPLATE_SHORECLIFF24
	(void)new TemplateTypeClass(ShoreCliff25);	//	TEMPLATE_SHORECLIFF25
	(void)new TemplateTypeClass(ShoreCliff26);	//	TEMPLATE_SHORECLIFF26
	(void)new TemplateTypeClass(ShoreCliff27);	//	TEMPLATE_SHORECLIFF27
	(void)new TemplateTypeClass(ShoreCliff28);	//	TEMPLATE_SHORECLIFF28
	(void)new TemplateTypeClass(Bridge1);			//	TEMPLATE_BRIDGE1
	(void)new TemplateTypeClass(Bridge1d);			//	TEMPLATE_BRIDGE1D
	(void)new TemplateTypeClass(Bridge2);			//	TEMPLATE_BRIDGE2
	(void)new TemplateTypeClass(Bridge2d);			//	TEMPLATE_BRIDGE2D
	(void)new TemplateTypeClass(Road01);			//	TEMPLATE_ROAD1
	(void)new TemplateTypeClass(Road02);			//	TEMPLATE_ROAD2
	(void)new TemplateTypeClass(Road03);			//	TEMPLATE_ROAD3
	(void)new TemplateTypeClass(Road04);			//	TEMPLATE_ROAD4
	(void)new TemplateTypeClass(Road05);			//	TEMPLATE_ROAD5
	(void)new TemplateTypeClass(Road06);			//	TEMPLATE_ROAD6
	(void)new TemplateTypeClass(Road07);			//	TEMPLATE_ROAD7
	(void)new TemplateTypeClass(Road08);			//	TEMPLATE_ROAD8
	(void)new TemplateTypeClass(Road09);			//	TEMPLATE_ROAD9
	(void)new TemplateTypeClass(Road10);			//	TEMPLATE_ROAD10
	(void)new TemplateTypeClass(Road11);			//	TEMPLATE_ROAD11
	(void)new TemplateTypeClass(Road12);			//	TEMPLATE_ROAD12
	(void)new TemplateTypeClass(Road13);			//	TEMPLATE_ROAD13
	(void)new TemplateTypeClass(Road14);			//	TEMPLATE_ROAD14
	(void)new TemplateTypeClass(Road15);			//	TEMPLATE_ROAD15
	(void)new TemplateTypeClass(Road16);			//	TEMPLATE_ROAD16
	(void)new TemplateTypeClass(Road17);			//	TEMPLATE_ROAD17
	(void)new TemplateTypeClass(Road18);			//	TEMPLATE_ROAD18
	(void)new TemplateTypeClass(Road19);			//	TEMPLATE_ROAD19
	(void)new TemplateTypeClass(Road20);			//	TEMPLATE_ROAD20
	(void)new TemplateTypeClass(Road21);			//	TEMPLATE_ROAD21
	(void)new TemplateTypeClass(Road22);			//	TEMPLATE_ROAD22
	(void)new TemplateTypeClass(Road23);			//	TEMPLATE_ROAD23
	(void)new TemplateTypeClass(Road24);			//	TEMPLATE_ROAD24
	(void)new TemplateTypeClass(Road25);			//	TEMPLATE_ROAD25
	(void)new TemplateTypeClass(Road26);			//	TEMPLATE_ROAD26
	(void)new TemplateTypeClass(Road27);			//	TEMPLATE_ROAD27
	(void)new TemplateTypeClass(Road28);			//	TEMPLATE_ROAD28
	(void)new TemplateTypeClass(Road29);			//	TEMPLATE_ROAD29
	(void)new TemplateTypeClass(Road30);			//	TEMPLATE_ROAD30
	(void)new TemplateTypeClass(Road31);			//	TEMPLATE_ROAD31
	(void)new TemplateTypeClass(Road32);			//	TEMPLATE_ROAD32
	(void)new TemplateTypeClass(Road33);			//	TEMPLATE_ROAD33
	(void)new TemplateTypeClass(Road34);			//	TEMPLATE_ROAD34
	(void)new TemplateTypeClass(Road35);			//	TEMPLATE_ROAD35
	(void)new TemplateTypeClass(Road36);			//	TEMPLATE_ROAD36

	/*
	**	Separate out the list of new operator calls. Watcom bombs
	**	if they are kept together.
	*/
	_Watcom_Ugh_Hack();
}


/***********************************************************************************************
 * TemplateTypeClass::Land_Type -- Determines land type from template and icon number.         *
 *                                                                                             *
 *    This routine will convert the specified icon number into the appropriate land type. The  *
 *    land type can be determined from the embedded colors in the "control template" section   *
 *    of the original art file. This control information is encoded into the icon data file    *
 *    to be retrieved and interpreted as the program sees fit. The engine only recognizes      *
 *    the first 16 colors as control colors, so the control map color value serves as an       *
 *    index into a simple lookup table.                                                        *
 *                                                                                             *
 * INPUT:   icon  -- The icon number within this template that is to be examined and used      *
 *                   to determine the land type.                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the land type that corresponds to the icon number specified.          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
LandType TemplateTypeClass::Land_Type(int icon) const
{
	IconsetClass const * icontrol = (IconsetClass const *)Get_Image_Data();

	if (icontrol != NULL) {
		unsigned char const * map = icontrol->Control_Map();
		if (map != NULL) {
			static LandType _land[16] = {
				LAND_CLEAR,
				LAND_CLEAR,
				LAND_CLEAR,
				LAND_CLEAR,			// Clear
				LAND_CLEAR,
				LAND_CLEAR,
				LAND_BEACH,			// Beach
				LAND_CLEAR,
				LAND_ROCK,			// Rock
				LAND_ROAD,			// Road
				LAND_WATER,			// Water
				LAND_RIVER,			//	River
				LAND_CLEAR,
				LAND_CLEAR,
				LAND_ROUGH,			// Rough
				LAND_CLEAR,
			};

			return(_land[map[icon % (icontrol->Map_Width() * icontrol->Map_Height())]]);
		}
	}
	return(LAND_CLEAR);
}


/***********************************************************************************************
 * TemplateTypeClass::From_Name -- Determine template from ASCII name.                         *
 *                                                                                             *
 *    This routine is used to determine the template number given only                         *
 *    an ASCII representation. The scenario loader uses this routine                           *
 *    to construct the map from the INI control file.                                          *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the ASCII name of the template.                                *
 *                                                                                             *
 * OUTPUT:  Returns with the template number. If the name had no match,                        *
 *          then returns with TEMPLATE_NONE.                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TemplateType TemplateTypeClass::From_Name(char const * name)
{
	if (name != NULL) {
		for (TemplateType index = TEMPLATE_FIRST; index < TEMPLATE_COUNT; index++) {
			if (stricmp(As_Reference(index).IniName, name) == 0) {
				return(index);
			}
		}
	}
	return(TEMPLATE_NONE);
}


/***********************************************************************************************
 * TemplateTypeClass::Occupy_List -- Determines occupation list.                               *
 *                                                                                             *
 *    This routine is used to examine the template map and build an                            *
 *    occupation list. This list is used to render a template cursor as                        *
 *    well as placement of icon numbers.                                                       *
 *                                                                                             *
 * INPUT:   placement   -- Is this for placement legality checking only? The normal condition  *
 *                         is for marking occupation flags.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the template occupation list.                            *
 *                                                                                             *
 * WARNINGS:   The return pointer is valid only until the next time that                       *
 *             this routine is called.                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *   12/12/1995 JLB : Optimized for direct access to iconset data.                             *
 *=============================================================================================*/
short const * TemplateTypeClass::Occupy_List(bool) const
{
	static short _occupy[13*8+5];
	short	* ptr;

	IconsetClass const * iconset = (IconsetClass const *)Get_Image_Data();
	unsigned char const * map = iconset->Map_Data();

	ptr = &_occupy[0];
	for (int index = 0; index < Width * Height; index++) {
		if (*map++ != 0xFF) {
			*ptr++ = (index % Width) + ((index / Width)*MAP_CELL_W);
		}
	}
	*ptr = REFRESH_EOL;

	return((short const *)&_occupy[0]);
}


/***********************************************************************************************
 * TemplateTypeClass::Init -- Loads graphic data for templates.                                *
 *                                                                                             *
 *    This routine loads the template graphic data for all the template                        *
 *    type supported for the specified theater. This routine is called                         *
 *    whenever the theater for the scenario is first determined.                               *
 *                                                                                             *
 * INPUT:   theater  -- The theater that the template data is to be                            *
 *                      loaded for.                                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine goes to disk!                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *   06/02/1994 JLB : Only handles iconset loading now (as it should).                         *
 *=============================================================================================*/
void TemplateTypeClass::Init(TheaterType theater)
{
	char fullname[_MAX_FNAME+_MAX_EXT];	// Fully constructed iconset name.
	void const * ptr;		// Working loaded iconset pointer.

	void *terrainPalette = (void *)MFCD::Retrieve(Theaters[theater].IsoPalette);

	for (TemplateType index = TEMPLATE_FIRST; index < TEMPLATE_COUNT; index++) {
		TemplateTypeClass	const & tplate = As_Reference(index);
// jmarshall - temp hack
		if (index > TEMPLATE_SHORE41)
			break;
// jmarshall end
		((void const *&)tplate.ImageData) = NULL;
		if (tplate.Theater & (1<<theater)) {
			_makepath(fullname, NULL, NULL, tplate.IniName, Theaters[theater].Suffix);
			ptr = MFCD::Retrieve(fullname);
			((void const *&)tplate.ImageData) = ptr;
#ifdef WIN32
			Register_Icon_Set((void*)ptr, TRUE);		//Register icon set for video memory caching
#endif

			((unsigned char &)tplate.Width) = Get_IconSet_MapWidth(ptr);
			((unsigned char &)tplate.Height) = Get_IconSet_MapHeight(ptr);

			// Try and load HD assets first.
			Image_t *hdImage = Load_StampHD(theater, fullname, tplate.ImageData);
			if (hdImage)
			{
				Get_Stamp_Size(tplate.ImageData, hdImage->renderwidth, hdImage->renderheight);
				((void const*&)tplate.HDImageData) = hdImage;
			}
			else
			{
				// If no HD assets, load the legacy assets into a texture.
				char tmp[512];
				sprintf(tmp, "icon_%s", fullname);
				((void const*&)tplate.HDImageData) = Load_Stamp(fullname, tplate.ImageData, terrainPalette);
			}
		}
	}
}


#ifdef SCENARIO_EDITOR
/***********************************************************************************************
 * TemplateTypeClass::Display -- Displays a generic representation of template.                *
 *                                                                                             *
 *    This routine is used to display a generic view of the template                           *
 *    object. This is necessary for selection in the scenario editor.                          *
 *                                                                                             *
 * INPUT:   x,y   -- The coordinates to center the display about.                              *
 *                                                                                             *
 *          window-- The window to base the coordinates upon.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TemplateTypeClass::Display(int x, int y, WindowNumberType window, HousesType ) const
{
	int	w,h;
	int	index;
	bool	scale;		// Should the template be half sized?

	Image_t* img = Get_HDImage_Data();

	w = Bound(img->width, 1, 13);
	h = Bound(img->height, 1, 8);
	//scale = (w > 3 || h > 3);
	//if (scale) {
	//	x -= (w * ICON_PIXEL_W) / 4;
	//	y -= (h * ICON_PIXEL_H) / 4;
	//} else {
	//	x -= (w * ICON_PIXEL_W) / 2;
	//	y -= (h * ICON_PIXEL_H) / 2;
	//}
	//x += WindowList[window][WINDOWX];
	//y += WindowList[window][WINDOWY];

	//IconsetClass const * iconset = (IconsetClass const *)Get_Image_Data();
	//unsigned char const * map = iconset->Map_Data();

	for (index = 0; index < img->isoTileInfo->NumTiles(); index++) {
		IsoTileImageHeader* isoImage = img->isoTileInfo->GetTileInfo(index);
		if (isoImage == NULL)
			continue;

		GL_RenderForeGroundImage(img, x + isoImage->TileX, y + isoImage->TileY, ICON_PIXEL_W, ICON_PIXEL_H, 0, index);	
	}
}


/***********************************************************************************************
 * TemplateTypeClass::Prep_For_Add -- Prepares to add template to scenario.                    *
 *                                                                                             *
 *    This routine prepares a list of template objects so that the                             *
 *    scenario editor can use this list to display a dialog box. The                           *
 *    selection of a template object will allow its placement upon the                         *
 *    map.                                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *   05/28/1994 JLB : Only handles real templates now.                                         *
 *   06/04/1994 JLB : Uses map editing interface functions.                                    *
 *=============================================================================================*/
void TemplateTypeClass::Prep_For_Add(void)
{
	for (TemplateType index = TEMPLATE_CLEAR1; index < TEMPLATE_COUNT; index++) {
// jmarshall
		if (index > TEMPLATE_SHORE41)
			return;
// jmarshall end
		if (As_Reference(index).Get_Image_Data()) {
			Map.Add_To_List(&As_Reference(index));
		}
	}
}
#endif


/***********************************************************************************************
 * TemplateTypeClass::Create_And_Place -- Creates and places a template object on the map.     *
 *                                                                                             *
 *    This support routine is used by the scenario editor to add a template object to the map  *
 *    and to the game.                                                                         *
 *                                                                                             *
 * INPUT:   cell  -- The cell to place the template object.                                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the template object placed successfully?                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TemplateTypeClass::Create_And_Place(CELL cell, HousesType ) const
{
	if (new TemplateClass(Type, cell)) {
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TemplateTypeClass::Create_One_Of -- Creates an object of this template type.                *
 *                                                                                             *
 *    This routine will create an object of this type. For certain template objects, such      *
 *    as walls, it is actually created as a building. The "building" wall is converted into    *
 *    a template at the moment of placing down on the map.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the appropriate object for this template type.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * TemplateTypeClass::Create_One_Of(HouseClass *) const
{
	return(new TemplateClass(Type, -1));
}


/***********************************************************************************************
 * TemplateTypeClass::One_Time -- Performs one-time initialization                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TemplateTypeClass::One_Time(void)
{
}


/***********************************************************************************************
 * TemplateTypeClass::As_Reference -- Fetches a reference to the template specified.           *
 *                                                                                             *
 *    This will return a reference to the TemplateTypeClass requested.                         *
 *                                                                                             *
 * INPUT:   type  -- The template type to fetch a reference to.                                *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the template type class specified.                     *
 *                                                                                             *
 * WARNINGS:   Be sure to pass a valid type parameter. This routine doesn't check it for       *
 *             legality.                                                                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TemplateTypeClass & TemplateTypeClass::As_Reference(TemplateType type)
{
	return(*TemplateTypes.Ptr(type));
}


COORDINATE TemplateTypeClass::Coord_Fixup(COORDINATE coord) const
{
	return Coord_Whole(coord);
}
