#pragma once

#define LUA_SCRIPT_NAME  "D:\\Git\\JH\\learn\\src\\test_lua\\script\\a.lua"

int cadd(lua_State* L)
{
    int a = lua_tointeger(L,1);
    int b = lua_tointeger(L,2);
    lua_pushinteger(L,a+b);
    return 1; //> ���� ����ֵ�ĸ���
}

void run_fun_with_cfun_in_script()
{
    lua_State* L = luaL_newstate();
    if (!L) return ;

    luaL_openlibs(L);

    //> ���ؽű�����luaL_loadfileִ��ʱ����
    int nRet = luaL_dofile(L, LUA_SCRIPT_NAME); 
    if (0 != nRet) goto Exit0;

    //> ע���ڽű���Ҫ���õ�c����
    lua_pushcfunction(L,cadd);         //ע����lua��ʹ�õ�c����  
    lua_setglobal(L,"cadd");           //�󶨵�lua�е�����cadd 

    //> ����lua�ű�
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