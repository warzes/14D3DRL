#include "stdafx.h"
#include "PlatformInputSystem.h"
//-----------------------------------------------------------------------------
InputSystem* gInputSystem = nullptr;
//-----------------------------------------------------------------------------
void LockMouse(bool lock)
{
}
//-----------------------------------------------------------------------------
InputSystem::InputSystem()
{
	assert(!gInputSystem);
	gInputSystem = this;

	initKeytable();
}
//-----------------------------------------------------------------------------
InputSystem::~InputSystem()
{
	gInputSystem = nullptr;
}
//-----------------------------------------------------------------------------
bool InputSystem::Create()
{
	return true;
}
//-----------------------------------------------------------------------------
void InputSystem::Destroy()
{
}
//-----------------------------------------------------------------------------
void InputSystem::initKeytable()
{
#if defined(_WIN32)
	// same as GLFW
	m_keycodes[0x00B] = KEY_0;
	m_keycodes[0x002] = KEY_1;
	m_keycodes[0x003] = KEY_2;
	m_keycodes[0x004] = KEY_3;
	m_keycodes[0x005] = KEY_4;
	m_keycodes[0x006] = KEY_5;
	m_keycodes[0x007] = KEY_6;
	m_keycodes[0x008] = KEY_7;
	m_keycodes[0x009] = KEY_8;
	m_keycodes[0x00A] = KEY_9;
	m_keycodes[0x01E] = KEY_A;
	m_keycodes[0x030] = KEY_B;
	m_keycodes[0x02E] = KEY_C;
	m_keycodes[0x020] = KEY_D;
	m_keycodes[0x012] = KEY_E;
	m_keycodes[0x021] = KEY_F;
	m_keycodes[0x022] = KEY_G;
	m_keycodes[0x023] = KEY_H;
	m_keycodes[0x017] = KEY_I;
	m_keycodes[0x024] = KEY_J;
	m_keycodes[0x025] = KEY_K;
	m_keycodes[0x026] = KEY_L;
	m_keycodes[0x032] = KEY_M;
	m_keycodes[0x031] = KEY_N;
	m_keycodes[0x018] = KEY_O;
	m_keycodes[0x019] = KEY_P;
	m_keycodes[0x010] = KEY_Q;
	m_keycodes[0x013] = KEY_R;
	m_keycodes[0x01F] = KEY_S;
	m_keycodes[0x014] = KEY_T;
	m_keycodes[0x016] = KEY_U;
	m_keycodes[0x02F] = KEY_V;
	m_keycodes[0x011] = KEY_W;
	m_keycodes[0x02D] = KEY_X;
	m_keycodes[0x015] = KEY_Y;
	m_keycodes[0x02C] = KEY_Z;
	m_keycodes[0x028] = KEY_APOSTROPHE;
	m_keycodes[0x02B] = KEY_BACKSLASH;
	m_keycodes[0x033] = KEY_COMMA;
	m_keycodes[0x00D] = KEY_EQUAL;
	m_keycodes[0x029] = KEY_GRAVE_ACCENT;
	m_keycodes[0x01A] = KEY_LEFT_BRACKET;
	m_keycodes[0x00C] = KEY_MINUS;
	m_keycodes[0x034] = KEY_PERIOD;
	m_keycodes[0x01B] = KEY_RIGHT_BRACKET;
	m_keycodes[0x027] = KEY_SEMICOLON;
	m_keycodes[0x035] = KEY_SLASH;
	m_keycodes[0x056] = KEY_WORLD_2;
	m_keycodes[0x00E] = KEY_BACKSPACE;
	m_keycodes[0x153] = KEY_DELETE;
	m_keycodes[0x14F] = KEY_END;
	m_keycodes[0x01C] = KEY_ENTER;
	m_keycodes[0x001] = KEY_ESCAPE;
	m_keycodes[0x147] = KEY_HOME;
	m_keycodes[0x152] = KEY_INSERT;
	m_keycodes[0x15D] = KEY_MENU;
	m_keycodes[0x151] = KEY_PAGE_DOWN;
	m_keycodes[0x149] = KEY_PAGE_UP;
	m_keycodes[0x045] = KEY_PAUSE;
	m_keycodes[0x146] = KEY_PAUSE;
	m_keycodes[0x039] = KEY_SPACE;
	m_keycodes[0x00F] = KEY_TAB;
	m_keycodes[0x03A] = KEY_CAPS_LOCK;
	m_keycodes[0x145] = KEY_NUM_LOCK;
	m_keycodes[0x046] = KEY_SCROLL_LOCK;
	m_keycodes[0x03B] = KEY_F1;
	m_keycodes[0x03C] = KEY_F2;
	m_keycodes[0x03D] = KEY_F3;
	m_keycodes[0x03E] = KEY_F4;
	m_keycodes[0x03F] = KEY_F5;
	m_keycodes[0x040] = KEY_F6;
	m_keycodes[0x041] = KEY_F7;
	m_keycodes[0x042] = KEY_F8;
	m_keycodes[0x043] = KEY_F9;
	m_keycodes[0x044] = KEY_F10;
	m_keycodes[0x057] = KEY_F11;
	m_keycodes[0x058] = KEY_F12;
	m_keycodes[0x064] = KEY_F13;
	m_keycodes[0x065] = KEY_F14;
	m_keycodes[0x066] = KEY_F15;
	m_keycodes[0x067] = KEY_F16;
	m_keycodes[0x068] = KEY_F17;
	m_keycodes[0x069] = KEY_F18;
	m_keycodes[0x06A] = KEY_F19;
	m_keycodes[0x06B] = KEY_F20;
	m_keycodes[0x06C] = KEY_F21;
	m_keycodes[0x06D] = KEY_F22;
	m_keycodes[0x06E] = KEY_F23;
	m_keycodes[0x076] = KEY_F24;
	m_keycodes[0x038] = KEY_LEFT_ALT;
	m_keycodes[0x01D] = KEY_LEFT_CONTROL;
	m_keycodes[0x02A] = KEY_LEFT_SHIFT;
	m_keycodes[0x15B] = KEY_LEFT_SUPER;
	m_keycodes[0x137] = KEY_PRINT_SCREEN;
	m_keycodes[0x138] = KEY_RIGHT_ALT;
	m_keycodes[0x11D] = KEY_RIGHT_CONTROL;
	m_keycodes[0x036] = KEY_RIGHT_SHIFT;
	m_keycodes[0x15C] = KEY_RIGHT_SUPER;
	m_keycodes[0x150] = KEY_DOWN;
	m_keycodes[0x14B] = KEY_LEFT;
	m_keycodes[0x14D] = KEY_RIGHT;
	m_keycodes[0x148] = KEY_UP;
	m_keycodes[0x052] = KEY_KP_0;
	m_keycodes[0x04F] = KEY_KP_1;
	m_keycodes[0x050] = KEY_KP_2;
	m_keycodes[0x051] = KEY_KP_3;
	m_keycodes[0x04B] = KEY_KP_4;
	m_keycodes[0x04C] = KEY_KP_5;
	m_keycodes[0x04D] = KEY_KP_6;
	m_keycodes[0x047] = KEY_KP_7;
	m_keycodes[0x048] = KEY_KP_8;
	m_keycodes[0x049] = KEY_KP_9;
	m_keycodes[0x04E] = KEY_KP_ADD;
	m_keycodes[0x053] = KEY_KP_DECIMAL;
	m_keycodes[0x135] = KEY_KP_DIVIDE;
	m_keycodes[0x11C] = KEY_KP_ENTER;
	m_keycodes[0x037] = KEY_KP_MULTIPLY;
	m_keycodes[0x04A] = KEY_KP_SUBTRACT;
#endif // _WIN32
}
//-----------------------------------------------------------------------------