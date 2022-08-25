#pragma once

template <typename T>
inline T clamp(T value, T min, T max)
{
	return std::max(min, std::min(value, max));
}

inline int odd(int number)
{
	return number / 2 * 2 + 1;
}

inline int even(int number)
{
	return number / 2 * 2;
}