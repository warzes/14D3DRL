#include "stdafx.h"
#include "PlatformWindowEvents.h"
//-----------------------------------------------------------------------------
void WindowEventIconified()
{
	std::string str = "window iconified";
	puts(str.c_str());
}
//-----------------------------------------------------------------------------
void WindowEventRestored()
{
	std::string str = "window restored";
	puts(str.c_str());
}
//-----------------------------------------------------------------------------
void WindowEventFocused()
{
	std::string str = "window focused";
	puts(str.c_str());
}
//-----------------------------------------------------------------------------
void WindowEventUnFocused()
{
	std::string str = "window unfocused";
	puts(str.c_str());
}
//-----------------------------------------------------------------------------
void WindowEventSize(int width, int height)
{
	std::string str = "window resize(";
	str += std::to_string(width);
	str += ":";
	str += std::to_string(height);
	str += ")";
	puts(str.c_str());
}
//-----------------------------------------------------------------------------
void MouseEventEnter()
{
	std::string str = "mouse enter";
	puts(str.c_str());
}
//-----------------------------------------------------------------------------
void MouseEventLeave()
{
	std::string str = "mouse leave";
	puts(str.c_str());
}
//-----------------------------------------------------------------------------
void MouseEventMove(int posX, int posY)
{
	std::string str = "mouse move(";
	str += std::to_string(posX);
	str += ":";
	str += std::to_string(posY);
	str += ")";
	//puts(str.c_str());
}
//-----------------------------------------------------------------------------