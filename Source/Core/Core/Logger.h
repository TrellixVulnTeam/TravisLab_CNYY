#pragma once
#include <functional>
#include "StdTypes.h"

#define LOG_SEVERITY(msg, severity, ...) Core::Log(Core::LogSeverity::severity, msg, ##__VA_ARGS__)
#define LOG_E(x, ...) LOG_SEVERITY(x, Error, ##__VA_ARGS__)
#define LOG_W(x, ...) LOG_SEVERITY(x, Warning, ##__VA_ARGS__)
#define LOG_I(x, ...) LOG_SEVERITY(x, Info, ##__VA_ARGS__)

#if ENABLED(DEBUG_LOGGING)
#define LOG_D(x, ...) LOG_SEVERITY(x, Debug, ##__VA_ARGS__)
#define LOG_H(x, ...) LOG_SEVERITY(x, HOT, ##__VA_ARGS__)
#else
#define LOG_D(x, ...)
#define LOG_H(x, ...)
#endif

namespace Core
{
enum class LogSeverity
{
	HOT = 0,
	Debug = 1,
	Info = 2,
	Warning = 3,
	Error = 4
};

extern LogSeverity g_MinLogSeverity;

void Log(LogSeverity severity, const char* pText, ...);

String ParseLogSeverity(LogSeverity severity);
LogSeverity ParseLogSeverity(const String& serialised);

void StartFileLogging(String path, u8 backupCount, String header);
void StopFileLogging();
} // namespace Core
