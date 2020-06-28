// mapscript.h
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

// Stores defaults of rules from before
// variables were changed within
struct MapScriptDefaultAttribute {
    RTTIType RTTI; // The type of object
    int ID=-1; // The index of the object in its heap
    int attribute;
    int value;
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
	std::vector<MapScriptDefaultAttribute*> DefaultTypeAttributeCache;

private:
	lua_State* L;
};



// Enums
typedef enum MapScriptAttributeType : char {

    // Techno types
    ATTRIBUTE_TYPE=0,
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
    ATTRIBUTE_CRUSHABLE,
    ATTRIBUTE_STEALTHY,
    ATTRIBUTE_SELECTABLE,
    ATTRIBUTE_LEGAL_TARGET,
    ATTRIBUTE_REPAIRABLE,
    ATTRIBUTE_SELF_HEALING,
    ATTRIBUTE_EXPLODES,
    ATTRIBUTE_HAS_CREW,
    ATTRIBUTE_MAX_PASSENGERS,
    ATTRIBUTE_IS_REPAIRING,

    // Building Specific
    ATTRIBUTE_CAPACITY,
    ATTRIBUTE_POWER,
    ATTRIBUTE_SELLABLE,

    // Unit Specific
    ATTRIBUTE_CRATE_GOODIE,
    ATTRIBUTE_CRUSHER,
    ATTRIBUTE_HARVEST,
    ATTRIBUTE_RADAR_EQUIPPED,
    ATTRIBUTE_JAMMER,
    ATTRIBUTE_GAPPER,

    // Infantry Specific
    ATTRIBUTE_FEMALE,
    ATTRIBUTE_CAPTURE,
    ATTRIBUTE_FRAIDY_CAT,
    ATTRIBUTE_CIVILIAN,
    ATTRIBUTE_BOMBER,
    ATTRIBUTE_DOG


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

void Script_CacheDefaultAttribute(RTTIType input_rtti, int input_id, MapScriptAttributeType input_attribute, int input_value);

#endif