#pragma once

namespace utility
{
#if defined(_WIN32)
	inline bool Utf8ToWide(const char* src, wchar_t* dst, int dst_num_bytes)
	{
		assert(src && dst && (dst_num_bytes > 1));
		memset(dst, 0, (size_t)dst_num_bytes);
		const int dst_chars = dst_num_bytes / (int)sizeof(wchar_t);
		const int dst_needed = MultiByteToWideChar(CP_UTF8, 0, src, -1, 0, 0);
		if ((dst_needed > 0) && (dst_needed < dst_chars))
		{
			MultiByteToWideChar(CP_UTF8, 0, src, -1, dst, dst_chars);
			return true;
		}
		else
		{
			// input string doesn't fit into destination buffer
			return false;
		}
	}
#endif // _WIN32
}