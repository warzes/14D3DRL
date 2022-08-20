#include "stdafx.h"
#include "GameInput.h"
//-----------------------------------------------------------------------------
bool KeyDownState[NumKey] = {false};
//-----------------------------------------------------------------------------
void SetKeyState(Key key, bool state)
{
	KeyDownState[static_cast<size_t>(key)] = state;
}
//-----------------------------------------------------------------------------
bool IsKeyDown(Key key)
{
	return KeyDownState[static_cast<size_t>(key)];
}
//-----------------------------------------------------------------------------