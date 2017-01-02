// LspInst.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <Windows.h>
#include <atlstr.h>
#include "common.h"

//>
//> LSP 已经不被微软推荐使用，现在推荐使用 WFP(Windows Filtering Platform)
//> 参考：https://msdn.microsoft.com/zh-cn/office/aa366510(v=vs.80)
//>

// -i "TinyLsp" "TinyLsp.dll" CatalogId 
int _tmain(int argc, _TCHAR* argv[])
{
    INT nRet = 0;
    HMODULE hMod = LspGetUpdateProvider();
    LspCreateHeap(&nRet);

    ::MessageBox(0, 0, 0, 0);


    const wchar_t* pwcsLspName = NULL;
    const wchar_t* pwcsLspFileName = NULL;
    BOOL    bInstall = FALSE;
    BOOL    bEnum = FALSE;
    BOOL    bRemove = FALSE;
    DWORD   dwCatalogId = 0;

    for (int i = 1; i < argc; i++)
    {
        if ((2 != ::wcslen(argv[i])) && (L'-' != argv[i][0]))
        {
            goto cleanup;
        }

        switch (tolower(argv[i][1]))
        {
        case L'p': // print the catalog
            {
                bEnum = TRUE;
                break;
            }
        case L'i': // Install LSP (by default it's non-IFS)
            {
                bInstall = TRUE;
                if (i+1 >= argc) goto cleanup;

                dwCatalogId = _wtoi(argv[++i]);
                break;
            }
        case L'r': // -r CatId    Remove LSP
            {
                bRemove = TRUE;
                if (i+1 >= argc) goto cleanup;
                dwCatalogId = _wtoi(argv[++i]);
                break;
            }
        case L'n':
            {
                if (i+1 >= argc) goto cleanup;
                pwcsLspName = argv[++i];
                break;
            }
        case L'f':
            {
                if (i+1 >= argc) goto cleanup;
                pwcsLspFileName = argv[++i];
                break;
            }
        default:
            break;
        }
    }

    wchar_t wcsSystemPath[MAX_PATH+1] = {0};
    WINSOCK_CATALOG Catalog;
#ifdef _WIN64
    Catalog = LspCatalog64Only;
    ::GetSystemDirectoryW(wcsSystemPath, MAX_PATH+1);
#else
    Catalog = LspCatalog32Only;
    ::GetSystemDirectoryW(wcsSystemPath, MAX_PATH+1);
#endif

    if (bEnum)
    {
        INT nSize = 0;
        LPWSAPROTOCOL_INFOW p = EnumerateProviders(Catalog, &nSize);
        for (INT i = 0; i < nSize; i++)
        {
            wprintf(L"%04d - %s \t %d\n", p[i].dwCatalogEntryId, p[i].szProtocol, p[i].ProtocolChain.ChainLen);
        }
        LspFree(p);
    }


    if (bInstall)
    {
        BOOL        bIfsProvider = FALSE;
        wchar_t     wcsBuf[MAX_PATH+1] = {0};
        wchar_t     wcsLspName[MAX_PATH+1] = {0};

        if (NULL == pwcsLspFileName || NULL == pwcsLspName)
        {
            goto cleanup;
        }

        ::memcpy(wcsLspName, pwcsLspName, sizeof(wchar_t)*(::wcslen(pwcsLspName)));
        ::memcpy(wcsBuf, wcsSystemPath, sizeof(wchar_t)*(MAX_PATH+1));
        ::PathAppendW(wcsBuf, pwcsLspFileName);
        InstallLsp(Catalog, wcsLspName, wcsBuf, dwCatalogId, bIfsProvider);
    }
    else if (bRemove)
    {
        RemoveProvider(Catalog, dwCatalogId);
    }

cleanup:
    LspFreeUpdateProvider(hMod);
    LspDestroyHeap();
    return 0;
}

