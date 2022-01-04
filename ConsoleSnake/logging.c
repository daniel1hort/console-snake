#include "logging.h"

void log_error(int line, const char* function, const char* file, const char* message) {
	printf_s("%s at line %d in function %s: %s\n", PathFindFileNameA(file), line, function, message);
}

_Noreturn void log_error_and_terminate(int line, const char* function, const char* file, const char* message) {
	log_error(line, function, file, message);
	exit(-1);
}