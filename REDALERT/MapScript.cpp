// MapScript.cpp
//

#include "FUNCTION.H"
#include "MapScript.h"

static int Script_GiveMoney(lua_State* L) {
	int cashToGive = lua_tointeger(L, -1);
	PlayerPtr->Refund_Money(cashToGive);
	return 1;
}

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

static int Script_StartMissionTimer(lua_State* L) {
	int ret = lua_tointeger(L, -1);
	Scen.MissionTimer = Scen.MissionTimer + (ret * (TICKS_PER_MINUTE / 10));
	Scen.MissionTimer.Start();
	Map.Redraw_Tab();
	return 1;
}

static int Script_StopMissionTimer(lua_State* L) {
	Scen.MissionTimer.Start();
	return 1;
}

static int Script_PlayMovie(lua_State* L) {
	int ret = lua_tointeger(L, -1);
	Play_Movie((VQType)ret);
	return 1;
}

static int Script_Speak(lua_State* L) {
	int ret = lua_tointeger(L, -1);
	Speak((VoxType)ret);
	return 1;
}

static int Script_SetBriefingText(lua_State* L) {
	strcpy(Scen.BriefingText, lua_tostring(L, -1));
	return 1;
}

/*
==================
MapScript::CallFunction
==================
*/
void MapScript::CallFunction(const char* functionName) {
	lua_getglobal(L, functionName);
	lua_pcall(L, 0, 0, 0);
} //BriefingText

/*
==================
MapScript::Init
==================
*/
bool MapScript::Init(const char* mapName) {
	char scriptName[256];
	sprintf(scriptName, "scripts/%s.script", mapName);
	L = luaL_newstate();
	if (luaL_loadfile(L, scriptName) || lua_pcall(L, 0, 0, 0)) {		
		L = NULL;
		return false;
	}

	lua_register(L, "Play_Movie", Script_PlayMovie);
	lua_register(L, "SetBriefingText", Script_SetBriefingText);
	lua_register(L, "Speak", Script_Speak);
	lua_register(L, "StartMissionTimer", Script_StartMissionTimer);
	lua_register(L, "StopMissionTimer", Script_StopMissionTimer);
	lua_register(L, "NumBuildingTypeForPlayer", Script_NumBuildingTypeForPlayer);
	lua_register(L, "GiveMoney", Script_GiveMoney);
	return true;
}

