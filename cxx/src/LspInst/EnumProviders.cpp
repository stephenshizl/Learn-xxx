#include "stdafx.h"
#include <winsock2.h>
#include <ws2spi.h>
#include "common.h"

//
// Function: GetProviders
//
// Description:
//    This enumerates the Winsock catalog via the global variable ProtocolInfo.
//
//  WSAEnumProtocols, 仅能枚举基础协议和协议链，不能枚举分层协议
//  WSCEnumProtocols, 能够枚举各种协议，包括分层协议，基础协议，协议链
//
LPWSAPROTOCOL_INFOW EnumerateProviders(
    WINSOCK_CATALOG Catalog, 
    LPINT           TotalProtocols)
{
    LPWSAPROTOCOL_INFOW pProtocolInfo = NULL;
    DWORD               dwProtocolInfoSize = 0;
    INT                 nErrorCode = NO_ERROR;
    INT                 nRet = NO_ERROR;

    if (NULL == TotalProtocols) goto cleanup;
    *TotalProtocols = 0;

#ifdef _WIN64
    if (LspCatalog64Only == Catalog || LspCatalogBoth == Catalog)
    {
        nRet = ::WSCEnumProtocols(NULL, pProtocolInfo, &dwProtocolInfoSize, &nErrorCode);
        if (SOCKET_ERROR == nRet && WSAENOBUFS != nErrorCode) goto cleanup;

        nErrorCode = NO_ERROR;
        pProtocolInfo = (LPWSAPROTOCOL_INFOW)LspAlloc(dwProtocolInfoSize, &nErrorCode);
        if (NULL == pProtocolInfo) goto cleanup;

        nRet = ::WSCEnumProtocols(NULL, pProtocolInfo, &dwProtocolInfoSize, &nErrorCode);
        if (SOCKET_ERROR == nRet) goto cleanup;

        *TotalProtocols = nRet;
    }
    else if (LspCatalog32Only == Catalog)
    {
        HMODULE            hModule;
        LPWSCENUMPROTOCOLS fnWscEnumProtocols32 = NULL;

        hModule = ::LoadLibrary(TEXT("ws2_32.dll"));
        if (NULL == hModule) goto cleanup;

        fnWscEnumProtocols32 = (LPWSCENUMPROTOCOLS) GetProcAddress(hModule, ("WSCEnumProtocols32"));
        if (NULL == fnWscEnumProtocols32) goto cleanup;

        nRet = fnWscEnumProtocols32(NULL, pProtocolInfo, &dwProtocolInfoSize, &nErrorCode);
        if ( SOCKET_ERROR == nRet )
        {
            if (WSAENOBUFS != nErrorCode) goto cleanup;
            nErrorCode = NO_ERROR;
        }

        pProtocolInfo = (LPWSAPROTOCOL_INFOW) LspAlloc(dwProtocolInfoSize, &nErrorCode);
        if (NULL == pProtocolInfo) goto cleanup;

        nRet = fnWscEnumProtocols32(NULL, pProtocolInfo, &dwProtocolInfoSize, &nErrorCode);
        if (SOCKET_ERROR == nRet) goto cleanup;

        *TotalProtocols = nRet;
        FreeLibrary(hModule);
    }
#else
    if (LspCatalog32Only == Catalog)
    {
        nRet = ::WSCEnumProtocols(NULL, pProtocolInfo, &dwProtocolInfoSize, &nErrorCode);
        if (SOCKET_ERROR == nRet && WSAENOBUFS != nErrorCode) goto cleanup;

        pProtocolInfo = (LPWSAPROTOCOL_INFOW)LspAlloc(dwProtocolInfoSize, &nErrorCode);
        if (NULL == pProtocolInfo) goto cleanup;

        nErrorCode = NO_ERROR;
        nRet = ::WSCEnumProtocols(NULL, pProtocolInfo, &dwProtocolInfoSize, &nErrorCode);
        if (SOCKET_ERROR == nRet) goto cleanup;

        *TotalProtocols = nRet;
    }
    else if (LspCatalog64Only == Catalog || LspCatalogBoth == Catalog)
    {
        dbgprint(L"Unable to enumerate 64-bit Winsock catalog from 32-bit process!");
    }
#endif

cleanup:
    if ((NO_ERROR != nErrorCode) && (NULL != pProtocolInfo))
    {
        LspFree(pProtocolInfo);
        pProtocolInfo = NULL;
    }

    return pProtocolInfo;
}
