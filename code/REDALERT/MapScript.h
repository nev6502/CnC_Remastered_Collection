// MapScript.h
//

#ifndef MAPSCRIPT_H
#define MAPSCRIPT_H

// Lua is written in C, so compiler needs to know how to link its libraries
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}


// Object Cache Item
// Stores an id lookup table
struct MapScriptObject {
	int ID=-1; // The object's "Global ID"
	int classIndex=-1; // The object's "Global ID"
	RTTIType RTTI; // The type of object
};


//
// MapScript Class
//
class MapScript {
public:


	~MapScript() { Deinit(); };

	
	bool Init(const char* mapName);
	void Deinit();

	void CallFunction(const char* functionName);
	void SetLuaPath(const char* input_path);

	std::vector<MapScriptObject*> ObjectCache;

private:
	lua_State* L;
};

// Non-Class-Functions
bool Script_SetObjectTrigger(int input_object_id, TriggerClass* input_trigger);
ObjectClass* Script_GetCacheObject(int input_object_id);

int Script_BuildingIndexFromID(int input_object_id);
int Script_UnitIndexFromID(int input_object_id);
int Script_AircraftIndexFromID(int input_object_id);
int Script_InfantryIndexFromID(int input_object_id);
int Script_VesselIndexFromID(int input_object_id);

#endif