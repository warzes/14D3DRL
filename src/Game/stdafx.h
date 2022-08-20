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

#include <gl/GL.h>
#include <glext.h>
#include <wglext.h>

#include "OpenGLHeader.h"

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif