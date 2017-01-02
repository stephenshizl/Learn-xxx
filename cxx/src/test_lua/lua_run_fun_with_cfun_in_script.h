#pragma once

#define LUA_SCRIPT_NAME  "D:\\Git\\JH\\learn\\src\\test_lua\\script\\a.lua"

int cadd(lua_State* L)
{
    int a = lua_tointeger(L,1);
    int b = lua_tointeger(L,2);
    lua_pushinteger(L,a+b);
    return 1; //> 返回 返回值的个数
}

void run_fun_with_cfun_in_script()
{
    lua_State* L = luaL_newstate();
    if (!L) return ;

    luaL_openlibs(L);

    //> 加载脚本，用luaL_loadfile执行时出错
    int nRet = luaL_dofile(L, LUA_SCRIPT_NAME); 
    if (0 != nRet) goto Exit0;

    //> 注册在脚本中要调用的c函数
    lua_pushcfunction(L,cadd);         //注册在lua中使用的c函数  
    lua_setglobal(L,"cadd");           //绑定到lua中的名字cadd 

    //> 调用lua脚本
    int a = 2, b = 3;
    lua_getglobal(L,"add_c");
    lua_pushinteger(L,a);
    lua_pushinteger(L,b);
    nRet = lua_pcall(L,2,1,0);
    if (0 != nRet)
    {
        printf("lua_pcall failed:%s",lua_tostring(L,-1));
    }
    else
    {
        printf("lua fun add_c:%d + %d = %ld\n", a, b,lua_tointeger(L,-1));
    }
    lua_pop(L,1);
Exit0:
    lua_close(L);
    return ;
}