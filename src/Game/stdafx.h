#pragma once

#if defined(_MSC_VER)
#	pragma warning(disable : 4514)
#	pragma warning(push, 0)
#	pragma warning(disable : 5039)
#endif

//=============================================================================
// STL Header
#include <cassert>
#include <chrono>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
//=============================================================================

//=============================================================================
// WIN32
#if defined(_WIN32)
#	define WIN32_LEAN_AND_MEAN
#	define NOMINMAX
#	include <windows.h>
//#	include <shellapi.h>
#endif // _WIN32
//=============================================================================

//=============================================================================
// ANDROID
#if defined(__ANDROID__)
#	include <android/log.h>
#endif // __ANDROID__
//=============================================================================

//=============================================================================
// OpenGL
#include <glad/gl.h>
#if defined(_WIN32)
#	include <glad/wgl.h>
#endif
//=============================================================================

#define GLM_FORCE_XYZW_ONLY
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif