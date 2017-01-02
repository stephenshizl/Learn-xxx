#include "stdafx.h"
#include <winsock2.h>
#include <ws2spi.h>
#include <SpOrder.h>
#include <Windows.h>
#include <strsafe.h>

//
// Global variable: Function pointer to WSCUpdateProvider if on Windows XP or greater.
//                  Uninstalling an LSP when other LSPs are layered over it is really
//                  difficult; however on Windows XP and greater the WSCUpdateProvider
//                  function makes this much simpler. On older platforms its a real
//                  pain.
LPWSCUPDATEPROVIDER fnWscUpdateProvider = NULL, fnWscUpdateProvider32 = NULL;

HMODULE LspGetUpdateProvider()
{
    HMODULE hMod = NULL;
    HRESULT hRet = E_FAIL;
    wchar_t wcsBuf[MAX_PATH+1] = {0};

    if (0 == ::GetSystemDirectoryW(wcsBuf, MAX_PATH+1))
    {
        wchar_t wcsBufTmp[MAX_PATH+1] = {0};
        hRet = ::StringCchCopyW(wcsBufTmp, MAX_PATH+1, L"%SYSTEM%\\system32");
        if (FAILED(hRet)) goto cleanup;

        hRet = ::ExpandEnvironmentStringsW(wcsBuf, wcsBufTmp, MAX_PATH+1);
        if (S_OK != hRet) goto cleanup;
    }

    hRet = ::StringCbCatW(wcsBuf, MAX_PATH, L"\\ws2_32.dll");
    if (FAILED(hRet)) goto cleanup;

    hMod = ::LoadLibraryW(wcsBuf);
    if (NULL == hMod) goto cleanup;

#ifdef _WIN64
    fnWscUpdateProvider = (LPWSCUPDATEPROVIDER)::GetProcAddress(hMod, "WSCUpdateProvider");
    fnWscUpdateProvider32 = (LPWSCUPDATEPROVIDER)::GetProcAddress(hMod, "WSCUpdateProvider32");
#else
    fnWscUpdateProvider = (LPWSCUPDATEPROVIDER)::GetProcAddress(hMod, "WSCUpdateProvider");
#endif
    return hMod;
cleanup:
    if (NULL != hMod) ::FreeLibrary(hMod);
    return NULL;
}

void LspFreeUpdateProvider(HMODULE hMod)
{
    if (NULL != hMod) ::FreeLibrary(hMod);
    fnWscUpdateProvider = NULL;
    fnWscUpdateProvider32 = NULL;
    return ;
}