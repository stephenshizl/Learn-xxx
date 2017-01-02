#include "stdafx.h"
#include <winsock2.h>
#include <ws2spi.h>
#include <SpOrder.h>
#include <strsafe.h>
#include <vector>
#include "common.h"


#define UPDATE_LSP_ENTRY     (0)
#define CATALOG_ID_FLAG      (1)

typedef void (*LPFN_GETLSPGUID) (GUID *lpGuid);
typedef int (WSAAPI * FNINSTALLPROVIDERANDCHAINS)(
    LPGUID lpProviderId,
    LPWSTR lpszProviderDllPath,
#ifdef _WIN64
    LPWSTR lpszProviderDllPath32,
#endif
    LPWSTR lpszLspName,
    DWORD dwServiceFlags,
    LPWSAPROTOCOL_INFOW lpProtocolInfoList,
    DWORD dwNumberOfEntries,
    LPDWORD lpdwCatalogEntryId,
    LPINT lpErrno);

extern LPWSCUPDATEPROVIDER fnWscUpdateProvider, fnWscUpdateProvider32;


BOOL IsIdInChain(WSAPROTOCOL_INFOW *pInfo, DWORD dwId)
{
    for(INT i=0; i < pInfo->ProtocolChain.ChainLen; i++)
    {
        if (pInfo->ProtocolChain.ChainEntries[i] == dwId)
            return TRUE;
    }
    return FALSE;
}

BOOL IsNonIfsProvider(
    WSAPROTOCOL_INFOW  *pProvider,
    int                 nProviderCount,
    DWORD               dwProviderId)
{
    for(int i=0; i < nProviderCount ;i++)
    {
        if (pProvider[i].dwCatalogEntryId == dwProviderId)
        {
            return !(pProvider[ i ].dwServiceFlags1 & XP1_IFS_HANDLES);
        }
    }
    return FALSE;
}

DWORD FindDummyIdFromProtocolChainID(DWORD CatalogId, WSAPROTOCOL_INFOW* Catalog, INT CatalogCount)
{
    for (INT i = 0; i < CatalogCount; i++)
    {
        if (Catalog[i].dwCatalogEntryId == CatalogId)
        {
            if (Catalog[i].ProtocolChain.ChainLen == LAYERED_PROTOCOL)
                return Catalog[i].dwCatalogEntryId;
            else
                return Catalog[i].ProtocolChain.ChainEntries[0];
        }
    }
    return 0;
}

void InsertIdIntoProtocolChain(
    WSAPROTOCOL_INFOW  *Entry,
    int                 Index,
    DWORD               InsertId)
{
    for(int i=Entry->ProtocolChain.ChainLen; i > Index ;i--)
    {
        Entry->ProtocolChain.ChainEntries[i] = Entry->ProtocolChain.ChainEntries[i - 1];
    }

    Entry->ProtocolChain.ChainEntries[Index] = InsertId;
    Entry->ProtocolChain.ChainLen++;
}

int RetrieveLspGuid(const wchar_t* wcsLspPath, GUID* Guid)
{
    HMODULE         hMod = NULL;
    LPFN_GETLSPGUID fnGetLspGuid = NULL;
    int             nRet = ERROR_SUCCESS;

    hMod = ::LoadLibraryW(wcsLspPath);
    if (NULL == hMod) goto cleanup;

    fnGetLspGuid = (LPFN_GETLSPGUID)::GetProcAddress(hMod, "GetLspGuid");
    if (NULL == fnGetLspGuid) goto cleanup;

    fnGetLspGuid(Guid);
    nRet = NO_ERROR;
cleanup:
    if (NULL != hMod) ::FreeLibrary(hMod);
    return nRet;
}

//
// Function: UpdateProvider
// 
// Description:
//    This function is a wrapper for the WSCUpdateProvider function and calls the
//    correct version depending on the Winsock catalog being manipulated.
//
int
UpdateProvider(
               WINSOCK_CATALOG     Catalog,            // Catalog to perform the udpate in
               LPGUID              ProviderId,         // Guid of provider(s) to update
               WCHAR              *DllPath,            // DLL path of LSP being updated
               WSAPROTOCOL_INFOW  *ProtocolInfoList,   // Array of provider structures to update
               DWORD               NumberOfEntries,    // Number of providers in the array
               LPINT               lpErrno             // Error value returned on failure
               )
{
    int     rc = SOCKET_ERROR;
#ifdef _WIN64
    if ( LspCatalog64Only == Catalog )
    {
        rc = fnWscUpdateProvider(
            ProviderId,
            DllPath,
            ProtocolInfoList,
            NumberOfEntries,
            lpErrno
            );
    }
    else if ( LspCatalog32Only == Catalog )
    {
        rc = fnWscUpdateProvider32(
            ProviderId,
            DllPath,
            ProtocolInfoList,
            NumberOfEntries,
            lpErrno
            );
    }
#else
    if ( LspCatalog32Only == Catalog )
    {
        rc = fnWscUpdateProvider(
            ProviderId,
            DllPath,
            ProtocolInfoList,
            NumberOfEntries,
            lpErrno
            );
    }
#endif
    return rc;
}

int InstallProvider(
    WINSOCK_CATALOG     Catalog,
    GUID*               Guid,
    const wchar_t*      wcsLspPath,
    WSAPROTOCOL_INFOW*  pProviders,
    INT                 nProviderCount)
{
    int nRet = SOCKET_ERROR;
    int nErrorCode = NO_ERROR;

#ifdef _WIN64
    if (LspCatalog64Only == Catalog)
    {
        nRet = ::WSCInstallProvider(Guid, wcsLspPath, pProviders, nProviderCount, &nErrorCode);
    }
    else if (LspCatalogBoth == Catalog)
    {
        nRet = ::WSCInstallProvider64_32(Guid, wcsLspPath, pProviders, nProviderCount, &nErrorCode);
    }
#else
    if (LspCatalog32Only == Catalog)
    {
        nRet = WSCInstallProvider(Guid, wcsLspPath, pProviders, nProviderCount, &nErrorCode);
    }
#endif
    else
    {
        goto cleanup;
    }

cleanup:
    return nRet;
}

int DeinstallProvider(
    WINSOCK_CATALOG     Catalog,
    GUID*               Guid)
{
    int nRet = SOCKET_ERROR;
    int nErrorCode = NO_ERROR;

#ifdef _WIN64
    if (LspCatalogBoth == Catalog)
    {
        // Remove from 64-bit catalog
        nRet = ::WSCDeinstallProvider(Guid, &nErrorCode);
        // Remove from the 32-bit catalog
        nRet = ::WSCDeinstallProvider32(Guid, &nErrorCode);
    }
    else if (LspCatalog64Only == Catalog)
    {
        nRet = ::WSCDeinstallProvider(Guid, &nErrorCode);
    }
    else
    {
        nRet = ::WSCDeinstallProvider32(Guid, &nErrorCode);
    }
#else
    if (LspCatalog32Only == Catalog )
    {
        // Remove from the 32-bit catalog
        nRet = ::WSCDeinstallProvider(Guid, &nErrorCode);
    }
    else
    {
        return SOCKET_ERROR;
    }
#endif

    return NO_ERROR;
}

// Note that an IFS entry must be installed such that no non-IFS providers are 
// layered beneath it. This means if the user chooses to install the IFS LSP over
// a provider which includes non-IFS layers, it must insert itself into the chain
// such that it is below all non-IFS providers.
int InstallIfsLspProtocolChains(
    WINSOCK_CATALOG Catalog,
    GUID*           Guid,
    const wchar_t*  wcsLspName,
    const wchar_t*  wcsLspPath,
    DWORD*          pdwCatalogId)
{
    //> TODO:
    INT                 nRet = SOCKET_ERROR;
    INT                 nErrorCode = 0;
    INT                 nLastNonIfsPos = 0;
    BOOL                bContainsNonIfs = FALSE;
    BOOL                bLayeredOverNonIfs = FALSE;
    DWORD               dwDummyLspId = 0;

    DWORD*              pProviderOrder = NULL;
    WSAPROTOCOL_INFOW*  pProviderNew = NULL;
    INT                 nProviderCountNew = 0;

    WSAPROTOCOL_INFOW   LayerEntry;
    WSAPROTOCOL_INFOW   TmpEntry;
    WSAPROTOCOL_INFOW*  pProvider = NULL;
    INT                 nProviderCount = 0;
    pProvider = EnumerateProviders(Catalog, &nProviderCount);
    if (NULL == pProvider) goto cleanup;

    for (INT i = 0; i < nProviderCount; i++)
    {
        if (0 == ::memcmp(&pProvider[i].ProviderId, Guid, sizeof(GUID)))
        {
            dwDummyLspId = pProvider[i].dwCatalogEntryId;
            break;
        }
    }

    for (INT i = 0; i < nProviderCount; i++)
    {
        if (pProvider[i].dwCatalogEntryId == pdwCatalogId[0])
        {
            if (pProvider[i].ProtocolChain.ChainLen >= MAX_PROFILE_LEN - 1)
            {
                // Too many LSPs installed, and no chunk left.
                goto cleanup;
            }

            ::memcpy(&TmpEntry, &pProvider[ i ], sizeof(TmpEntry));
            ::memcpy(&LayerEntry, &pProvider[ i ], sizeof(LayerEntry));
            ::StringCchPrintfW(LayerEntry.szProtocol, WSAPROTOCOL_LEN, L"%s over [%s]", wcsLspName, pProvider[i].szProtocol);

            // Check whether the selected entry contains non IFS LSPs in its chain
            if (pProvider[i].ProtocolChain.ChainLen >= 2)
            {
                for (INT j = pProvider[i].ProtocolChain.ChainLen-2; j >= 0; j--)
                {
                    bContainsNonIfs = IsNonIfsProvider(pProvider, nProviderCount, pProvider[i].ProtocolChain.ChainEntries[j]);
                    if (TRUE == bContainsNonIfs)
                    {
                        nLastNonIfsPos = j;
                        break;
                    }
                }
            }

            if (TRUE == bContainsNonIfs)
            {
                // Need to modify the pProvider entry to reference the
                // added LSP entry within its chain

                // Need to fix the 'LayerEntry' 
                for (INT from = nLastNonIfsPos+1, to = 1; from < LayerEntry.ProtocolChain.ChainLen; from++, to++)
                {
                    LayerEntry.ProtocolChain.ChainEntries[to] = LayerEntry.ProtocolChain.ChainEntries[from];
                }
                LayerEntry.ProtocolChain.ChainEntries[0] = dwDummyLspId;
                LayerEntry.ProtocolChain.ChainLen -= (nLastNonIfsPos - 1);
                LayerEntry.dwServiceFlags1 |= XP1_IFS_HANDLES;
                bLayeredOverNonIfs = TRUE;

                // In the 'modified' array make a space at location after 'j'
                for(INT pos = pProvider[i].ProtocolChain.ChainLen; pos > nLastNonIfsPos+1 ;pos--)
                {
                    pProvider[i].ProtocolChain.ChainEntries[pos] = pProvider[i].ProtocolChain.ChainEntries[pos - 1];
                }
                pProvider[i].ProtocolChain.ChainLen++;
                pProvider[i].ProtocolChain.ChainEntries[nLastNonIfsPos+1] = UPDATE_LSP_ENTRY;
                // Save the index to the layer which corresponds to this entry
                pProvider[i].dwProviderReserved = CATALOG_ID_FLAG;

                // Need to insert the IFS provider in all LSPs that are layered
                // above the location where the IFS provider was just inserted
                for (INT pos = nLastNonIfsPos; pos > 0; pos--)
                {
                    INT k = 0;
                    for (; k < nProviderCount; k++)
                    {
                        if (pProvider[k].dwCatalogEntryId == TmpEntry.ProtocolChain.ChainEntries[pos]) break;
                    }

                    if (k > nProviderCount - 1) continue;
                    if (pProvider[k].ProtocolChain.ChainLen == LAYERED_PROTOCOL)
                    {
                        // Not good. The catalog ID in the chain points to the dummy
                        // entry. We'll need to do some other heuristic to find the
                        // "right" entry.
                        k = -1;
                        for(INT m=0; m < nProviderCount ;m++)
                        {
                            if ((TmpEntry.iAddressFamily == pProvider[m].iAddressFamily) &&
                                (TmpEntry.iSocketType == pProvider[m].iSocketType) && 
                                (TmpEntry.iProtocol == pProvider[m].iProtocol) &&
                                ((pos+1) == pProvider[m].ProtocolChain.ChainLen))
                            {
                                k = m;
                                break;
                            }
                        }
                    }

                    if (k == -1) continue;
                    if (pProvider[k].ProtocolChain.ChainLen < 2) continue;
                    for (INT m = pProvider->ProtocolChain.ChainLen - 2; k >= 0; k--)
                    {
                        if (TRUE == IsNonIfsProvider(pProvider, nProviderCount, pProvider[k].ProtocolChain.ChainEntries[m]))
                        {
                            InsertIdIntoProtocolChain(&pProvider[k], m+1, UPDATE_LSP_ENTRY);
                            pProvider[k].dwProviderReserved = CATALOG_ID_FLAG;
                        }
                    }
                }
            } 

            if (TRUE != bContainsNonIfs)
            {
                InsertIdIntoProtocolChain(&LayerEntry, 0, dwDummyLspId);
                // The second entry is always the ID of the current pProvider[i]
                //     In case of multiple LSPs then if we didn't do this the [1] index
                //     would contain the ID of that LSP's dummy entry and not the entry
                //     itself.
                LayerEntry.ProtocolChain.ChainEntries[1] = TmpEntry.dwCatalogEntryId;
                LayerEntry.dwServiceFlags1 |= XP1_IFS_HANDLES;
            }
        }
    }

    if (RPC_S_OK != UuidCreate(&LayerEntry.ProviderId)) goto cleanup;
    nRet = InstallProvider(Catalog, &LayerEntry.ProviderId, wcsLspPath, &LayerEntry, 1);
    if (NO_ERROR != nRet) goto cleanup;

    if (FALSE == bLayeredOverNonIfs)
    {
        std::vector<DWORD> vecProtocolOrder;
        WSAPROTOCOL_INFOW* pCurProvider = EnumerateProviders(Catalog, &nProviderCount);
        if (NULL == pCurProvider) goto cleanup;

        vecProtocolOrder.reserve(nProviderCount);
        for (INT i = 0; i < nProviderCount; i++)
        {
            if (TRUE == IsIdInChain(&pCurProvider[i], dwDummyLspId))
            {
                vecProtocolOrder.push_back(pCurProvider[i].dwCatalogEntryId);
            }
        }

        for (INT i = 0; i < nProviderCount; i++)
        {
            if (FALSE == IsIdInChain(&pCurProvider[i], dwDummyLspId))
            {
                vecProtocolOrder.push_back(pCurProvider[i].dwCatalogEntryId);
            }
        }

        LspFree(pCurProvider);
        pCurProvider = NULL;

#ifdef _WIN64
        if (LspCatalog32Only == Catalog)
        {
            nRet = ::WSCWriteProviderOrder32(&vecProtocolOrder[0], nProviderCount);
        }
        else if (LspCatalog64Only == Catalog)
        {
            nRet = ::WSCWriteProviderOrder(&vecProtocolOrder[0], nProviderCount);
        }
#else
        if (LspCatalog32Only == Catalog)
        {
            nRet = ::WSCWriteProviderOrder(&vecProtocolOrder[0], nProviderCount);
        }
#endif
        goto cleanup;
    }

    pProviderNew = EnumerateProviders(Catalog, &nProviderCountNew);
    if (NULL == pProviderNew) goto cleanup;

    for (INT i = 0; i < nProviderCountNew; i++)
    {
        if (0 == ::memcmp(&LayerEntry.ProviderId, &pProviderNew[i].ProviderId, sizeof(GUID)))
        {
            LayerEntry.dwCatalogEntryId = pProviderNew[i].dwCatalogEntryId;
        }
    }

    // Update the protocol chains of the modified entries to point to the just installed providers
    for (INT i = 0; i < nProviderCount; i++)
    {
        if (pProvider[i].dwProviderReserved == 0) continue;
        for (INT j = 0; j < pProvider[i].ProtocolChain.ChainLen; j++)
        {
            if (UPDATE_LSP_ENTRY == pProvider[i].ProtocolChain.ChainEntries[j])
            {
                pProvider[i].ProtocolChain.ChainEntries[j] = LayerEntry.dwCatalogEntryId;
                pProvider[i].dwProviderReserved = 0;
            }
        }

        WCHAR   wszLspDll[ MAX_PATH ];
        INT ProviderPathLen = MAX_PATH - 1;
        nRet = ::WSCGetProviderPath(&pProvider[i].ProviderId, wszLspDll, &ProviderPathLen, &nErrorCode);
        if (SOCKET_ERROR == nRet) goto cleanup;
        nRet = UpdateProvider(Catalog, &pProvider[i].ProviderId, wszLspDll, &pProvider[i], 1, &nErrorCode);
        if (SOCKET_ERROR == nRet) goto cleanup;
    }

    if (pProvider) LspFree(pProvider), pProvider = NULL;
    if (pProviderNew) LspFree(pProviderNew), pProviderNew = NULL;

    //WSCUpdateProvider doesn't update the process' copy of the winsock catalog. 
    //By calling cleanup and startup again, it forces a refresh. Otherwise, 
    //the rest of the installer code can't see the changes that were just made. 
    {
        WSADATA wsd;
        WSACleanup();
        WSAStartup(MAKEWORD(2,2), &wsd);
    }

    pProvider = EnumerateProviders(Catalog, &nProviderCount);
    if (NULL == pProvider) goto cleanup;
    pProviderOrder = (DWORD*)LspAlloc(nProviderCount*sizeof(DWORD), &nErrorCode);
    if (NULL == pProviderOrder) goto cleanup;

    INT index = 0;
    for (INT i = 0; i < nProviderCount; i++)
    {
        if (LayerEntry.dwCatalogEntryId == pProvider[i].dwCatalogEntryId)
        {
            pProvider[i].dwProviderReserved = 1;
            pProviderOrder[index++] = pProvider[i].dwCatalogEntryId;
        }
    }

    // Now go through the protocol chain of the entries we layered over and put those
    //    LSP entries next in the new order
    WSAPROTOCOL_INFOW* pEntry = NULL;
    for (INT i = 0; i < nProviderCount; i++)
    {
        if (LayerEntry.dwCatalogEntryId == pProvider[i].dwCatalogEntryId)
        {
            pEntry = &pProvider[i];
            break;
        }
    }
    if (NULL == pEntry) goto cleanup;

    for (INT i = 1; i < pEntry->ProtocolChain.ChainLen - 1; i++)
    {
        dwDummyLspId = FindDummyIdFromProtocolChainID(pEntry->ProtocolChain.ChainEntries[i], pProvider, nProviderCount);
        for (INT j = 0; j < nProviderCount; j++)
        {
            if (pProvider[j].ProtocolChain.ChainLen > 1 &&
                pProvider[j].ProtocolChain.ChainEntries[0] == dwDummyLspId &&
                pProvider[j].dwProviderReserved == 0)
            {
                pProviderOrder[index++] = pProvider[j].dwCatalogEntryId;
                pProvider[j].dwProviderReserved = 1;
            }
        }
    }

    // Now any catalog entry that wasn't already copied, copy it
    for (INT i = 0; i < nProviderCount; i++)
    {
        if (pProvider[i].dwProviderReserved == 0)
            pProviderOrder[index++] = pProvider[i].dwCatalogEntryId;
    }

    // Write the new catalog order
#ifdef _WIN64
    if (LspCatalog32Only == Catalog)
    {
        nRet = ::WSCWriteProviderOrder32(pProviderOrder, nProviderCount);
    }
    else if (LspCatalog64Only == Catalog)
    {
        nRet = ::WSCWriteProviderOrder(pProviderOrder, nProviderCount);
    }
#else
    if (LspCatalog32Only == Catalog)
    {
        nRet = ::WSCWriteProviderOrder(pProviderOrder, nProviderCount);
    }
#endif

cleanup:
    if (pProvider) LspFree(pProvider);
    if (pProviderNew) LspFree(pProviderNew);
    if (pProviderOrder) LspFree(pProviderOrder);
    return nRet;
}

int InstallNonIfsLspProtocolChains(
    WINSOCK_CATALOG Catalog,
    GUID*           Guid,
    const wchar_t*  wcsLspName,
    const wchar_t*  wcsLspPath,
    DWORD*          pdwCatalogId)
{
    INT                 nRet = SOCKET_ERROR;
    DWORD               dwDummyLspId = 0;
    WSAPROTOCOL_INFOW   LayerEntry;
    WSAPROTOCOL_INFOW*  pProvider = NULL;
    INT                 nProviderCount = 0;
    pProvider = EnumerateProviders(Catalog, &nProviderCount);
    if (NULL == pProvider) goto cleanup;

    // Find the dummy entry so we can extract its catalog ID
    for (INT i = 0; i < nProviderCount; i++)
    {
        if (0 == ::memcmp(&pProvider[i].ProviderId, Guid, sizeof(GUID)))
        {
            dwDummyLspId = pProvider[i].dwCatalogEntryId;
            break;
        }
    }

   for (INT i = 0; i < nProviderCount; i++)
   {
       if (pProvider[i].dwCatalogEntryId == pdwCatalogId[0])
       {
            if (pProvider[i].ProtocolChain.ChainLen >= MAX_PROFILE_LEN - 1)
            {
                // Too many LSPs installed, and no chunk left.
                goto cleanup;
            }

            ::memcpy(&LayerEntry, &pProvider[ i ], sizeof(LayerEntry));
            ::StringCchPrintfW(LayerEntry.szProtocol, WSAPROTOCOL_LEN, L"%s over [%s]", wcsLspName, pProvider[i].szProtocol);

            for (INT j = LayerEntry.ProtocolChain.ChainLen; j > 0; j--)
            {
                LayerEntry.ProtocolChain.ChainEntries[j] = LayerEntry.ProtocolChain.ChainEntries[j-1];
            }
            LayerEntry.ProtocolChain.ChainLen++;
            LayerEntry.ProtocolChain.ChainEntries[0] = dwDummyLspId;
            // The second entry is always the ID of the current pProvider[i]
            //     In case of multiple LSPs then if we didn't do this the [1] index
            //     would contain the ID of that LSP's dummy entry and not the entry
            //     itself.
            LayerEntry.ProtocolChain.ChainEntries[1] = pProvider[i].dwCatalogEntryId;
            // Remove the IFS flag
            LayerEntry.dwServiceFlags1 &= (~XP1_IFS_HANDLES); 
       }
   }

   if (RPC_S_OK != ::UuidCreate(&LayerEntry.ProviderId))
   {
       goto cleanup;
   }

    // Install the layered chain providers
    nRet = InstallProvider(Catalog, &LayerEntry.ProviderId, wcsLspName, &LayerEntry, 1);
    if (NO_ERROR != nRet) goto cleanup;

    // Reorder the winsock catalog so the layered chain entries appear first
    {
        std::vector<DWORD> vecProtocolOrder;
        WSAPROTOCOL_INFOW* pCurProvider = EnumerateProviders(Catalog, &nProviderCount);
        if (NULL == pCurProvider) goto cleanup;

        vecProtocolOrder.reserve(nProviderCount);
        for (INT i = 0; i < nProviderCount; i++)
        {
            if (TRUE == IsIdInChain(&pCurProvider[i], dwDummyLspId))
            {
                vecProtocolOrder.push_back(pCurProvider[i].dwCatalogEntryId);
            }
        }

        for (INT i = 0; i < nProviderCount; i++)
        {
            if (FALSE == IsIdInChain(&pCurProvider[i], dwDummyLspId))
            {
                vecProtocolOrder.push_back(pCurProvider[i].dwCatalogEntryId);
            }
        }

        LspFree(pCurProvider);
        pCurProvider = NULL;

#ifdef _WIN64
        if (LspCatalog32Only == Catalog)
        {
            nRet = ::WSCWriteProviderOrder32(&vecProtocolOrder[0], nProviderCount);
        }
        else if (LspCatalog64Only == Catalog)
        {
            nRet = ::WSCWriteProviderOrder(&vecProtocolOrder[0], nProviderCount);
        }
#else
        if (LspCatalog32Only == Catalog)
        {
            nRet = ::WSCWriteProviderOrder(&vecProtocolOrder[0], nProviderCount);
        }
#endif
    }
cleanup:
    if (NULL != pProvider) LspFree(pProvider);
    return nRet;
}

int InstallProviderVista(
    WINSOCK_CATALOG Catalog,
    wchar_t*        wcsLspName,
    wchar_t*        wcsLspFilePath,
    wchar_t*        wcsLspFilePath32,
    LPGUID          providerGuid,
    DWORD           dwCatalogIdArrayCount,
    DWORD*          pdwCatalogIdArray,
    BOOL            IfsProvider,
    BOOL            InstallOverAll)
{
    int nRet = SOCKET_ERROR;
    int nErrorCode = NO_ERROR;
    char* lpInstallFunction = NULL;
    FNINSTALLPROVIDERANDCHAINS lpInstallProviderAndChains;

    int nProviderCount  = 0;
    DWORD dwEntryCount  = 0;
    WSAPROTOCOL_INFOW *protocolList = NULL;
    WSAPROTOCOL_INFOW *pEnumProviders = NULL;

    HMODULE hMod = LoadLibraryW(L"ws2_32.dll");
    if (NULL == hMod) goto cleanup;

#ifdef _WIN64
    if (LspCatalog64Only == Catalog || LspCatalog32Only == Catalog)
    {
        // New install API always installs into both catalogs
        goto cleanup;
    }
    else
    {
        lpInstallFunction = "WSCInstallProviderAndChains64_32";
    }
#else
    if (LspCatalog64Only == Catalog || LspCatalogBoth == Catalog)
    {
        // Cannot install into 64-bit catalog from 32-bit process
        goto cleanup;
    }
    else
    {
        lpInstallFunction = "WSCInstallProviderAndChains";
    }
#endif

    lpInstallProviderAndChains = (FNINSTALLPROVIDERANDCHAINS)::GetProcAddress(hMod, lpInstallFunction);
    if (NULL == lpInstallProviderAndChains) goto cleanup;

    //
    // Install over all unique BSPs on the system so pass NULL for the provider list
    //
    if (InstallOverAll)
    {
        nRet = lpInstallProviderAndChains(
            providerGuid, wcsLspFilePath,
#ifdef _WIN64
            (wcsLspFilePath32[0] == '\0' ? wcsLspFilePath : wcsLspFilePath),
#endif
            wcsLspName, (IfsProvider ? XP1_IFS_HANDLES : 0),
            NULL, NULL, NULL, &nErrorCode);
        if (SOCKET_ERROR == nRet) goto cleanup;
        goto cleanup;
    }

    //
    // User specified a subset of providers to install over
    //
    nRet = SOCKET_ERROR;
    protocolList = (WSAPROTOCOL_INFOW*) LspAlloc(sizeof(WSAPROTOCOL_INFOW)*dwCatalogIdArrayCount, &nErrorCode);
    if (NULL == protocolList) goto cleanup;

    pEnumProviders = EnumerateProviders(Catalog, &nProviderCount);
    if (NULL == pEnumProviders) goto cleanup;

    dwEntryCount = 0;
    for (INT i = 0; i < (INT)dwCatalogIdArrayCount; i++)
    {
        for (INT j = 0; j < nProviderCount; j++)
        {
            if (pEnumProviders[j].dwCatalogEntryId == pdwCatalogIdArray[i])
                ::memcpy(&protocolList[dwEntryCount++], &pEnumProviders[j], sizeof(WSAPROTOCOL_INFOW));
        }
    }

    nRet = lpInstallProviderAndChains(
        providerGuid, wcsLspFilePath,
#ifdef _WIN64
        (wcsLspFilePath32[0] == '\0' ? wcsLspFilePath : wcsLspFilePath32),
#endif
        wcsLspName, (IfsProvider ? XP1_IFS_HANDLES : 0),
        protocolList, dwEntryCount, NULL, &nErrorCode);
    if (SOCKET_ERROR == nRet) goto cleanup;

    nRet = NO_ERROR;
cleanup:
    return nRet;
}

int InstallLsp(
    WINSOCK_CATALOG Catalog,
    wchar_t*        wsLspName,
    wchar_t*        wsLspFilePath,
    DWORD           dwCatalogId,  //> ID to install over
    BOOL            bIfsProvider)
{
    int  nRet = SOCKET_ERROR;
    GUID ProviderBaseGuid;

    OSVERSIONINFOEX     osv = {0};
    osv.dwOSVersionInfoSize = sizeof(osv);

    if(NULL == wsLspName || NULL == wsLspFilePath)
    {
        goto cleanup;
    }

    RetrieveLspGuid(wsLspFilePath, &ProviderBaseGuid);
    ::GetVersionEx((LPOSVERSIONINFO) &osv);

#ifdef _WIN64
    if (LspCatalog32Only == Catalog)
    {
        goto cleanup;
    }
    if (LspCatalogBoth == Catalog)
    {
        //> what should i do when call WSCWriteProviderOrder ?
    }
#else
    if (LspCatalog64Only == Catalog || LspCatalogBoth == Catalog)
    {
        goto cleanup;
    }
#endif

    if (osv.dwMajorVersion >= 6) 
    {
        // On Windows Vista, use the new LSP install API
        nRet = InstallProviderVista(
            Catalog, wsLspName, wsLspFilePath, wsLspFilePath, 
            &ProviderBaseGuid, 1, &dwCatalogId, bIfsProvider, FALSE);
        goto cleanup;
    }

    //> Two Steps:
    //> install a hidden 'dummy' entry, with ProviderBaseGuid, for a new catalog ID DummyEntryID
    //> install the protocol chain entry, with DummyEntryID¡¢a new GUID
    {
        INT               nProtocolCount = 0;
        WSAPROTOCOL_INFOW *pProtocolInfo = NULL;
        WSAPROTOCOL_INFOW  DummyEntry = {0};
        pProtocolInfo = EnumerateProviders(Catalog, &nProtocolCount);
        if (NULL == pProtocolInfo) goto cleanup;

        for (INT i = 0; i < nProtocolCount; i++)
        {
            if (pProtocolInfo[i].dwCatalogEntryId == dwCatalogId)
            {
                ::memcpy(&DummyEntry, &pProtocolInfo[i], sizeof(DummyEntry));
                break;
            }
        }

        DummyEntry.iSocketType = 0;
        DummyEntry.iProtocol   = 0;
        DummyEntry.dwProviderFlags |= PFL_HIDDEN;
        DummyEntry.dwProviderFlags &= (~PFL_MATCHES_PROTOCOL_ZERO);
        DummyEntry.ProtocolChain.ChainLen = LAYERED_PROTOCOL;
        ::wcsncpy_s(DummyEntry.szProtocol, wsLspName, WSAPROTOCOL_LEN);
        if (FALSE == bIfsProvider) DummyEntry.dwServiceFlags1 &= (~XP1_IFS_HANDLES);

        nRet = InstallProvider(Catalog, &ProviderBaseGuid, wsLspName, &DummyEntry, 1);
        if (NO_ERROR != nRet) goto cleanup;
    }

    if (FALSE == bIfsProvider)
    {
        nRet = InstallNonIfsLspProtocolChains(Catalog, &ProviderBaseGuid, wsLspName, wsLspFilePath, &dwCatalogId);
    }
    else
    {
        nRet = InstallIfsLspProtocolChains(Catalog, &ProviderBaseGuid, wsLspName, wsLspFilePath, &dwCatalogId);
    }

    if ( SOCKET_ERROR == nRet )
    {
        DeinstallProvider(Catalog, &ProviderBaseGuid);
    }

cleanup:
    return nRet;
}
