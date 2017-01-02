#pragma once

#define LUA_HELLOWORLD  "print(\"test run lua buffer\")"
#define LUA_HELLOWORLD_LEN ::strlen(LUA_HELLOWORLD)

void run_lua_buffer()
{
    lua_State *L = luaL_newstate(); 
    if (NULL == L)
    {
        return ;
    }

    luaL_openlibs(L);

    luaL_loadbuffer(L, LUA_HELLOWORLD, LUA_HELLOWORLD_LEN, NULL);
    int error = lua_pcall(L, 0, 0, 0);

    int n = lua_gettop(L);
    if (error)
    {
        fprintf(stderr,"%s",lua_tostring(L,-1));
        lua_pop(L,1);
    }

    lua_close(L);
    L = NULL;
    return;
}