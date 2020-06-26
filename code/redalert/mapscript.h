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



// Enums
typedef enum MapScriptAttributeType : char {
    ATTRIBUTE_NONE=0,
    ATTRIBUTE_STRENGTH,
    ATTRIBUTE_MAX_STRENGTH,
    ATTRIBUTE_COST,
    ATTRIBUTE_SIGHT_RANGE,
    ATTRIBUTE_TECH_LEVEL,
    ATTRIBUTE_ARMOR,
    ATTRIBUTE_PRIMARY_WEAPON,
    ATTRIBUTE_SECONDARY_WEAPON,
    ATTRIBUTE_SPEED,
    ATTRIBUTE_MAX_SPEED,
    ATTRIBUTE_MAX_AMMO,
    ATTRIBUTE_ROTATION_SPEED,
    ATTRIBUTE_IS_CRUSHABLE,
    ATTRIBUTE_IS_STEALTHY,
    ATTRIBUTE_IS_SELECTABLE,
    ATTRIBUTE_IS_LEGAL_TARGET,
    ATTRIBUTE_IS_REPAIRABLE,
    ATTRIBUTE_IS_SELF_HEALING,
    ATTRIBUTE_IS_EXPLODING,
    ATTRIBUTE_HAS_CREW,
    ATTRIBUTE_MAX_PASSENGERS
} MapScriptAttributeType;

// Non-Class-Functions
bool Script_SetObjectTrigger(int input_object_id, TriggerClass* input_trigger);
ObjectClass* Script_GetCacheObject(int input_object_id);

int Script_BuildingIndexFromID(int input_object_id);
int Script_UnitIndexFromID(int input_object_id);
int Script_AircraftIndexFromID(int input_object_id);
int Script_InfantryIndexFromID(int input_object_id);
int Script_VesselIndexFromID(int input_object_id);

static int Script_SetObjectTypeAttribute(lua_State* L, TechnoTypeClass* this_type_class, MapScriptAttributeType attribute_type);


#endif