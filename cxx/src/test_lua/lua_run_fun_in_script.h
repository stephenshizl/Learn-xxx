#pragma once

#define LUA_SCRIPT_NAME  "D:\\Git\\JH\\learn\\src\\test_lua\\script\\a.lua"

void run_fun_in_script()
{
    lua_State* L = luaL_newstate();
    if (!L) return ;

    luaL_openlibs(L);

    //> 加载脚本，用luaL_loadfile执行时出错
    //> luaL_loadfile 只是把源文件加载到内存中，少了“编译”这一步
    //> luaL_dofile   既加载又编译
    int nRet = luaL_dofile(L, LUA_SCRIPT_NAME);
    if (0 != nRet) goto Exit0;

    //> 调用脚本中的函数
    lua_getglobal(L, "add");
    lua_pushnumber(L,1);
    lua_pushnumber(L,2);
    nRet = lua_pcall(L,2,1,0);
    if (0 != nRet)
    {
        printf("lua_pcall failed:%s",lua_tostring(L,-1));
    }
    else
    {
        printf("lua fun add return : %ld\n", lua_tointeger(L,-1));
    }
    lua_pop(L,1);
Exit0:
    lua_close(L);
    return ;
}