#pragma once

#define LUA_SCRIPT_NAME  "D:\\Git\\JH\\learn\\src\\test_lua\\script\\a.lua"

void run_lua_script()
{
    lua_State* L = luaL_newstate();
    if (!L) return ;

    luaL_openlibs(L);

    //> Ö´ÐÐ½Å±¾
    luaL_loadfile(L, LUA_SCRIPT_NAME);
    lua_pcall(L, 0, 0, 0);

    lua_close(L);
    return ;
}