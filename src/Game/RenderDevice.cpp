#include "stdafx.h"
#include "RenderDevice.h"
#include "Log.h"
#include "OpenGLHeader.h"
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

	if (!OpenGLInitExtensions())
		return false;

	glEnable(GL_DEPTH_TEST);

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
	glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
//-----------------------------------------------------------------------------
void RenderDevice::Swap()
{
	SwapBuffers(hDC);
}
//-----------------------------------------------------------------------------