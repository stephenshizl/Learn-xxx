// test_lua.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include "lua.hpp"
#include "lua_run_buffer.h"
#include "lua_run_script.h"
#include "lua_run_fun_in_script.h"
#include "lua_run_fun_with_cfun_in_script.h"

int _tmain(int argc, _TCHAR* argv[])
{
    run_lua_buffer();
    run_lua_script();
    run_fun_in_script();
    run_fun_with_cfun_in_script();
	return 0;
}