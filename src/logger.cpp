#include "logger.hpp"

void Logger::debug(const char *message) { printf("[DEBUG] -> %s\n", message); }

void Logger::error(const char *message) { printf("[ERROR] -> %s\n", message); }

void Logger::info(const char *message) { printf("[INFO] -> %s\n", message); }
