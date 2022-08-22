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
};

constexpr size_t NumKey = 9;

extern bool KeyDownState[NumKey];

void SetKeyState(Key key, bool state);
bool IsKeyDown(Key key);