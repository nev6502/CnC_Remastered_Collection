// MapScript.cpp
//

#include "FUNCTION.H"
#include "MapScript.h"

/***********************************************************************************************
 * Script_GiveCredits - Gives a given player a given amount of credits                         *
 *                                                                                             *
 *   SCRIPT INPUT:	cashToGive (int) - The amount of cold hard credits to give the player      *
 *                                                                                             *
 *                	houseType (int)  - The player (house) index to give credits to             *
 *                                     or -1 to give to all players                            *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   6/11/2020 - JM Created                                                                    *
 *   6/12/2020 - JJ Added player (house) index input                                           *
 *=============================================================================================*/
static int Script_GiveCredits(lua_State* L) {
    
    int cashToGive = lua_tointeger(L, 1);

    int houseType = -1;

    // Optional houseType parameter
    if (lua_gettop(L) == 2) {
        houseType = lua_tointeger(L, 2);
    }

    for (int h_index = 0; h_index < Houses.Count(); h_index++) {

        HouseClass* house = Houses.Ptr(h_index);

        if (house->ID == houseType || houseType == -1) {
            house->Refund_Money(cashToGive);

            // Discontinue giving out free cash if a specific house was specified
            if (houseType >= 0) {
                break;
            }
        }

    }

    return 1;
}

/***********************************************************************************************
 * Script_NumBuildingTypeForPlayer - Returns the amount of specific buildings for a player     *
 *                                                                                             *
 *   SCRIPT INPUT:	structType (int) - The structure index to count buildings for              *
 *                                     or -1 for all structures                                *
 *                                                                                             *
 *                	houseType (int)  - The player (house) index to count buildings for         *
 *                	                   or -1 to for all players                                *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of matching buildings for that player        *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CountBuildings(lua_State* L) {
    int structType = lua_tointeger(L, 1); 
    
    int houseType = -1;

    // Optional houseType parameter
    if (lua_gettop(L) == 2) {
        houseType = lua_tointeger(L, 2);
    }
    
    int result = 0;
    for (int b_index = 0; b_index < Buildings.Count(); b_index++) {
        BuildingClass* building = Buildings.Ptr(b_index);

        if (building->Owner() == houseType || houseType == -1) {
            if (building->Class->Type == structType || structType == -1) {
                result++;
            }
        }
    }

    lua_pushnumber(L, result);
    return 1;
}


/***********************************************************************************************
 * Script_CountAircraft - Returns the amount of specific aircraft for a player                 *
 *                                                                                             *
 *   SCRIPT INPUT:	aircraftType (int) - The aircraft index to count aircraft for              *
 *                                       or -1 for all aircraft                                *
 *                                                                                             *
 *                	houseType (int)    - The player (house) index to count aircraft for        *
 *                                       or -1 for all players                                 *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of matching aircraft for that player         *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CountAircraft(lua_State* L) {
    int aircraftType = lua_tointeger(L, 1); 
    
    int houseType = -1;

    // Optional houseType parameter
    if (lua_gettop(L) == 2) {
        houseType = lua_tointeger(L, 2);
    }
    
    int result = 0;
    for (int a_index = 0; a_index < Aircraft.Count(); a_index++) {
        AircraftClass* aircraft = Aircraft.Ptr(a_index);

        if (aircraft->Owner() == houseType || houseType == -1) {
            if (aircraft->Class->Type == aircraftType || aircraftType == -1) {
                result++;
            }
        }
    }

    lua_pushnumber(L, result);
    return 1;
}

/***********************************************************************************************
 * Script_CountUnits - Returns the amount of specific units for a player                       *
 *                                                                                             *
 *   SCRIPT INPUT:	unitType (int)  - The unit index to count units for                        *
 *                                    or -1 for all units                                      *
 *                                                                                             *
 *                	houseType (int) - The player (house) index to count units for              *
 *                                    or -1 for all players                                    *
 *                                                                                             *
 *                  NOTE: -1 for either index input acts as ALL                                *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of matching units for that player            *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CountUnits(lua_State* L) {
    int unitType = lua_tointeger(L, 1); 
    
    int houseType = -1;

    // Optional houseType parameter
    if (lua_gettop(L) == 2) {
        houseType = lua_tointeger(L, 2);
    }
    
    int result = 0;
    for (int u_index = 0; u_index < Units.Count(); u_index++) {
        UnitClass* unit = Units.Ptr(u_index);

        if (unit->Owner() == houseType || houseType == -1) {
            if (unit->Class->Type == unitType || unitType == -1) {
                result++;
            }
        }
    }

    lua_pushnumber(L, result);
    return 1;
}


/***********************************************************************************************
 * Script_CountInfantry - Returns the amount of specific infantry for a player                 *
 *                                                                                             *
 *   SCRIPT INPUT:	infantryType (int) - The infantry index to count infantry for              *
 *                                       or -1 for all infantry                                *
 *                                                                                             *
 *                	houseType (int)    - The player (house) index to count infantry for        *
 *                                       or -1 for all players                                 *
 *                                                                                             *
 *                  NOTE: -1 for either index input acts as ALL                                *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of matching infantry for that player         *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CountInfantry(lua_State* L) {
    int infantryType = lua_tointeger(L, 1);

    int houseType = -1;

    // Optional houseType parameter
    if (lua_gettop(L) == 2) {
        houseType = lua_tointeger(L, 2);
    }
    
    int result = 0;
    for (int u_index = 0; u_index < Infantry.Count(); u_index++) {
        InfantryClass* infantry = Infantry.Ptr(u_index);

        if (infantry->Owner() == houseType || houseType == -1) {
            if (infantry->Class->Type == infantryType || infantryType == -1) {
                result++;
            }
        }
    }

    lua_pushnumber(L, result);
    return 1;
}


/***********************************************************************************************
 * Script_CountVessels - Returns the amount of specific vessels for a player                   *
 *                                                                                             *
 *   SCRIPT INPUT:	vesselType (int) - The vessel index to count vessels for                   *
 *                                     or -1 for all vessels                                   *
 *                                                                                             *
 *                	houseType (int) - The player (house) index to count vessels for            *
 *                                     or -1 for all players                                   *
 *                                                                                             *
 *                  NOTE: -1 for either index input acts as ALL                                *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of matching vessels for that player          *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CountVessels(lua_State* L) {
    int vesselType = lua_tointeger(L, 1);

    int houseType = -1;

    // Optional houseType parameter
    if (lua_gettop(L) == 2) {
        houseType = lua_tointeger(L, 2);        
    }

    int result = 0;
    for (int u_index = 0; u_index < Vessels.Count(); u_index++) {
        VesselClass* vessel = Vessels.Ptr(u_index);

        if (vessel->Owner() == houseType || houseType == -1) {
            if (vessel->Class->Type == vesselType || vesselType == -1) {
                result++;
            }
        }
    }

    lua_pushnumber(L, result);
    return 1;
}


/***********************************************************************************************
 * Script_SetTriggerCallback - Adds a lua callback to an existing trigger                      *
 *                                                                                             *
 *   SCRIPT INPUT:	triggerName (string)     - The ININame of the trigger via its class        *
 *                                                                                             *
 *                	callbackName (string)    - The function name to be called when the         *
 *                                             actions have been met                           *
 *                                                                                             *
 *                  actionIndex (int) (opt.) - 0: always callback on any of the given paths    *
 *                                                of the trigger                               *
 *                                             1: only callback on first action                *
 *                                             2: only callback on second action               *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of matching buildings for that player        *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_SetTriggerCallback(lua_State* L) {

    const char* triggerName = lua_tostring(L, 1);
    const char* callbackName = lua_tostring(L, 2);
    char actionIndex = 0;

    // Optional actionIndex parameter
    if (lua_gettop(L) == 3) {
        actionIndex = lua_tointeger(L, 2);
    }

    // Find the trigger and set up callback
    for (int t_index = 0; t_index < Triggers.Count(); t_index++) {
        TriggerClass* trigger = Triggers.Ptr(t_index);

        if (trigger != NULL) {
            
            if (strcmp(trigger->Name(), triggerName) == 0) {

                strncpy(trigger->MapScriptCallback,callbackName,sizeof(trigger->MapScriptCallback ) - 1);
                trigger->MapScriptActionIndex = actionIndex;

                break;
            }

        }
        
    }

    return 1;
}



/***********************************************************************************************
 * Script_Win - The specified player wins                                                      *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int) - The player (house that wins)                             *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_Win(lua_State* L) {
    
    int houseType = lua_tointeger(L, -1);

    if (houseType != HOUSE_NONE) {

        HouseClass* this_house = HouseClass::As_Pointer((HousesType)houseType);

        if (this_house != NULL) {

            if (this_house->ID == PlayerPtr->Class->House) {
                PlayerPtr->Flag_To_Win();
            }
            else {
                PlayerPtr->Flag_To_Lose();
            }

            bool success = this_house->Fire_Sale();
            lua_pushboolean(L, int(success));
        }
    }

    return 1;
}

/***********************************************************************************************
 * Script_Lose - The specified player loses                                                    *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The waypoint index of which to reveal                          *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_Lose(lua_State* L) {

    int houseType = lua_tointeger(L, -1);

    if (houseType != HOUSE_NONE) {

        HouseClass* this_house = HouseClass::As_Pointer((HousesType)houseType);

        if (this_house != NULL) {

            if (this_house->ID == PlayerPtr->Class->House) {
                PlayerPtr->Flag_To_Lose();
            }
            else {
                PlayerPtr->Flag_To_Win();
            }

            bool success = this_house->Fire_Sale();
            lua_pushboolean(L, int(success));
        }
    }

    return 1;
}


/***********************************************************************************************
 * Script_BeginProduction - This will enable production to begin for the house specified       *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int) - The AI player (house) index to allow winning for         *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_BeginProduction(lua_State* L) {

    int houseType = lua_tointeger(L, -1);

    if (houseType != HOUSE_NONE) {

        HouseClass* this_house = HouseClass::As_Pointer((HousesType)houseType);

        if (this_house != NULL) {
            this_house->Begin_Production();
        }
    }

    return 1;
}


/***********************************************************************************************
 * Script_CreateTeam - Attempts to create team (as defined in map)                             *
 *                                                                                             *
 *   SCRIPT INPUT:	teamType (int) - The team index to attempt to create                       *
 *                                                                                             *
 *   SCRIPT OUTPUT:  success (bool) - Did a team get created?                                  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CreateTeam(lua_State* L) {

    const char* teamType = lua_tostring(L, 1);

    TeamTypeClass* teamPtr = TeamTypeClass::From_Name(teamType);

    if (teamPtr != NULL) {
        ScenarioInit++;

        bool success = false;

        TeamClass* new_team = teamPtr->Create_One_Of();

        if (new_team != NULL) {
            success = true;
        }

        lua_pushboolean(L, int(success));

        ScenarioInit--;
    }

    return 1;
}


/***********************************************************************************************
 * Script_DestroyTeam - Destroy all teams of the type specified                                *
 *                                                                                             *
 *   SCRIPT INPUT:	teamType (int) - The team index to attempt to destroy                      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_DestroyTeam(lua_State* L) {

    const char* teamType = lua_tostring(L, 1);

    TeamTypeClass* teamPtr = TeamTypeClass::From_Name(teamType);

    if (teamPtr != NULL) {

        teamPtr->Destroy_All_Of();

    }

    return 1;
}


/***********************************************************************************************
 * Script_AllHunt - Force all units of specified house to go into hunt mode                    *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int) - The AI player (house) index to allow winning for         *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_AllHunt(lua_State* L) {

    int houseType = lua_tointeger(L, -1);

    if (houseType != HOUSE_NONE) {

        HouseClass* this_house = HouseClass::As_Pointer((HousesType)houseType);

        if (this_house != NULL) {
            this_house->Do_All_To_Hunt();
        }
    }

    return 1;
}


/***********************************************************************************************
 * Script_Reinforcements - Attempts to create reinforcements as defined by team                *
 *                                                                                             *
 *   SCRIPT INPUT:	teamType (int) - The team index to attempt to create                       *
 *                                                                                             *
 *   SCRIPT OUTPUT:  success (bool) - Did Do_Reinforcements return true?                       *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_Reinforcements(lua_State* L) {

    const char* teamType = lua_tostring(L, 1);

    TeamTypeClass* teamPtr = TeamTypeClass::From_Name(teamType);

    if (teamPtr != NULL) {
        bool success = Do_Reinforcements(teamPtr);
        lua_pushboolean(L, int(success));
    }
    else {
        lua_pushboolean(L, 0);
    }



    return 1;
}


/***********************************************************************************************
 * Script_DropZoneFlare - Places drop down smoke at specified waypoint location                *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The waypoint index of which to drop the smoke                  *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_DropZoneFlare(lua_State* L) {
    int ret = lua_tointeger(L, -1);

    if (ret > 0 && ret < sizeof(Scen.Waypoint)) {
        new AnimClass(ANIM_LZ_SMOKE, Cell_Coord(Scen.Waypoint[ret]));
    }

    return 1;
}


/***********************************************************************************************
 * Script_FireSale - Make AI house give up, selling everything and going all in on attack      *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int) - The AI player (house) index to allow winning for         *
 *                                                                                             *
 *   SCRIPT OUTPUT:  success (bool) - Did the action get performed?                            *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_FireSale(lua_State* L) {

    int houseType = lua_tointeger(L, -1);

    if (houseType != HOUSE_NONE) {

        HouseClass* this_house = HouseClass::As_Pointer((HousesType)houseType);

        if (this_house != NULL) {
            bool success = this_house->Fire_Sale();
            lua_pushboolean(L, int(success));
        }
    }

    return 1;
}


/***********************************************************************************************
 * Script_PlayMovie - Plays the given movie                                                    *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The movie index of which to play                               *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_PlayMovie(lua_State* L) {
    int ret = lua_tointeger(L, -1);
    Play_Movie((VQType)ret);
    return 1;
}


/***********************************************************************************************
 * Script_TriggerText - Triggers text to display in-game                                       *
 *                                                                                             *
 *   SCRIPT INPUT:	textIndex (int) - The text index to display (from tutorial.ini)            *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_TriggerText(lua_State* L) {
    int textIndex = lua_tointeger(L, -1);

    // TODO: Implement colors / flags

    // JJQUESTION: This appears right but doesn't do anything. Is text broken at the moment?
    if (textIndex > 0 && textIndex < sizeof(TutorialText)) {

        Session.Messages.Add_Message(NULL, textIndex, (char*)TutorialText[textIndex], PCOLOR_GREEN, TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE);
    }

    return 1;
}


/***********************************************************************************************
 * Script_DestroyTrigger - Reveals the entire map                                              *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The waypoint index of which to reveal                          *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_DestroyTrigger(lua_State* L) {
    if (!PlayerPtr->IsVisionary) {
        PlayerPtr->IsVisionary = true;
        for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
            Map.Map_Cell(cell, PlayerPtr);
        }
    }

    return 1;
}

/***********************************************************************************************
 * Script_AutoCreate - Allows house AI to begin creating autocreate teams                      *
 *                                                                                             *
 *  Autocreation means that instead of a teamtype being                                        *
 *  manually created by a Create Team trigger action, that                                     *
 *  it will be subject to the autocreate function. Each                                        *
 *  computer country can have its autocreation function                                        *
 *  turned on through a trigger action, after which the                                        *
 *  creation of the teamtypes that have autocreation turned                                    *
 *  on will be subject to being created, at the whim of the                                    *
 *  autocreate function. See [TeamTypes] Section for more                                      *
 *  details.                                                                                   *
 *                        - The Red Alert Single Player Mission Creation Guide                 *
 *                                                                                             *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int) - The AI player (house) index to allow autocreate for      *
 *                  createEnables (bool, default yes) - Allow autocreate on/off                *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_AutoCreate(lua_State* L) {

    int houseType = 0;
    bool autoCreateEnabled = true;

    houseType = lua_tointeger(L, 1);

    // If there is a second argument given, process it
    if (lua_gettop(L) == 2) {
        autoCreateEnabled = lua_toboolean(L, 2);
    }

    if (houseType != HOUSE_NONE) {

        HouseClass* this_house = HouseClass::As_Pointer((HousesType)houseType);

        if (this_house != NULL) {

            // If we are enabling auto creation by the house, clear alert
            if (autoCreateEnabled) {
                this_house->AlertTime = 0; // TODO - don't assume successful pointer
            }

            // Now set the parameter
            this_house->IsAlerted = autoCreateEnabled;

        }

    }

    return 1;
}

/***********************************************************************************************
 * Script_AllowWin - Allows a win                                                              *
 *                                                                                             *
 *   Used when you want a specific objective                                                   *
 *   to be met before the mission is accomplished.  In other words, if the                     *
 *   trigger with this action assigned to it hasn't been fired, but the                        *
 *   actual WIN trigger has been fired, the mission will not end until this                    *
 *   "allow win" trigger is fired.                                                             *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int) - The AI player (house) index to allow winning for         *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_AllowWin(lua_State* L) {

    int houseType = lua_tointeger(L, -1);

    if (houseType != HOUSE_NONE) {

        HouseClass* this_house = HouseClass::As_Pointer((HousesType)houseType);

        if (this_house != NULL) {
            this_house->Blockage++;
        }
    }

    return 1;
}


/***********************************************************************************************
 * Script_RevealAll - Reveals the entire map  TODO: input player                               *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The waypoint index of which to reveal                          *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_RevealAll(lua_State* L) {
    if (!PlayerPtr->IsVisionary) {
        PlayerPtr->IsVisionary = true;
        for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
            Map.Map_Cell(cell, PlayerPtr);
        }
    }

    return 1;
}

/***********************************************************************************************
 * Script_RevealCell - Reveals map around given waypoint  TODO: input player                   *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The waypoint index of which to reveal                          *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_RevealCell(lua_State* L) {
    int ret = lua_tointeger(L, -1);

    if (!PlayerPtr->IsVisionary) {
        Map.Sight_From(Scen.Waypoint[ret], Rule.GapShroudRadius, PlayerPtr, false);
    }

    return 1;
}

/***********************************************************************************************
 * Script_RevealZone - Reveals area of map around given waypoint  TODO: input player           *
 *                                                                                             *
 *   Reveal all cells of the zone that the specified waypoint is located in. This can be       *
 *   used to reveal whole islands or bodies of water                                           *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The waypoint index of which to reveal                          *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_RevealZone(lua_State* L) {
    int ret = lua_tointeger(L, -1);

    if (!PlayerPtr->IsVisionary) {
        int zone = Map[Scen.Waypoint[ret]].Zones[MZONE_CRUSHER];

        for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
            if (Map[cell].Zones[MZONE_CRUSHER] == zone) {
                Map.Map_Cell(cell, PlayerPtr);
            }
        }

    }

    return 1;
}


/***********************************************************************************************
 * Script_PlaySound - Plays the given Sound                                                    *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The sound index of which to play                               *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_PlaySound(lua_State* L) {
    int ret = lua_tointeger(L, -1);

    Sound_Effect((VocType)ret);

    return 1;
}


/***********************************************************************************************
 * Script_PlayMusic - Plays the given Music score.                                             *
 *                                                                                             *
 *   Plays music score at given index. Negative numbers stop any music playing.                *
 *                                                                                             *
 *     SCRIPT INPUT:	ret (int) - The music index of which to play                           *
 *                                                                                             *
 *     SCRIPT OUTPUT:  void                                                                    *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_PlayMusic(lua_State* L) {
    int ret = lua_tointeger(L, -1);

    if (ret < 0) {
        Theme.Stop();
    }
    else {
        Theme.Play_Song((ThemeType)ret);
    }

    return 1;
}


/***********************************************************************************************
 * Script_PlaySpeech - Plays the given Speech audio                                            *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The speech index of which to play                              *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_PlaySpeech(lua_State* L) {
    int ret = lua_tointeger(L, -1);
    Speak((VoxType)ret);
    return 1;
}



/***********************************************************************************************
 * Script_StartMissionTimer - Starts the in-game mission timer                                 *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - amount of time in tenths of a minute                           *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_StartMissionTimer(lua_State* L) {
    int ret = lua_tointeger(L, -1);
    Scen.MissionTimer = Scen.MissionTimer + (ret * (TICKS_PER_MINUTE / 10));
    Scen.MissionTimer.Start();
    Map.Redraw_Tab();
    return 1;
}

/***********************************************************************************************
 * Script_StopMissionTimer - Stops the in-game mission timer                                   *
 *                                                                                             *
 *   SCRIPT INPUT:	none                                                                       *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_StopMissionTimer(lua_State* L) {
    Scen.MissionTimer.Start();
    return 1;
}


/***********************************************************************************************
 * Script_SetBriefingText - Sets the text on the mission briefing screen                       *
 *                                                                                             *
 *   SCRIPT INPUT:	(char*); The input text                                                    *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_SetBriefingText(lua_State* L) {
    strcpy(Scen.BriefingText, lua_tostring(L, -1));
    return 1;
}


/***********************************************************************************************
 * MapScript::CallFunction - Calls a given function within the current script                  *
 *                                                                                             *
 * INPUT:  functionName - The name of the function to call                                     *
 *                                                                                             *
 * OUTPUT:  void                                                                               *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
void MapScript::CallFunction(const char* functionName) {

    lua_getglobal(L, functionName);
    lua_pcall(L, 0, 0, 0);
    
} //BriefingText


/***********************************************************************************************
 * MapScript::setLuaPath - Sets Lua's path / extensions to search for scripts                  *
 *                                                                                             *
 *   This helps Lua know where/how to look for files when using require(), otherwise we could  *
 *   hard code path/extension as we did previously                                             *
 *                                                                                             *
 * INPUT:  pathvalue - See lua documentation RE: LUA_PATH to know how to deal with this        *
 *                                                                                             *
 * OUTPUT:  void                                                                               *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
void MapScript::SetLuaPath( const char* input_path)
{

    // Needs space to keep the initial paths
    char new_path[1024];

    // Set new GLOBALS.package.path
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "path"); // get field "path" from table at top of stack (-1)
    const char* cur_path = lua_tostring(L, -1); // grab path string from top of stack

    sprintf(new_path, "%s;%s", cur_path, input_path);

    lua_pop(L, 1);
    lua_pushstring(L, new_path);
    lua_setfield(L, -2, "path"); 
    lua_pop(L, 1);
}



/***********************************************************************************************
 * MapScript::Init -- Loads and initializes script for a given map                             *
 *                                                                                             *
 *    This routine is called at the beginning of a mission                                     *
 *                                                                                             *
 * INPUT:  mapName - The script path                                                           *
 *                                                                                             *
 * OUTPUT:  bool; did the map script load and initialize?                                      *
 *                                                                                             *
 * WARNINGS:  none                                                                             *
 *                                                                                             *
 *=============================================================================================*/
bool MapScript::Init(const char* mapName) {
    char scriptName[256];

    // Build map path
    sprintf(scriptName, "scripts/%s.script", mapName);
    L = luaL_newstate();

    luaL_openlibs(L);

    // Load file
    if (luaL_loadfile(L, scriptName)){
        L = NULL;
        return false;
    }

    // TODO: I've tried several combinations of paths (including "scripts/?"), 
    // but I can only get "require 'basefilename'" to work within a 
    // script, not "require 'basefilename.script'". This may be completely 
    // normal within lua, but it seems odd.

    // This allows require() to work
    SetLuaPath(";scripts/?.script;");

    if (lua_pcall(L, 0, 0, 0)) {
        L = NULL;
        return false;
    }

    /**********************************************************************************************
    * Red Alert Vanilla Actions                                                                   *
    *=============================================================================================*/

    lua_register(L, "Win", Script_Win);                                             // player wins!
    lua_register(L, "Lose", Script_Lose);                                           // player loses.
    lua_register(L, "BeginProduction", Script_BeginProduction);                     // computer begins factory production.
    lua_register(L, "CreateTeam", Script_CreateTeam);                               // computer creates a certain type of team
    lua_register(L, "DestroyTeam", Script_DestroyTeam);                             // destroy team (comment was redacted. was something offensive originally written?)
    lua_register(L, "AllHunt", Script_AllHunt);                                     // all enemy units go into hunt mode (teams destroyed).
    lua_register(L, "Reinforcements", Script_Reinforcements);                       // player gets reinforcements (house that gets them is determined by the Reinforcement instance)
    lua_register(L, "DropZoneFlare", Script_DropZoneFlare);                         // Deploy drop zone smoke.
    lua_register(L, "FireSale", Script_FireSale);	                                // Sell all buildings and go on rampage.
    lua_register(L, "PlayMovie", Script_PlayMovie);							        // Play movie (temporarily suspend game).
    lua_register(L, "TriggerText", Script_TriggerText);                             // Triggers a text message display.
    lua_register(L, "DestroyTrigger", Script_DestroyTrigger);                       // Destroy specified trigger.
    lua_register(L, "AutoCreate", Script_AutoCreate);                               // Computer to autocreate teams.
                                                                                    // Win if captured, lose if destroyed ( function does not exist )
    lua_register(L, "AllowWin", Script_AllowWin);                                   // Allows winning if triggered.

    lua_register(L, "RevealAll", Script_RevealAll);                                 // Reveal the entire map.
    lua_register(L, "RevealCell", Script_RevealCell);                               // Reveal map around cell #.
    lua_register(L, "RevealZone", Script_RevealZone);                               // Reveal all of specified zone.
    lua_register(L, "PlaySound", Script_PlaySound);                                 // Play sound effect.
    lua_register(L, "PlayMusic", Script_PlayMusic);                                 // Play musical score.
    lua_register(L, "PlaySpeech", Script_PlaySpeech);						        // Play EVA speech.
        // TODO: Force trigger to activate.
    lua_register(L, "StartMissionTimer", Script_StartMissionTimer);			        // Start mission timer.
    lua_register(L, "StopMissionTimer", Script_StopMissionTimer);			        // Stop mission timer.


/**********************************************************************************************
* Red Alert Vanilla Events                                                                    *
*=============================================================================================*/

// player enters this square
// Spied by.
// Thieved by (raided or stolen vehicle).
// player discovers this object
// House has been discovered.
// player attacks this object
// player destroys this object
// Any object event will cause the trigger.
// all house's units destroyed
// all house's buildings destroyed
// all house's units & buildings destroyed
// house reaches this many credits
// Scenario elapsed time from start.
// Pre expired mission timer.
// Number of buildings destroyed.
// Number of units destroyed.
// No factories left.
// Civilian has been evacuated.
// Specified building has been built.
// Specified unit has been built.
// Specified infantry has been built.
// Specified aircraft has been built.
// Specified team member leaves map.
// Enters same zone as waypoint 'x'.
// Crosses horizontal trigger line.
// Crosses vertical trigger line.
// If specified global has been set.
// If specified global has been cleared.
// If all fake structures are gone.
// When power drops below 100%.
// All bridges destroyed.
// Check for building existing.


/**********************************************************************************************
* Utility Functions                                                                           *
*=============================================================================================*/

    lua_register(L, "SetBriefingText", Script_SetBriefingText);						// Sets the [text] on the mission briefing screen
    lua_register(L, "GiveCredits", Script_GiveCredits);                             // Give [credits] to [player] 
    
    lua_register(L, "CountBuildings", Script_CountBuildings);	                    // Number of buildings of [type] for given [player]
    lua_register(L, "CountAircraft", Script_CountAircraft);	                        // Number of units of [type] for given [player]
    lua_register(L, "CountUnits", Script_CountUnits);	                            // Number of units of [type] for given [player]
    lua_register(L, "CountInfantry", Script_CountInfantry);	                        // Number of infantry of [type] for given [player]
    lua_register(L, "CountVessels", Script_CountVessels);	                        // Number of vessels of [type] for given [player]

    lua_register(L, "SetTriggerCallback", Script_SetTriggerCallback);	            // Initiates a given [callback] on an existing [trigger]

    
/**********************************************************************************************
* Script Globals                                                                              *
*=============================================================================================*/

    lua_pushnumber(L, PlayerPtr->ID);                                                // Local player (house) index
    lua_setglobal(L, "_localPlayer");


    return true;
}

