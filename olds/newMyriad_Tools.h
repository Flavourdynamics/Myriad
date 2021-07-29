#ifndef Myriad_Tools_h
#define Myriad_Tools_h
// Includes lots of core functionality
#include <Arduino.h>
//#define ESP32Virtual // For intellisense testing
#include <FastLED.h>


using pattern_func = void (*)(bool newpatbool);
typedef struct {
  pattern_func Pattern;
  const String Name;
} NamedPattern;
extern NamedPattern Pattern_List[];
extern const uint16_t NUMpatterns;
extern const uint16_t NUMpalettes;

extern bool LEDonoff;
extern uint8_t LEDtargbright;
extern uint8_t LEDcurbright;
extern uint8_t patternum;
extern uint8_t palnum;
extern uint16_t STATEloopinterval;
extern uint16_t STATEpatshuffleinterval;
extern uint16_t STATEpalshuffleinterval;
extern bool patshuffle;
extern bool palshuff;
extern bool palmatch;
extern uint32_t framerate;
extern uint16_t LEDhalfstrips;
extern uint16_t LEDhalfper;
extern uint8_t hue[];
extern uint8_t blendspeed;
extern uint8_t targfade;
extern uint8_t currfade;
extern uint8_t crossct;
extern int16_t rowcount[];          // For patterns that roll out
extern int16_t colcount[];
extern int16_t count[];
extern uint8_t oldpattern;
extern CRGB leds[];
extern CRGB leds2[];

extern CRGBPalette16 targetPalette;
extern CRGBPalette16 currentPalette;

class Myriad_Tools{
  public:

};

// LEDs
void LEDsetup();
uint16_t XY(uint16_t x, uint16_t y);
void fader(bool newPL, byte fadeval);
void blackout();
void brightnessfader();
#ifdef ESP32Virtual

#endif

// Paterns and runtime
void peroidictasks();
void fastruntasks();
void shuffler();
void palettetargeting(TProgmemRGBGradientPalettePtr matchpal);
void patcrossproc(int newpatnum);
void patrunproc(bool newPL, byte fadeamt, int8_t hueinc, uint8_t huespeed, TProgmemRGBGradientPalettePtr procpal);
void crossfader();

//////////////////////////// Blur ////////////////////////////////
// blurColumns: perform a blur1d on each column of a rectangular matrix
void blurColumnsv3(CRGB* leds, uint16_t height, uint16_t width, fract8 blur_amount);
void blurRows1( CRGB* leds, uint16_t width, uint16_t height, fract8 blur_amount);
void blur(CRGB* leds, uint16_t height, uint16_t width, fract8 blur_amount);

////////////////////////// Colour Conversion //////////////////////////////////////////
// uint16_t txtcolor = Color24toColor16(Wheel(map(letters[l], '0', 'Z', 255, 0)));
uint16_t Color24toColor16(uint32_t color);
uint32_t Wheel(byte WheelPos); // Input a value 0 to 255 to get a color value.

#endif
