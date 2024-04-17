#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "global.hpp"

class Logger {
public:
  static void debug(const char *message);
  static void error(const char *message);
  static void info(const char *message);
};
#endif
