#ifndef GLAD_WGL_H_
#define GLAD_WGL_H_

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <glad/gl.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x00000001
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_OPENGL_NO_ERROR_ARB 0x31B3
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

typedef HGLRC (GLAD_API_PTR *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int * attribList);
typedef const char * (GLAD_API_PTR *PFNWGLGETEXTENSIONSSTRINGARBPROC)(HDC hdc);
typedef const char * (GLAD_API_PTR *PFNWGLGETEXTENSIONSSTRINGEXTPROC)(void);

GLAD_API_CALL PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
GLAD_API_CALL PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
GLAD_API_CALL PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT;

GLAD_API_CALL int gladLoaderLoadWGL(HDC hdc);

#ifdef __cplusplus
}
#endif
#endif