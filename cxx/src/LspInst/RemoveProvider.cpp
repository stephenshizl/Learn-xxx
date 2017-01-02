#include "stdafx.h"
#include "common.h"

//
// Function: RemoveProvider
//
// Description:
//    This function removes a layered provider. Things can get tricky if
//    we're removing a layered provider which has been layered over by 
//    another provider. This routine first creates the LSP map to determine
//    if other LSPs on the system reference the LSP we want to remove. If
//    there are we must fix those LSPs before deleting the target LSP.
//    If we're on a platform that supports WSCUpdateProvider its simply a
//    matter of removing any reference to the target LSP's layered protocol
//    chains and the dummy hidden entry. 
//
//    If we're not on a WSCUpdateProvider enabled system, then its very tricky.
//    We must uninstall the dependent LSPs first followed by reinstalling them
//    in the same order they were originally installed. For example if LSP1,
//    LSP2, and LSP3 are installed (in that order) and this routine is invoked
//    to remove LSP1, we must uninstall LSP3 and LSP2 followed by re-installing 
//    (LSP2 first then LSP3). For each LSP added back we must fix up the protocol
//    chains of the next higher LSP so the reference the new catalog IDs (since
//    the action of installing an LSP assigns a new catalog ID).
//
//    NOTE: If WSCUpdateProvider is not supported there is the possiblity of
//          another process changing the Winsock catalog at the same time we're
//          trying to fix it back up. If this occurs it is possible for the 
//          corruption to occur.
//
int RemoveProvider(
    WINSOCK_CATALOG Catalog,            // Catalog to remove an LSP from
    DWORD           dwProviderId        // Catalog ID of LSPs hidden entry
    )
{
    //> 这里只简单处理 non-if 方式的 lsp
    GUID                 ProviderGuid;
    INT                  nRet = NO_ERROR;
    INT                  nErrorCode = NO_ERROR;
    INT                  nProviderCount = 0;
    WSAPROTOCOL_INFOW*   pProvider = NULL;
    pProvider = EnumerateProviders(Catalog, &nProviderCount);
    if (NULL == pProvider) goto cleanup;

    for (INT i = 0; i < nProviderCount; i++)
    {
        if ((pProvider[i].ProtocolChain.ChainLen > 1) &&
            pProvider[i].ProtocolChain.ChainEntries[0] == dwProviderId)
        {
            ::WSCDeinstallProvider(&pProvider[i].ProviderId, &nErrorCode);
        }
    }

    for (INT i = 0; i < nProviderCount; i++)
    {
        if (pProvider[i].ProtocolChain.ChainLen != LAYERED_PROTOCOL) 
            continue;
        if (pProvider[i].dwCatalogEntryId = dwProviderId)
        {
            ::memcpy(&ProviderGuid, &pProvider[i].ProviderId, sizeof(GUID));
            break;
        }
    }

    nRet = ::WSCDeinstallProvider(&ProviderGuid, &nErrorCode);
cleanup:
    return nRet;
}