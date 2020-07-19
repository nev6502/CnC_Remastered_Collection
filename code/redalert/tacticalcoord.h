// tacticalcoord.h
//

struct DisplayTacticalCoord {
	DisplayTacticalCoord();
	DisplayTacticalCoord(COORDINATE coord);

	operator DisplayTacticalCoord::COORDINATE() const { return GetCoordinate(); }
	COORDINATE GetCoordinate() const;

	void SetFromCoordinate(COORDINATE coord);

	int x;
	int y;
};