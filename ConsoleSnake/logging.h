#ifndef __LOGGING_H__
#define __LOGGING_H__

#pragma comment(lib, "Shlwapi.lib")

#include <stdio.h>
#include <windows.h>
#include <shlwapi.h>

#define LOG_ERROR(message) log_error(__LINE__, __FUNCTION__, __FILE__, message);
#define LOG_ERROR_AND_TERMINATE(message) log_error_and_terminate(__LINE__, __FUNCTION__, __FILE__, message);

extern void log_error(int line, const char* function, const char* file, const char* message);
extern void log_error_and_terminate(int line, const char* function, const char* file, const char* message);

#endif
