// BDAD_AF.H
//

// Building Data for Red Alert Allied Force.

static BuildingTypeClass const ClassAFV01(
	STRUCT_AFV01,
	TXT_CIV1,						// NAME:			Short name of the structure.
	"afV01",						// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0, 0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
	false,						// Is this building a fake (decoy?)
	true,							// Animation rate is regulated for constant speed?
	true,							// Always use the given name for the building?
	false,						// Is this a wall type structure?
	true,							// Simple (one frame) damage imagery?
	true,							// Is it invisible to radar?
	true,							// Can the player select this?
	true,							// Is this a legal target for attack or move?
	true,							// Is this an insignificant building?
	true,							// Theater specific graphic image?
	false,						// Does it have a rotating turret?
	false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE: Building size.
	NULL,								// Preferred exit cell list.
	(short const*)List0011,	// OCCUPYLIST:	List of active foundation squares.
	(short const*)List1100		// OVERLAPLIST:List of overlap cell offset.
);