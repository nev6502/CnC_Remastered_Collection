// MapScript.cpp
//

#include "FUNCTION.H"
#include "MapScript.h"

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
	return true;
}

