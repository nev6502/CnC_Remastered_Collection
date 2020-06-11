// MapScript.h
//

// Lua is written in C, so compiler needs to know how to link its libraries
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

//
// MapScript
//
class MapScript {
public:
	bool Init(const char* mapName);
	void CallFunction(const char* functionName);
private:
	lua_State* L;
};
