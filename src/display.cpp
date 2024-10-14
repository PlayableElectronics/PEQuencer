#include "display.hpp"
#include "Adafruit_SH110X.h"

MacropadDisplay::MacropadDisplay() {
  display = new Adafruit_SH1106G(128, 64, 27, 26, 24, 23, 22);
}

MacropadDisplay::~MacropadDisplay() { delete display; }

void MacropadDisplay::initialize() {
  display->begin(0, true);
  display->setTextSize(1);
  display->setTextColor(SH110X_BLACK, SH110X_WHITE);
  this->clear();
};

void MacropadDisplay::clear() {
  display->clearDisplay();
}

void MacropadDisplay::print() {
  display->setCursor(2, 55);  // Ustawienie kursora
  display->print("FOOO");     // Wyświetlanie tekstu
  display->display(); 
}
