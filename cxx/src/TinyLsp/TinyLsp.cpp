// TinyLsp.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif


#include <Winsock2.h>
#include <Ws2spi.h>
#include <Windows.h>
#include <tchar.h>
#include <atlstr.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Rpcrt4.lib")

WSPUPCALLTABLE g_pUpCallTable;      // �ϲ㺯���б����LSP�������Լ���α�������ʹ����������б�
WSPPROC_TABLE g_NextProcTable;      // �²㺯���б�
TCHAR   g_szCurrentApp[MAX_PATH];   // ��ǰ���ñ�DLL�ĳ��������
//
// This is the hardcoded guid for our dummy (hidden) catalog entry
//
GUID gProviderGuid = { //c5fabbd0-9736-11d1-947f-00c04fad860d
    0xc5fabbd0,
    0x9736,
    0x11d1,
    {0x94, 0x7f, 0x00, 0xc0, 0x4f, 0xad, 0x86, 0x0d}
};

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
    {
      case DLL_PROCESS_ATTACH:
      {
            // ȡ����ģ�������
            ::GetModuleFileName(NULL, g_szCurrentApp, MAX_PATH);
      }
        break;
    }
    return TRUE;
}

void WSPAPI GetLspGuid(LPGUID lpGuid)
{
    memcpy(lpGuid, &gProviderGuid, sizeof( GUID ));
}

LPWSAPROTOCOL_INFOW GetProvider(LPINT lpnTotalProtocols)
{
    DWORD dwSize = 0;
    int nError;
    LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
    // ȡ����Ҫ�ĳ���
    if(::WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError) == SOCKET_ERROR)
    {
        if(nError != WSAENOBUFS)
            return NULL;
    }
    pProtoInfo = (LPWSAPROTOCOL_INFOW)::GlobalAlloc(GPTR, dwSize);
    *lpnTotalProtocols = ::WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError);
    return pProtoInfo;
}

void FreeProvider(LPWSAPROTOCOL_INFOW pProtoInfo)
{
    ::GlobalFree(pProtoInfo);
}

int WSPAPI WSPSendTo(
                     SOCKET          s,
                     LPWSABUF        lpBuffers,
                     DWORD           dwBufferCount,
                     LPDWORD         lpNumberOfBytesSent,
                     DWORD           dwFlags,
                     const struct sockaddr FAR * lpTo,
                     int             iTolen,
                     LPWSAOVERLAPPED lpOverlapped,
                     LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
                     LPWSATHREADID   lpThreadId,
                     LPINT           lpErrno
                     )
{
    // �ܾ�����Ŀ�Ķ˿�Ϊ4567��UDP���
    SOCKADDR_IN sa = *(SOCKADDR_IN*)lpTo;
    if(sa.sin_port == htons(4567))
    {
        int iError;
        g_NextProcTable.lpWSPShutdown(s, SD_BOTH, &iError);
        *lpErrno = WSAECONNABORTED;
        return SOCKET_ERROR;
    }

    return g_NextProcTable.lpWSPSendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo
        , iTolen, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
}

int WSPAPI WSPStartup(
                      WORD wVersionRequested,
                      LPWSPDATA lpWSPData,
                      LPWSAPROTOCOL_INFO lpProtocolInfo,
                      WSPUPCALLTABLE UpcallTable,
                      LPWSPPROC_TABLE lpProcTable
                      )
{
    if(lpProtocolInfo->ProtocolChain.ChainLen <= 1)
    {
        return WSAEPROVIDERFAILEDINIT;
    }

    // �������ϵ��õĺ�����ָ�루�������ǲ�ʹ������
    g_pUpCallTable = UpcallTable;

    // ö��Э�飬�ҵ��²�Э���WSAPROTOCOL_INFOW�ṹ
    WSAPROTOCOL_INFOW   NextProtocolInfo;

    int nTotalProtos;
    LPWSAPROTOCOL_INFOW pProtoInfo = GetProvider(&nTotalProtos);

    // �²����ID
    DWORD dwBaseEntryId = lpProtocolInfo->ProtocolChain.ChainEntries[1];

    int i;
    for(i=0; i<nTotalProtos; i++)
    {
        if(pProtoInfo[i].dwCatalogEntryId == dwBaseEntryId)
        {
            memcpy(&NextProtocolInfo, &pProtoInfo[i], sizeof(NextProtocolInfo));
            break;
        }
    }

    {
        wchar_t strUuid[128] = {0};
        UuidToStringW(&lpProtocolInfo->ProviderId, (RPC_WSTR*)strUuid);
        CStringW tmp;
        tmp.Format(L"---TinyLsp---, EntryID:%d, Protocol:%s, ProviderID:%s \n", 
            lpProtocolInfo->dwCatalogEntryId, 
            lpProtocolInfo->szProtocol, 
            strUuid);
        ::OutputDebugString(tmp);
    }

    if(i >= nTotalProtos)
    {
        return WSAEPROVIDERFAILEDINIT;
    }

    // �����²�Э���DLL
    int nError;
    TCHAR szBaseProviderDll[MAX_PATH];
    int nLen = MAX_PATH;

    // ȡ���²��ṩ����DLL·��
    if(::WSCGetProviderPath(&NextProtocolInfo.ProviderId, szBaseProviderDll, &nLen, &nError) == SOCKET_ERROR)
    {
        return WSAEPROVIDERFAILEDINIT;
    }

    if(!::ExpandEnvironmentStrings(szBaseProviderDll, szBaseProviderDll, MAX_PATH))
    {
        return WSAEPROVIDERFAILEDINIT;
    }

    // �����²��ṩ����
    HMODULE hModule = ::LoadLibrary(szBaseProviderDll);
    if(hModule == NULL)
    {
        return WSAEPROVIDERFAILEDINIT;
    }

    // �����²��ṩ�����WSPStartup����
    LPWSPSTARTUP  pfnWSPStartup = NULL;
    pfnWSPStartup = (LPWSPSTARTUP)::GetProcAddress(hModule, "WSPStartup");
    if(pfnWSPStartup == NULL)
    {
        return WSAEPROVIDERFAILEDINIT;
    }

    // �����²��ṩ�����WSPStartup����
    LPWSAPROTOCOL_INFOW pInfo = lpProtocolInfo;
    if(NextProtocolInfo.ProtocolChain.ChainLen == BASE_PROTOCOL)
        pInfo = &NextProtocolInfo;

    int nRet = pfnWSPStartup(wVersionRequested, lpWSPData, pInfo, UpcallTable, lpProcTable);
    if(nRet != ERROR_SUCCESS)
    {
        return nRet;
    }

    // �����²��ṩ�ߵĺ�����
    g_NextProcTable = *lpProcTable;

    // �޸Ĵ��ݸ��ϲ�ĺ�����Hook����Ȥ�ĺ�����������Ϊʾ������Hook��WSPSendTo����
    // ��������Hook������������WSPSocket��WSPCloseSocket��WSPConnect��
    lpProcTable->lpWSPSendTo = WSPSendTo;

    FreeProvider(pProtoInfo);
    return nRet;
}



#ifdef _MANAGED
#pragma managed(pop)
#endif

