#pragma once

#if defined(_MSC_VER)
#	pragma warning(push, 0)
#	pragma warning(disable : 5039)
#endif

#include <cassert>
#include <string>

//=============================================================================
// WIN32
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>
#endif // _WIN32
//=============================================================================

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif