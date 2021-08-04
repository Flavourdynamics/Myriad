#ifndef Myriad_Types_h
#define Myriad_Types_h
#include <FastLED.h>
struct NamedPalette {
  const TProgmemRGBGradientPalettePtr Palette;
  const String Name;

  NamedPalette(TProgmemRGBGradientPalettePtr p, const String n)
    : Palette(p), Name(n) {}
};

typedef void (*SimplePatternList[])();
using pattern_func = void (*)(bool newpatbool, CRGB *dest);
typedef struct {
  pattern_func Pattern;
  const String Name;
} NamedPattern;

extern NamedPalette Palette_List[];
extern NamedPattern Pattern_List[];
extern const uint16_t NUMpalettes;
#endif