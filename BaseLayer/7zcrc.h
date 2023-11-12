#pragma once
#ifndef __7ZCRC__
#define __7ZCRC__
#include "Stdafx.h"
// extern "C" UINT32 __fastcall CrcUpdateT4(UINT32 v, const void* data, size_t size, const UINT32 * table);
// extern "C" UINT32 __fastcall CrcUpdateT8(UINT32 v, const void* data, size_t size, const UINT32 * table);

void CrcGenerateTable();
DWORD crc7z(DWORD crc, BYTE* data, int len);

#endif