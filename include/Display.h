#ifndef Display_h

#define Display_h

#include <Arduino.h>
#include <State.h>
#include <vector>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

extern const bool useDisplay;
extern State state;

class Display
{
public:
  void begin();
  void loop();
  void printMessage(String line);

private:
  TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
  std::vector<String> lines;
  void show();
  unsigned long previousMillis = 0;
  const long interval = 500;
  const uint textWidth = 26;
  const uint32_t backgroundColour =  TFT_CYAN; //TFT_BLACK
};

#endif
