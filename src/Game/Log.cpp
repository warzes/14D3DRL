#include "stdafx.h"
#include "Log.h"
//-----------------------------------------------------------------------------
namespace
{
	FILE* LogFile = nullptr;
}
//-----------------------------------------------------------------------------
void InitLogFile(const char* fileName)
{

}
//-----------------------------------------------------------------------------
void CloseLogFile()
{

}
//-----------------------------------------------------------------------------
void LogPrint(const char* message)
{
#if defined(__ANDROID__)
	__android_log_write(ANDROID_LOG_INFO, "SE_APP", message);
#else
	puts(message);
	OutputDebugStringA(message);
#endif
}
//-----------------------------------------------------------------------------
void LogWarning(const char* message)
{
	const std::string text = "[ \033[33mWARNING\033[0m ] : " + std::string(message);
	LogPrint(text.c_str());
}
//-----------------------------------------------------------------------------
void LogError(const char* message)
{
	const std::string text = "[ \033[31mERROR\033[0m   ] : " + std::string(message);
	LogPrint(text.c_str());
}
//-----------------------------------------------------------------------------
void LogFatal(const char* message)
{
	const std::string text = "[ \033[31mFATAL\033[0m   ] : " + std::string(message);
	LogPrint(text.c_str());
	IsEngineRun = false;
}
//-----------------------------------------------------------------------------