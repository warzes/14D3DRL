#pragma once

enum class Key
{
	Up,
	Down,
	Right,
	Left,
	A,
	B,
	C,
};

constexpr size_t NumKey = 7;

extern bool KeyDownState[NumKey];

void SetKeyState(Key key, bool state);

bool IsKeyDown(Key key);