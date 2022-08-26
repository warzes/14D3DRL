#pragma once

enum class Key
{
	Up,
	Down,
	Right,
	Left,
	StrafeLeft,
	StrafeRight,
	A,
	B,
	C,
	Esc,

	// debug
#ifdef _DEBUG
	R,F,T,G,
#endif
};

#ifdef _DEBUG
constexpr size_t NumKey = 14;
#else
constexpr size_t NumKey = 10;
#endif

extern bool KeyDownState[NumKey];

void SetKeyState(Key key, bool state);
bool IsKeyDown(Key key);