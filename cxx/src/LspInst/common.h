#pragma once
#include <winsock2.h>
#include <ws2spi.h>

// For 64-bit systems, we need to know which catalog to operate on
typedef enum
{
    LspCatalogBoth = 0,
    LspCatalog32Only,
    LspCatalog64Only
} WINSOCK_CATALOG;

#ifdef Debug
void dbgprint(const wchar_t *format, ...);
#else
#define dbgprint
#endif

void*   LspAlloc(SIZE_T size, INT *lpErrno);
void    LspFree(void* buf);
int     LspCreateHeap(INT *lpErrno);
void    LspDestroyHeap();

LPWSAPROTOCOL_INFOW EnumerateProviders(
    WINSOCK_CATALOG Catalog, 
    LPINT           TotalProtocols);

int InstallLsp(
    WINSOCK_CATALOG Catalog,
    wchar_t*        wsLspName,
    wchar_t*        wsLspFilePath,
    DWORD           dwCatalogId,  //> ID to install over
    BOOL            bIfsProvider);
int RemoveProvider(
    WINSOCK_CATALOG Catalog,            // Catalog to remove an LSP from
    DWORD           dwProviderId);      // Catalog ID of LSPs hidden entry

HMODULE LspGetUpdateProvider();
void LspFreeUpdateProvider(HMODULE hMod);