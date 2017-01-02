#include "stdafx.h"
#include <Windows.h>

HANDLE gLspHeap = NULL;

void* LspAlloc(SIZE_T size, INT *lpErrno)
{
    void *mem = NULL;
    mem = ::HeapAlloc(gLspHeap, HEAP_ZERO_MEMORY, size);
    if (NULL == mem)
    {
        *lpErrno = WSAENOBUFS;
    }

    return mem;
}

void LspFree(void* buf)
{
    ::HeapFree( gLspHeap, 0, buf );
}

int LspCreateHeap(INT *lpErrno)
{
    gLspHeap = ::HeapCreate( 0, 128000, 0 );
    if ( NULL == gLspHeap )
    {
        *lpErrno = WSAEPROVIDERFAILEDINIT;
        return SOCKET_ERROR;
    }
    return NO_ERROR;
}

void LspDestroyHeap()
{
    if ( NULL != gLspHeap )
    {
        ::HeapDestroy( gLspHeap );
        gLspHeap = NULL;
    }
}


