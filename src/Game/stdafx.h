#pragma once

#if defined(_MSC_VER)
#	pragma warning(push, 0)
#	pragma warning(disable : 5039)
#endif

#include <cassert>
#include <string>
#include <vector>

//=============================================================================
// WIN32
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>
#endif // _WIN32
//=============================================================================

#include <glad/gl.h>
#if defined(_WIN32)
#	include <glad/wgl.h>
#endif

#define GLM_FORCE_XYZW_ONLY
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLHeader.h"

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif