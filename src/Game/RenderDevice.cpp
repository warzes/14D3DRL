#include "stdafx.h"
#include "RenderDevice.h"
#include "Log.h"
//-----------------------------------------------------------------------------
extern "C"
{
	// NVIDIA: Force usage of NVidia GPU in case there is an integrated graphics unit as well, if we don't do this we risk getting the integrated graphics unit and hence a horrible performance
	// -> See "Enabling High Performance Graphics Rendering on Optimus Systems" http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
	_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;

	// AMD: Force usage of AMD GPU in case there is an integrated graphics unit as well, if we don't do this we risk getting the integrated graphics unit and hence a horrible performance
	// -> Named "Dynamic Switchable Graphics", found no official documentation, only https://community.amd.com/message/1307599#comment-1307599 - "Can an OpenGL app default to the discrete GPU on an Enduro system?"
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
//-----------------------------------------------------------------------------
extern HWND hWnd;
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

	if (!gladLoaderLoadWGL(hDC))
	{
		LogFatal("gladLoaderLoadWGL fail");
		return false;
	}
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(hRCTemp);

	constexpr int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
#ifdef _DEBUG
		WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	hRC = wglCreateContextAttribsARB(hDC, 0, attribs);
	if (!hRC || !wglMakeCurrent(hDC, hRC))
	{
		LogFatal("Creating render context fail");
		return false;
	}

	if (!gladLoaderLoadGL())
	{
		LogFatal("gladLoadGL fail");
		return false;
	}


	wglSwapIntervalEXT(0);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
	glClearDepth(1.0f);

	// Init state: Culling
	// NOTE: All shapes/models triangles are drawn CCW
	//glCullFace(GL_BACK);                                    // Cull the back face (default)
	//glFrontFace(GL_CCW);                                    // Front face are defined counter clockwise (default)
	//glEnable(GL_CULL_FACE);                                 // Enable backface culling

	// Show current OpenGL and GLSL version
	LogPrint("GL: OpenGL device information:");
	LogPrint(("    > Vendor:   " + std::string((const char*)glGetString(GL_VENDOR))).c_str());
	LogPrint(("    > Renderer: " + std::string((const char*)glGetString(GL_RENDERER))).c_str());
	LogPrint(("    > Version:  " + std::string((const char*)glGetString(GL_VERSION))).c_str());
	LogPrint(("    > GLSL:     " + std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION))).c_str());

	return true;
}
//-----------------------------------------------------------------------------
void RenderDevice::Destroy()
{
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//-----------------------------------------------------------------------------
void RenderDevice::Swap()
{
	SwapBuffers(hDC);
}
//-----------------------------------------------------------------------------