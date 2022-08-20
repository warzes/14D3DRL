#include "stdafx.h"
#include "RenderDevice.h"
#include "Log.h"
//-----------------------------------------------------------------------------
extern HWND hWnd;
extern bool Fullscreen;
extern int WindowWidth;
extern int WindowHeight;
//-----------------------------------------------------------------------------
HDC hDC = nullptr;
HGLRC hRC = nullptr;
//-----------------------------------------------------------------------------
bool RenderDevice::Create()
{
	hDC = GetDC(hWnd);
	if (!hDC)
	{
		LogFatal("GetDC fail");
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;

	int format = ChoosePixelFormat(hDC, &pfd);
	if (!format || !SetPixelFormat(hDC, format, &pfd))
	{
		LogFatal("Setting pixel format fail");
		return false;
	}

	HGLRC hRCTemp = wglCreateContext(hDC);
	if (!hRCTemp || !wglMakeCurrent(hDC, hRCTemp))
	{
		LogFatal("Ñreating temp render context fail");
		return false;
	}

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(
		wglGetProcAddress("wglCreateContextAttribsARB"));

	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(hRCTemp);

	if (!wglCreateContextAttribsARB)
	{
		LogFatal("wglCreateContextAttribsARB fail");
		return false;
	}

	constexpr int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	hRC = wglCreateContextAttribsARB(hDC, 0, attribs);
	if (!hRC || !wglMakeCurrent(hDC, hRC))
	{
		LogFatal("Creating render context fail");
		return false;
	}

	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	/*LOG_DEBUG("OpenGL render context information:\n"
		"  Renderer       : %s\n"
		"  Vendor         : %s\n"
		"  Version        : %s\n"
		"  GLSL version   : %s\n"
		"  OpenGL version : %d.%d\n",
		(const char*)glGetString(GL_RENDERER),
		(const char*)glGetString(GL_VENDOR),
		(const char*)glGetString(GL_VERSION),
		(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION),
		major, minor
	);*/

	return true;
}
//-----------------------------------------------------------------------------
void RenderDevice::Destroy()
{
	if (Fullscreen)
	{
		ChangeDisplaySettings(nullptr, CDS_RESET);
		ShowCursor(TRUE);
	}

	if (hRC)
	{
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(hRC);
	}

	if (hDC) ReleaseDC(hWnd, hDC);

}
//-----------------------------------------------------------------------------
void RenderDevice::Clear()
{
	glViewport(0, 0, WindowWidth, WindowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
}
//-----------------------------------------------------------------------------
void RenderDevice::Swap()
{
	SwapBuffers(hDC);
}
//-----------------------------------------------------------------------------