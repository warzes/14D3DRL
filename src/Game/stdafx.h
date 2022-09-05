#pragma once

#if defined(_MSC_VER)
#	pragma warning(disable : 4514)
#	pragma warning(disable : 4626)
#	pragma warning(disable : 4820)
#	pragma warning(disable : 5027)
#	pragma warning(disable : 5045)
#	pragma warning(push, 0)
#	pragma warning(disable : 5039)
#endif

//=============================================================================
// STL Header
#include <cassert>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <random>
#include <string>
#include <array>
#include <queue>
#include <vector>
#include <unordered_map>
#include <memory>
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
// EMSCRIPTEN
#if defined(__EMSCRIPTEN__)
#	if SE_WGPU // TODO:
#		include <webgpu/webgpu.h>
#	endif
#	include <emscripten/emscripten.h>
#	include <emscripten/html5.h>
#endif // __EMSCRIPTEN__
//=============================================================================

//=============================================================================
// ANDROID
#if defined(__ANDROID__)
#	include <android/log.h>
#	include <pthread.h>
#	include <unistd.h>
#	include <time.h>
#	include <android/native_activity.h>
#	include <android/looper.h>
#	include <EGL/egl.h>
#endif // __ANDROID__
//=============================================================================

//=============================================================================
// LINUX
#if defined(__linux__) || defined(__unix__)
#	define GL_GLEXT_PROTOTYPES
#	include <X11/Xlib.h>
#	include <X11/Xutil.h>
#	include <X11/XKBlib.h>
#	include <X11/keysym.h>
#	include <X11/Xresource.h>
#	include <X11/Xatom.h>
#	include <X11/extensions/XInput2.h>
#	include <X11/Xcursor/Xcursor.h>
#	include <X11/cursorfont.h>
#	include <X11/Xmd.h>
#	include <dlfcn.h>
#	include <limits.h>
#	include <pthread.h>
#	include <time.h>
#endif // LINUX
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

#include "CoreConfig.h"