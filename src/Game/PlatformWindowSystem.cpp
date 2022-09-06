#include "stdafx.h"
#include "PlatformWindowSystem.h"
#include "Log.h"
#include "GameInput.h"
#include "PlatformUtility.h"
#include "PlatformWindowEvents.h"
#if defined(_WIN32)
//-----------------------------------------------------------------------------
extern void LockMouse(bool lock);
//-----------------------------------------------------------------------------
enum PROCESS_DPI_AWARENESS
{
	PROCESS_DPI_UNAWARE = 0,
	PROCESS_SYSTEM_DPI_AWARE = 1,
	PROCESS_PER_MONITOR_DPI_AWARE = 2
};
enum MONITOR_DPI_TYPE
{
	MDT_EFFECTIVE_DPI = 0,
	MDT_ANGULAR_DPI = 1,
	MDT_RAW_DPI = 2,
	MDT_DEFAULT = MDT_EFFECTIVE_DPI
};
struct dpi
{
	bool aware;
	float content_scale;
	float window_scale;
	float mouse_scale;
};
//-----------------------------------------------------------------------------
#ifndef GET_X_LPARAM
#	define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#	define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif
//-----------------------------------------------------------------------------
constexpr wchar_t CLASS_NAME[] = L"GameWindow";

HINSTANCE hInstance = nullptr;
HWND hWnd = nullptr;
MSG msg = { };
bool WindowClose = false;

bool HighDpi = true;
dpi Dpi;
float dpiScale = 1.0f;

int WindowWidth = 0;
int WindowHeight = 0;
int FramebufferWidth = 0;
int FramebufferWeight = 0;
bool Fullscreen = false;
RECT StoredWindowRect = { 0 }; // used to restore window pos/size when toggling fullscreen => windowed

bool Iconified = false;

// Mouse TODO: переместить?
struct
{
	float x = 0.0f;
	float y = 0.0f;
	float dx = 0.0f;
	float dy = 0.0f;
	bool shown = true;
	bool locked = false;
	bool pos_valid = true;
} mouse;
bool mouse_tracked = false;
uint8_t raw_input_data[256] = { 0 };
bool raw_input_mousepos_valid = false;
LONG raw_input_mousepos_x = 0;
LONG raw_input_mousepos_y = 0;
//-----------------------------------------------------------------------------
inline void initDpi()
{
	DECLARE_HANDLE(DPI_AWARENESS_CONTEXT_T);
	typedef BOOL(WINAPI* SETPROCESSDPIAWARE_T)();
	typedef bool (WINAPI* SETPROCESSDPIAWARENESSCONTEXT_T)(DPI_AWARENESS_CONTEXT_T);
	typedef HRESULT(WINAPI* SETPROCESSDPIAWARENESS_T)(PROCESS_DPI_AWARENESS);
	typedef HRESULT(WINAPI* GETDPIFORMONITOR_T)(HMONITOR, MONITOR_DPI_TYPE, UINT*, UINT*);

	SETPROCESSDPIAWARE_T fn_setprocessdpiaware = nullptr;
	SETPROCESSDPIAWARENESS_T fn_setprocessdpiawareness = nullptr;
	GETDPIFORMONITOR_T fn_getdpiformonitor = nullptr;
	SETPROCESSDPIAWARENESSCONTEXT_T fn_setprocessdpiawarenesscontext = nullptr;

	HINSTANCE user32 = LoadLibrary(L"user32.dll");
	if (user32)
	{
		fn_setprocessdpiaware = (SETPROCESSDPIAWARE_T)(void*)GetProcAddress(user32, "SetProcessDPIAware");
		fn_setprocessdpiawarenesscontext = (SETPROCESSDPIAWARENESSCONTEXT_T)(void*)GetProcAddress(user32, "SetProcessDpiAwarenessContext");
	}
	HINSTANCE shcore = LoadLibrary(L"shcore.dll");
	if (shcore)
	{
		fn_setprocessdpiawareness = (SETPROCESSDPIAWARENESS_T)(void*)GetProcAddress(shcore, "SetProcessDpiAwareness");
		fn_getdpiformonitor = (GETDPIFORMONITOR_T)(void*)GetProcAddress(shcore, "GetDpiForMonitor");
	}
	/*
		NOTE on SetProcessDpiAware() vs SetProcessDpiAwareness() vs SetProcessDpiAwarenessContext():

		These are different attempts to get DPI handling on Windows right, from oldest to newest. SetProcessDpiAwarenessContext() is required for the new DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 method.
	*/
	if (fn_setprocessdpiawareness)
	{
		if (HighDpi)
		{
			// app requests HighDPI rendering, first try the Win10 Creator Update per-monitor-dpi awareness, if that fails, fall back to system-dpi-awareness
			Dpi.aware = true;
			DPI_AWARENESS_CONTEXT_T per_monitor_aware_v2 = (DPI_AWARENESS_CONTEXT_T)-4;
			if (!(fn_setprocessdpiawarenesscontext && fn_setprocessdpiawarenesscontext(per_monitor_aware_v2)))
			{
				// fallback to system-dpi-aware
				fn_setprocessdpiawareness(PROCESS_SYSTEM_DPI_AWARE);
			}
		}
		else
		{
			// if the app didn't request HighDPI rendering, let Windows do the upscaling
			Dpi.aware = false;
			fn_setprocessdpiawareness(PROCESS_DPI_UNAWARE);
		}
	}
	else if (fn_setprocessdpiaware)
	{
		// fallback for Windows 7
		Dpi.aware = true;
		fn_setprocessdpiaware();
	}
	// get dpi scale factor for main monitor
	if (fn_getdpiformonitor && Dpi.aware)
	{
		POINT pt = { 1, 1 };
		HMONITOR hm = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
		UINT dpix, dpiy;
		HRESULT hr = fn_getdpiformonitor(hm, MDT_EFFECTIVE_DPI, &dpix, &dpiy);
		if (FAILED(hr))
		{
			LogFatal("ERROR DPI");
		}
		// clamp window scale to an integer factor
		Dpi.window_scale = (float)dpix / 96.0f;
	}
	else
	{
		Dpi.window_scale = 1.0f;
	}
	if (HighDpi)
	{
		Dpi.content_scale = Dpi.window_scale;
		Dpi.mouse_scale = 1.0f;
	}
	else
	{
		Dpi.content_scale = 1.0f;
		Dpi.mouse_scale = 1.0f / Dpi.window_scale;
	}
	dpiScale = Dpi.content_scale;
	if (user32) FreeLibrary(user32);
	if (shcore)  FreeLibrary(shcore);
}
//-----------------------------------------------------------------------------
inline void dpiChanged(HWND hwnd, LPRECT proposed_win_rect)
{
	// called on WM_DPICHANGED, which will only be sent to the application if sapp_desc.high_dpi is true and the Windows version is recent enough to support DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
	assert(HighDpi);
	HINSTANCE user32 = LoadLibraryA("user32.dll");
	if (!user32)
		return;

	typedef UINT(WINAPI* GETDPIFORWINDOW_T)(HWND);
	GETDPIFORWINDOW_T fn_getdpiforwindow = (GETDPIFORWINDOW_T)(void*)GetProcAddress(user32, "GetDpiForWindow");
	if (fn_getdpiforwindow)
	{
		UINT dpix = fn_getdpiforwindow(hwnd);
		// NOTE: for high-dpi apps, mouse_scale remains one
		Dpi.window_scale = (float)dpix / 96.0f;
		Dpi.content_scale = Dpi.window_scale;
		dpiScale = Dpi.window_scale;
		SetWindowPos(hwnd, 0,
			proposed_win_rect->left,
			proposed_win_rect->top,
			proposed_win_rect->right - proposed_win_rect->left,
			proposed_win_rect->bottom - proposed_win_rect->top,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
	FreeLibrary(user32);
}
//-----------------------------------------------------------------------------
LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	wchar_t mesgt[32];
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		WindowClose = true;
		return 0;

	case WM_SYSCOMMAND:
		switch (wParam & 0xFFF0)
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			if (Fullscreen) return 0;// disable screen saver and blanking in fullscreen mode
			break;
		case SC_KEYMENU:
			// user trying to access menu via ALT
			return 0;
		}
		break;

	case WM_ERASEBKGND:
		return 1;

	case WM_SIZE:
	{
		const bool iconified = wParam == SIZE_MINIMIZED;
		if (iconified != Iconified)
		{
			Iconified = iconified;
			if (iconified)
				WindowEventIconified();
			else
				WindowEventRestored();
		}
		WindowEventSize(/*width*/LOWORD(lParam), /*height*/HIWORD(lParam));
		WindowWidth = std::max(1, static_cast<int>(LOWORD(lParam)));
		WindowHeight = std::max(1, static_cast<int>(HIWORD(lParam)));
	}
	break;

	case WM_SETFOCUS:
		WindowEventFocused();
		break;

	case WM_KILLFOCUS:
		// if focus is lost for any reason, and we're in mouse locked mode, disable mouse lock
		if (mouse.locked) LockMouse(false);
		WindowEventUnFocused();
		break;

	case WM_DPICHANGED:
		// Update window's DPI and size if its moved to another monitor with a different DPI Only sent if DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 is used.
		dpiChanged(hWnd, (LPRECT)lParam);
		break;

	case WM_LBUTTONDBLCLK:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_MBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_XBUTTONDOWN:
	{
		const UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
		}
		else if (button == XBUTTON2)
		{
		}
	}

	break;

	case WM_LBUTTONUP:
		break;
	case WM_MBUTTONUP:
		break;
	case WM_RBUTTONUP:
		break;
	case WM_XBUTTONUP:
	{
		const UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
		}
		else if (button == XBUTTON2)
		{
		}
	}
	break;

	case WM_MOUSEMOVE:
		if (!mouse.locked)
		{
			const float new_x = (float)GET_X_LPARAM(lParam) * Dpi.mouse_scale;
			const float new_y = (float)GET_Y_LPARAM(lParam) * Dpi.mouse_scale;
			// don't update dx/dy in the very first event
			if (mouse.pos_valid)
			{
				mouse.dx = new_x - mouse.x;
				mouse.dy = new_y - mouse.y;
			}
			mouse.x = new_x;
			mouse.y = new_y;
			mouse.pos_valid = true;
			if (!mouse_tracked)
			{
				mouse_tracked = true;
				TRACKMOUSEEVENT tme = { 0 };
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hWnd;
				TrackMouseEvent(&tme);
				MouseEventEnter();
			}
			MouseEventMove(mouse.x, mouse.y);
		}
		break;

	case WM_INPUT:
		// raw mouse input during mouse-lock
		if (mouse.locked)
		{
			HRAWINPUT ri = (HRAWINPUT)lParam;
			UINT size = sizeof(raw_input_data);
			// see: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getrawinputdata
			if ((UINT)-1 == GetRawInputData(ri, RID_INPUT, &raw_input_data, &size, sizeof(RAWINPUTHEADER)))
			{
				LogFatal("GetRawInputData() failed\n");
				break;
			}
			const RAWINPUT* rawMouseData = (const RAWINPUT*)&raw_input_data;
			if (rawMouseData->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
			{
				/* mouse only reports absolute position
				   NOTE: THIS IS UNTESTED, it's unclear from reading the
				   Win32 RawInput docs under which circumstances absolute
				   positions are sent.
				*/
				if (raw_input_mousepos_valid)
				{
					LONG new_x = rawMouseData->data.mouse.lLastX;
					LONG new_y = rawMouseData->data.mouse.lLastY;
					mouse.dx = (float)(new_x - raw_input_mousepos_x);
					mouse.dy = (float)(new_y - raw_input_mousepos_y);
					raw_input_mousepos_x = new_x;
					raw_input_mousepos_y = new_y;
					raw_input_mousepos_valid = true;
				}
			}
			else
			{
				/* mouse reports movement delta (this seems to be the common case) */
				mouse.dx = (float)rawMouseData->data.mouse.lLastX;
				mouse.dy = (float)rawMouseData->data.mouse.lLastY;
			}
			MouseEventMove(raw_input_mousepos_x, raw_input_mousepos_y);
		}
		break;

	case WM_MOUSELEAVE:
		if (!mouse.locked)
		{
			mouse_tracked = false;
			MouseEventLeave();
		}
		break;

	case WM_MOUSEWHEEL:
		//int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		break;
		
	case WM_CHAR:
	//{
	//	const bool repeat = !!(lParam & 0x40000000);
	//	const wchar_t c = (wchar_t)wParam;
	//	swprintf_s(mesgt, L"WM_CHAR: %c\n", c);
	//	OutputDebugString(mesgt);
	//}
		break;

	case WM_KEYDOWN:
		if (wParam == VK_UP || wParam == uint8_t('W')) SetKeyState(Key::Up, true);
		else if (wParam == VK_DOWN || wParam == uint8_t('S')) SetKeyState(Key::Down, true);
		else if (wParam == VK_LEFT || wParam == uint8_t('A')) SetKeyState(Key::Left, true);
		else if (wParam == VK_RIGHT || wParam == uint8_t('D')) SetKeyState(Key::Right, true);
		else if (wParam == uint8_t('Q')) SetKeyState(Key::StrafeLeft, true);
		else if (wParam == uint8_t('E')) SetKeyState(Key::StrafeRight, true);
		else if (wParam == VK_SPACE || wParam == uint8_t('Z')) SetKeyState(Key::A, true);
		else if (wParam == uint8_t('X')) SetKeyState(Key::B, true);
		else if (wParam == uint8_t('C')) SetKeyState(Key::C, true);
		else if (wParam == VK_ESCAPE) SetKeyState(Key::Esc, true);

#ifdef _DEBUG
		else if (wParam == uint8_t('R')) SetKeyState(Key::R, true);
		else if (wParam == uint8_t('F')) SetKeyState(Key::F, true);
		else if (wParam == uint8_t('T')) SetKeyState(Key::T, true);
		else if (wParam == uint8_t('G')) SetKeyState(Key::G, true);
#endif

		//const bool repeat = !!(lParam & 0x40000000);
		//const int vk = (int)(HIWORD(lParam) & 0x1FF);
		//swprintf_s(mesgt, L"WM_KEYDOWN: 0x%x\n", wParam);
		break;

	case WM_KEYUP:
		if (wParam == VK_UP || wParam == uint8_t('W')) SetKeyState(Key::Up, false);
		else if (wParam == VK_DOWN || wParam == uint8_t('S')) SetKeyState(Key::Down, false);
		else if (wParam == VK_LEFT || wParam == uint8_t('A')) SetKeyState(Key::Left, false);
		else if (wParam == VK_RIGHT || wParam == uint8_t('D')) SetKeyState(Key::Right, false);
		else if (wParam == uint8_t('Q')) SetKeyState(Key::StrafeLeft, false);
		else if (wParam == uint8_t('E')) SetKeyState(Key::StrafeRight, false);
		else if (wParam == VK_SPACE || wParam == uint8_t('Z')) SetKeyState(Key::A, false);
		else if (wParam == uint8_t('X')) SetKeyState(Key::B, false);
		else if (wParam == uint8_t('C')) SetKeyState(Key::C, false);
		else if (wParam == VK_ESCAPE) SetKeyState(Key::Esc, false);

#ifdef _DEBUG
		else if (wParam == uint8_t('R')) SetKeyState(Key::R, false);
		else if (wParam == uint8_t('F')) SetKeyState(Key::F, false);
		else if (wParam == uint8_t('T')) SetKeyState(Key::T, false);
		else if (wParam == uint8_t('G')) SetKeyState(Key::G, false);
#endif
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//-----------------------------------------------------------------------------
bool registerWindowClass()
{
	WNDCLASS wc = { };
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = windowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = nullptr;

	const ATOM classAtom = RegisterClass(&wc);
	if (!classAtom)
	{
		LogFatal("Failed to register window class");
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------------
inline bool windowUpdateDimensions()
{
	RECT rect;
	if (GetClientRect(hWnd, &rect))
	{
		float window_width = (float)(rect.right - rect.left) / Dpi.window_scale;
		float window_height = (float)(rect.bottom - rect.top) / Dpi.window_scale;
		WindowWidth = (int)roundf(window_width);
		WindowHeight = (int)roundf(window_height);
		int fb_width = (int)roundf(window_width * Dpi.content_scale);
		int fb_height = (int)roundf(window_height * Dpi.content_scale);
		// prevent a framebuffer size of 0 when window is minimized
		if (0 == fb_width)
		{
			fb_width = 1;
		}
		if (0 == fb_height) {
			fb_height = 1;
		}
		if ((fb_width != FramebufferWidth) || (fb_height != FramebufferWeight))

		{
			FramebufferWidth = fb_width;
			FramebufferWeight = fb_height;
			return true;
		}
	}
	else
	{
		WindowWidth = WindowHeight = 1;
		FramebufferWidth = FramebufferWeight = 1;
	}
	return false;
}
//-----------------------------------------------------------------------------
inline void setFullscreen(bool fullscreen, UINT swpFlags)
{
	HMONITOR monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO minfo = { 0 };
	minfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &minfo);
	const RECT mr = minfo.rcMonitor;
	const int monitor_w = mr.right - mr.left;
	const int monitor_h = mr.bottom - mr.top;

	const DWORD win_ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD win_style;
	RECT rect = { 0, 0, 0, 0 };

	Fullscreen = fullscreen;
	if (!Fullscreen)
	{
		win_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
		rect = StoredWindowRect;
	}
	else
	{
		GetWindowRect(hWnd, &StoredWindowRect);
		win_style = WS_POPUP | WS_SYSMENU | WS_VISIBLE;
		rect.left = mr.left;
		rect.top = mr.top;
		rect.right = rect.left + monitor_w;
		rect.bottom = rect.top + monitor_h;
		AdjustWindowRectEx(&rect, win_style, FALSE, win_ex_style);
	}
	const int win_w = rect.right - rect.left;
	const int win_h = rect.bottom - rect.top;
	const int win_x = rect.left;
	const int win_y = rect.top;
	SetWindowLongPtr(hWnd, GWL_STYLE, win_style);
	SetWindowPos(hWnd, HWND_TOP, win_x, win_y, win_w, win_h, swpFlags | SWP_FRAMECHANGED);
}
//-----------------------------------------------------------------------------
bool WindowSystem::Create(const WindowSystemCreateInfo& createInfo)
{
	HighDpi = createInfo.HighDpi;
	initDpi();

	hInstance = GetModuleHandle(nullptr);

	if (!registerWindowClass())
	{
		LogFatal("RegisterWindowClass() failed!");
		return false;
	}

	wchar_t wndTitle[128];
	utility::Utf8ToWide(createInfo.Title, wndTitle, 128);

	const DWORD win_ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	const DWORD win_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;


	RECT rect = { 0, 0, 0, 0 };
	rect.right = static_cast<LONG>(static_cast<float>(createInfo.Width) * Dpi.window_scale);
	rect.bottom = static_cast<LONG>(static_cast<float>(createInfo.Height) * Dpi.window_scale);
	AdjustWindowRectEx(&rect, win_style, FALSE, win_ex_style);
	const int win_width = rect.right - rect.left;
	const int win_height = rect.bottom - rect.top;

	const bool use_default_width = 0 == createInfo.Width;
	const bool use_default_height = 0 == createInfo.Height;

	hWnd = CreateWindowEx(win_ex_style, CLASS_NAME, wndTitle, win_style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		use_default_width ? CW_USEDEFAULT : win_width,
		use_default_height ? CW_USEDEFAULT : win_height,
		nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)
	{
		LogFatal("CreateWindowEx() failed!");
		return false;
	}

	windowUpdateDimensions();
	if (createInfo.Fullscreen)
	{
		setFullscreen(createInfo.Fullscreen, SWP_HIDEWINDOW);
		windowUpdateDimensions();
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return true;
}
//-----------------------------------------------------------------------------
void WindowSystem::Destroy()
{
	if (Fullscreen)
	{
		ChangeDisplaySettings(nullptr, CDS_RESET);
		ShowCursor(TRUE);
	}
	DestroyWindow(hWnd); hWnd = nullptr;
	UnregisterClass(CLASS_NAME, hInstance);
}
//-----------------------------------------------------------------------------
bool WindowSystem::Update()
{
	if (WindowClose) return false;

	while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}
//-----------------------------------------------------------------------------
#endif // _WIN32
//-----------------------------------------------------------------------------
WindowSystem* gWindowSystem = nullptr;
//-----------------------------------------------------------------------------
WindowSystem::WindowSystem()
{
	assert(!gWindowSystem);
	gWindowSystem = this;
}
//-----------------------------------------------------------------------------
WindowSystem::~WindowSystem()
{
	gWindowSystem = nullptr;
}
//-----------------------------------------------------------------------------
float GetWindowAspect()
{
	return (float)WindowWidth/(float)WindowHeight;
}
//-----------------------------------------------------------------------------
int GetWindowWidth()
{
	return WindowWidth;
}
//-----------------------------------------------------------------------------
int GetWindowHeight()
{
	return WindowHeight;
}
//-----------------------------------------------------------------------------