#ifndef Myriad_Config_h // Contains settings for Myriad components
#define Myriad_Config_h

////////// Comment out uncessary components //////////
#define Myriad_2D_Patterns  // LED patterns designed for a two dimensional LED matrix
#define Myriad_EQ           // Processes audio data
#define Myriad_Display      // Functions for using connected I2C displays
#define Myriad_Arrays       // Static array database of images and sprites for display on a 2D matrix
//#define Myriad_Gyro         // Functions for reading and processing gyroscope/accelerometer data
#define Myriad_Fonts        // Fonts database for printing text 
#define Myriad_BT           // Provides bluetooth serial data processing
//#define Myriad_Testbed      // A file for testing patterns in the full environment

////////// FastLED Setup //////////
#define FASTLED_INTERNAL  // Hides fastled compiler info
#define FASTLED_ESP32_I2S true  // Engages the I2S driver
//#define FASTLED_RMT_MAX_CHANNELS 8
//#define FASTLED_RMT_MEM_BLOCKS 2
#include <FastLED.h>
#include <LEDMatrix.h>
#include <FastLED_NeoMatrix.h>
#include <Framebuffer_GFX.h>

#define LEDtilewidth 4 // How much width each input controls
#define LEDtileheight 60 // how high each strip is
#define LEDtilehorz 7 // number of matrices arranged horizontally
#define LEDtilevert 1  // how many tiles stacked vertically
#define LEDstrips (LEDtilewidth*LEDtilehorz)  // How many LEDs across the array is
#define LEDper (LEDtileheight*LEDtilevert)    // How many LEDs are on each vertical strip
#define LEDsegm (LEDtilewidth*LEDper)         // The number of LEDs on each segment connected to the
#define LEDtotal (LEDstrips*LEDper)

// Compatibility for NeoMatrix
#define mw LEDstrips
#define mh LEDper
#define NUMMATRIX LEDtotal
// Compatibility for some other demos
#define NUM_LEDS LEDtotal
#define MATRIX_WIDTH LEDstrips
#define MATRIX_HEIGHT LEDper

// Create two LED array objects for crossfading
cLEDMatrix<LEDtilewidth, LEDtileheight, VERTICAL_MATRIX, LEDtilehorz, LEDtilevert , VERTICAL_BLOCKS> ledmatrix;
CRGB *leds = ledmatrix[0];
cLEDMatrix<LEDtilewidth, LEDtileheight, VERTICAL_MATRIX, LEDtilehorz, LEDtilevert , VERTICAL_BLOCKS> ledmatrix2;
CRGB *leds2 = ledmatrix2[0];

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, LEDtilewidth, LEDtileheight, LEDtilehorz, LEDtilevert,
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE + NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_PROGRESSIVE);

// LED Settings
#define LEDtype WS2813
#define LEDcorr TypicalSMD5050
#define LEDdither 255     //try 0 to reduce flickering
#define LEDonoff = true;
uint8_t LEDcurbright = 0;
uint8_t LEDtargbright = 5;
uint16_t LEDamps = 2000;

// Fader
#define deffade 5         // Default fade amt
uint8_t currfade = deffade; // Initialize variable fader
uint8_t targfade;           // Value for storing saving 
uint8_t faderlookup[] = {deffade, 255, deffade, 255, deffade, deffade, deffade, 20, 20, 128, deffade, 20, deffade, deffade, deffade, 10, deffade, deffade, deffade, 128, deffade, deffade, 255};
/////////////////////////0////////1/////2////////3/////4////////5////////6//////7///8///9////10///////11///12//////13/////////14/////15//16///////17////////18//////19///20///////21///////22

// Palette
CRGBPalette16 currentPalette;   // This is the palette used by patterns
CRGBPalette16 targetPalette;    // This palette is used to store a target palette for blending
uint8_t palnum = 0;             // change back to 0
uint8_t blendspeed = 35;        // How fast the palette blends (higher is faster)
uint8_t hue[2];                 // Hue value as an array to be used during crossfading

// Pattern variables, these are arrays to handle two independent values during crossfading
uint8_t patternum = 0;    // For keeping track of what patterns are running. Default = 0
uint8_t oldpattern;       // For storing what pattern we're fading away from on swap
uint16_t crossct = 255;   // This may need to be 16 bits, or could be trimmed to 8 maybe
int16_t rowcount[2];          // For patterns that roll out
int16_t colcount[2];
int16_t count[2];
bool patshuffle = false;
uint16_t noise[30][2];
uint16_t timer;
uint8_t stars[LEDstrips]; // Deprecated, probably
uint8_t beat = 144;

//State
uint8_t STATEreadinterval = 20;
uint8_t STATEloopinterval = 8;
uint8_t STATEpatshuffleinterval = 10;
uint16_t STATEpalshuffleinterval = 10;

//Pattern Key
typedef void (*SimplePatternList[])();
uint8_t PATTERNnum = 13; // Index number of which pattern is current
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Core 0 Setup
TaskHandle_t Core0;
bool frameready = false;  // Perhaps for splitting the creating and writing of frames

////////// Core components //////////
#include "Myriad_Palettes.h"
#include "Myriad_Tools.h"

#ifdef Myriad_Fonts
  #include "Myriad_Fonts.h"
#endif
#ifdef Myriad_Arrays
  #include "Myriad_Arrays.h"
#endif
#ifdef Myriad_BT
  #include <BluetoothSerial.h>
  // Bluetooth
  BluetoothSerial Bluetooth;
  const byte BTnumchars = 20;
  char BTreceivedchars[BTnumchars];
  bool BTnewdata = false;
  char BTprimary[BTnumchars] = {0};
  int BTsecondary;
  char BTmessage[BTnumchars];
  uint8_t BTupcount;
  #include "Myriad_BT.h"
#endif
#ifdef Myriad_EQ
  #include "Myriad_EQ.h"
#endif
#ifdef Myriad_2D_Patterns
  #include "Myriad_2D_Patterns.h"
#endif
#ifdef Myriad_Display
  #include "Myriad_Display.h"
#endif
#ifdef Myriad_Gyro
  #include "Myriad_Gyro.h"
#endif

#ifdef Myriad_Testbed
  #include "Myriad_Testbed.h"
#endif

#endif