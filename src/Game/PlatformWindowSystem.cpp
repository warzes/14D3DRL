#include "stdafx.h"
#include "PlatformWindowSystem.h"
#include "Log.h"
#include "GameInput.h"
#if defined(_WIN32)
//-----------------------------------------------------------------------------
constexpr wchar_t CLASS_NAME[] = L"GameWindow";

HINSTANCE hInstance = nullptr;
HWND hWnd = nullptr;
MSG msg = { };
bool WindowClose = false;

int WindowWidth = 0;
int WindowHeight = 0;
bool Fullscreen = false;
RECT StoredWindowRect = { 0 }; // used to restore window pos/size when toggling fullscreen => windowed
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		WindowWidth = std::max(1, (int)LOWORD(lParam));
		WindowHeight = std::max(1, (int)HIWORD(lParam));
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
		if (wParam == VK_DOWN || wParam == uint8_t('S')) SetKeyState(Key::Down, true);
		if (wParam == VK_LEFT || wParam == uint8_t('A')) SetKeyState(Key::Left, true);
		if (wParam == VK_RIGHT || wParam == uint8_t('D')) SetKeyState(Key::Right, true);
		if (wParam == VK_SPACE || wParam == uint8_t('Z')) SetKeyState(Key::A, true);
		if (wParam == uint8_t('X')) SetKeyState(Key::B, true);
		if (wParam == uint8_t('C')) SetKeyState(Key::C, true);
		//const bool repeat = !!(lParam & 0x40000000);
		//const int vk = (int)(HIWORD(lParam) & 0x1FF);
		//swprintf_s(mesgt, L"WM_KEYDOWN: 0x%x\n", wParam);
		break;

	case WM_KEYUP:
		if (wParam == VK_UP || wParam == uint8_t('W')) SetKeyState(Key::Up, false);
		if (wParam == VK_DOWN || wParam == uint8_t('S')) SetKeyState(Key::Down, false);
		if (wParam == VK_LEFT || wParam == uint8_t('A')) SetKeyState(Key::Left, false);
		if (wParam == VK_RIGHT || wParam == uint8_t('D')) SetKeyState(Key::Right, false);
		if (wParam == VK_SPACE || wParam == uint8_t('Z')) SetKeyState(Key::A, false);
		if (wParam == uint8_t('X')) SetKeyState(Key::B, false);
		if (wParam == uint8_t('C')) SetKeyState(Key::C, false);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//-----------------------------------------------------------------------------
bool RegisterWindowClass()
{
	WNDCLASS wc = { };
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);

	const ATOM classAtom = RegisterClass(&wc);
	if (!classAtom)
	{
		LogFatal("Failed to register window class");
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------------
void setFullscreen(bool fullscreen, UINT swp_flags)
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
	SetWindowPos(hWnd, HWND_TOP, win_x, win_y, win_w, win_h, swp_flags | SWP_FRAMECHANGED);
}
//-----------------------------------------------------------------------------
bool WindowSystem::Create(const WindowSystemCreateInfo& createInfo)
{
	hInstance = GetModuleHandle(nullptr);

	if (!RegisterWindowClass())
	{
		LogFatal("RegisterWindowClass() failed!");
		return false;
	}

	const DWORD win_ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD win_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;

	RECT rect = { 0, 0, 0, 0 };
	rect.right = (int)((float)createInfo.Width);
	rect.bottom = (int)((float)createInfo.Height);
	const bool use_default_width = 0 == createInfo.Width;
	const bool use_default_height = 0 == createInfo.Height;
	AdjustWindowRectEx(&rect, win_style, FALSE, win_ex_style);
	const int win_width = rect.right - rect.left;
	const int win_height = rect.bottom - rect.top;

	hWnd = CreateWindowEx(win_ex_style, CLASS_NAME, createInfo.Title, win_style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		use_default_width ? CW_USEDEFAULT : win_width,
		use_default_height ? CW_USEDEFAULT : win_height,
		nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)
	{
		LogFatal("CreateWindowEx() failed!");
		return false;
	}

	if (createInfo.fullscreen)
		setFullscreen(createInfo.fullscreen, SWP_HIDEWINDOW);

	GetClientRect(hWnd, &rect);
	WindowWidth = std::max(1, (int)roundf((float)(rect.right - rect.left)));
	WindowHeight = std::max(1, (int)roundf((float)(rect.bottom - rect.top)));

	ShowWindow(hWnd, SW_SHOW);

	return true;
}
//-----------------------------------------------------------------------------
void WindowSystem::Destroy()
{
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
float GetScreenAspect()
{
	return (float)WindowWidth/(float)WindowHeight;
}
//-----------------------------------------------------------------------------