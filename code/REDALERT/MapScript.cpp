// MapScript.cpp
//

#include "FUNCTION.H"
#include "MapScript.h"

/***********************************************************************************************
 * Script_GiveCreditsToPlayer - Gives a given player a given amount of credits                 *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int) - The player (house) index to count buildings for          *
 *                	cashToGive (int) - The amount of cold hard credits to give the player      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of the given buildings for that player       *
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
static int Script_GiveCreditsToPlayer(lua_State* L) {
    int houseType = lua_tointeger(L, 1);
    int cashToGive = lua_tointeger(L, 2);

    for (int h_index = 0; h_index < Houses.Count(); h_index++) {

        HouseClass* house = Houses.Ptr(h_index);

        if (house->ID == houseType) {
            house->Refund_Money(cashToGive);
        }

    }

    return 1;
}

/***********************************************************************************************
 * Script_NumBuildingTypeForPlayer - Returns the amount of specific buildings for a player     *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int) - The player (house) index to count buildings for          *
 *                	structType (int) - The structure index to count buildings for              *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of the given buildings for that player       *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_NumBuildingTypeForPlayer(lua_State* L) {
    int houseType = lua_tointeger(L, 1);
    int structType = lua_tointeger(L, 2);
    int result = 0;
    for (int b_index = 0; b_index < Buildings.Count(); b_index++) {
        BuildingClass* building = Buildings.Ptr(b_index);

        if (building->Owner() == houseType) {
            if (building->Class->Type == structType) {
                result++;
            }
        }
    }

    lua_pushnumber(L, result);
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
 * Script_CreateTeam - Attempts to create team (as defined in map)                             *
 *                                                                                             *
 *   SCRIPT INPUT:	teamType (int) - The team index to attempt to create                       *
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
static int Script_CreateTeam(lua_State* L) {

    const char* teamType = lua_tostring(L, -1);

    TeamTypeClass* teamPtr = TeamTypeClass::From_Name(teamType);
    assert(teamPtr != NULL);

    ScenarioInit++;
    teamPtr->Create_One_Of();
    ScenarioInit--;



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
    bool createEnabled = true;

    houseType = lua_tointeger(L, 1);

    if (lua_gettop(L) == 2) {
        createEnabled = lua_toboolean(L, 2);
    }

    if (createEnabled) {
        HouseClass::As_Pointer((HousesType)houseType)->AlertTime = 0;
    }

    if (houseType != HOUSE_NONE) {
        HouseClass* specified_house = HouseClass::As_Pointer((HousesType)houseType);
        specified_house->IsAlerted = createEnabled;

    }

    return 1;
}

/***********************************************************************************************
 * Script_AllowWin - Allows a win...ire map                                                    *
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
        HouseClass::As_Pointer((HousesType)houseType)->Blockage++;
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
 *   SCRIPT INPUT:	ret (int) - The music index of which to play                               *
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
static int Script_PlayMusic(lua_State* L) {
    int ret = lua_tointeger(L, -1);
    Theme.Play_Song((ThemeType)ret);
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
    sprintf(scriptName, "scripts/%s.script", mapName);
    L = luaL_newstate();

    if (luaL_loadfile(L, scriptName) || lua_pcall(L, 0, 0, 0)) {
        L = NULL;
        return false;
    }

    /**********************************************************************************************
    * Red Alert Vanilla Actions                                                                   *
    *=============================================================================================*/

    // player wins!
    // player loses.
    // computer begins factory production.
    lua_register(L, "CreateTeam", Script_CreateTeam);    // computer creates a certain type of team
        // destroy team (comment was redacted. was something offensive originally written?)
        // all enemy units go into hunt mode (teams destroyed).
        // player gets reinforcements
        // (house that gets them is determined by
        // the Reinforcement instance)
        // Deploy drop zone smoke.
        // Sell all buildings and go on rampage.
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
        // Force trigger to activate.
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
    lua_register(L, "GiveCreditsToPlayer", Script_GiveCreditsToPlayer);             // Give [player] [credits]
    lua_register(L, "NumBuildingTypeForPlayer", Script_NumBuildingTypeForPlayer);	// Number of buildings of [type] for given [player]


/**********************************************************************************************
* Script Globals                                                                              *
*=============================================================================================*/

    lua_pushnumber(L, PlayerPtr->ID);                                                // Local (human) player
    lua_setglobal(L, "_localPlayer");


    return true;
}

