#include "stdafx.h"
#include <Windows.h>
#include <strsafe.h>

#ifdef Debug
void 
dbgprint(
         const wchar_t *format,
         ...
         )
{
    static  DWORD pid=0;
    va_list vl;
    wchar_t dbgbuf1[2048], dbgbuf2[2048];

    if ( 0 == pid )
    {
        pid = GetCurrentProcessId();
    }

    va_start(vl, format);
    StringCbVPrintf(dbgbuf1, sizeof(dbgbuf1),format, vl);
    StringCbPrintf(dbgbuf2, sizeof(dbgbuf2),L"%lu: %s\r\n", pid, dbgbuf1);
    va_end(vl);

    OutputDebugString(dbgbuf2);
}
#endif