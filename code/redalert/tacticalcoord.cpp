// tacticalcoord.cpp
//

#include "function.h"

DisplayTacticalCoord::DisplayTacticalCoord() {
	x = 0;
	y = 0;
}

DisplayTacticalCoord::DisplayTacticalCoord(COORDINATE coord)
{
	SetFromCoordinate(coord);
}

COORDINATE DisplayTacticalCoord::GetCoordinate() const
{
	return XY_Coord(x, y);
}

void DisplayTacticalCoord::SetFromCoordinate(COORDINATE coord)
{
	//Map.Coord_To_Pixel(coord, x, y);
	x = Coord_X(coord);
	y = Coord_Y(coord);
}