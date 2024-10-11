#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#include <Adafruit_SH110X.h>

class BasicDisplay {
public:
  virtual ~BasicDisplay() {}
  virtual void initialize() = 0;
  virtual void clear() = 0;
  virtual void print() = 0;
};

class MacropadDisplay : public BasicDisplay {
public:
  MacropadDisplay();
  ~MacropadDisplay() override;

  void initialize() override;
  void clear() override;
  void print() override;

private:
  Adafruit_SH1106G *display;
};

#endif
