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

#define _min(a,b) ((a)<(b)?(a):(b))
#define _max(a,b) ((a)>(b)?(a):(b))

typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef float f16;
typedef long f32;

#endif