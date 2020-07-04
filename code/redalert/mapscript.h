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

    int UID; // The UID of the object (to prevent ID collisions)

	int ID=-1; // The object's "Global ID"
	int classIndex=-1; // The object's "Global ID"
	RTTIType RTTI; // The type of object

};

// Stores defaults of object attributes whenever they are changed for the first time (for later restoration)
struct MapScriptDefaultObjectAttribute {
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

    int cUID = 0;

	~MapScript() { Deinit(); };

	
	bool Init(const char* mapName);
	void Deinit();

    MapScriptObject* CreateCacheObject();

	void CallFunction(const char* functionName, int input_args=0, ...);
	void SetLuaPath(const char* input_path);

	std::vector<MapScriptObject*> ObjectCache;
	std::vector<MapScriptDefaultObjectAttribute*> DefaultObjectTypeAttributeCache;

private:
	lua_State* L;
};



// Enums
typedef enum MapScriptObjectAttributeType : char {

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


} MapScriptObjectAttributeType;

// Enums
typedef enum MapScriptHouseAttributeType : char {

    // Techno types
    HOUSE_ATTRIBUTE_IQ = 0,
    HOUSE_ATTRIBUTE_TECH_LEVEL,
    HOUSE_ATTRIBUTE_IS_HUMAN,
    HOUSE_ATTRIBUTE_FIREPOWER_BIAS,
    HOUSE_ATTRIBUTE_GROUNDSPEED_BIAS,
    HOUSE_ATTRIBUTE_ARMOR_BIAS,
    HOUSE_ATTRIBUTE_ROF_BIAS,
    HOUSE_ATTRIBUTE_COST_BIAS,
    HOUSE_ATTRIBUTE_REPAIR_DELAY,
    HOUSE_ATTRIBUTE_BUILD_DELAY,
    HOUSE_ATTRIBUTE_STARTED,
    HOUSE_ATTRIBUTE_ALERTED,
    HOUSE_ATTRIBUTE_BASE_BUILDING,
    HOUSE_ATTRIBUTE_DISCOVERED,
    HOUSE_ATTRIBUTE_CAPACITY,
    HOUSE_ATTRIBUTE_TIBERIUM,
    HOUSE_ATTRIBUTE_CREDITS,
    HOUSE_ATTRIBUTE_ALERT_TIMER,
    HOUSE_ATTRIBUTE_REPAIR_TIMER,
    HOUSE_ATTRIBUTE_POINTS,
    HOUSE_ATTRIBUTE_DESTROYED_BUILDINGS,
    HOUSE_ATTRIBUTE_DESTROYED_VEHICLES, 
    HOUSE_ATTRIBUTE_DESTROYED_INFANTRY,
    HOUSE_ATTRIBUTE_DESTROYED_AIRCRAFT,
    HOUSE_ATTRIBUTE_DESTROYED_VESSELS,
    HOUSE_ATTRIBUTE_CAPTURED_BUILDINGS,
    HOUSE_ATTRIBUTE_CRATES,
    HOUSE_ATTRIBUTE_BUILDING_FACTORIES,
    HOUSE_ATTRIBUTE_UNIT_FACTORIES, 
    HOUSE_ATTRIBUTE_INFANTRY_FACTORIES,
    HOUSE_ATTRIBUTE_AIRCRAFT_FACTORIES,
    HOUSE_ATTRIBUTE_VESSEL_FACTORIES,
    HOUSE_ATTRIBUTE_POWER,
    HOUSE_ATTRIBUTE_DRAIN,
    HOUSE_ATTRIBUTE_LAST_ATTACKER

} MapScriptHouseAttributeType;

// Non-Class-Functions
bool Script_SetObjectTrigger(int input_object_id, TriggerClass* input_trigger);
ObjectClass* Script_GetCacheObject(int input_object_id);
void Script_DeleteCacheObject(int input_object_id);

int Script_BuildingIndexFromID(int input_object_id);
int Script_UnitIndexFromID(int input_object_id);
int Script_AircraftIndexFromID(int input_object_id);
int Script_InfantryIndexFromID(int input_object_id);
int Script_VesselIndexFromID(int input_object_id);

static int Script_SetObjectTypeAttribute(lua_State* L, TechnoTypeClass* this_type_class, MapScriptObjectAttributeType attribute_type);
static int Script_GetObjectTypeAttribute(lua_State* L, TechnoTypeClass* this_type_class, MapScriptObjectAttributeType attribute_type);

void Script_CacheDefaultObjectTypeAttribute(RTTIType input_rtti, int input_id, MapScriptObjectAttributeType input_attribute, int input_value);

#endif