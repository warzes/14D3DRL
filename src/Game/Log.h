#pragma once

void LogPrint(const char* message);
void LogWarning(const char* message);
void LogError(const char* message);
void LogFatal(const char* message);

inline void LogPrint(const std::string& message)
{
	LogPrint(message.c_str());
}
inline void LogWarning(const std::string& message)
{
	LogWarning(message.c_str());
}
inline void LogError(const std::string& message)
{
	LogError(message.c_str());
}
inline void LogFatal(const std::string& message)
{
	LogFatal(message.c_str());
}