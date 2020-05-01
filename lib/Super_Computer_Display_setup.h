//#include <../../../../lib/Super_Computer_Display_setup.h>

// Setup for ESP32 and ST7735 80 x 160 TFT specific to Ada's Super Computer

#define ST7735_DRIVER // Define additional parameters below for this display

#define TFT_WIDTH 80
#define TFT_HEIGHT 160

#define ST7735_GREENTAB160x80 // For 160 x 80 display (BGR, inverted, 26 offset)

#ifdef ESP32
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS 5   // Chip select control pin
#define TFT_DC 2   // Data Command control pin
#define TFT_RST -1 // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
#endif

#define LOAD_GLCD  // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2 // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4 // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6 // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7 // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8 // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
//#define LOAD_FONT8N // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

// Comment out the #define below to stop the SPIFFS filing system and smooth font code being loaded
// this will save ~20kbytes of FLASH
// #define SMOOTH_FONT

// #define SPI_FREQUENCY  20000000
#define SPI_FREQUENCY 27000000 // Actually sets it to 26.67MHz = 80/3
