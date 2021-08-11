#include <FastLED.h>
#include <LEDMatrix.h>
// Matrix dimensions and defines
#define NUM_STRIPS 28
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS
#define LEDtilewidth 4 // How much width each input controls
#define LEDtileheight 60 // how high each strip is
#define LEDtilehorz 7 // number of matrices arranged horizontally
#define LEDtilevert 1  // how many tiles stacked vertically
#define LEDstrips (LEDtilewidth*LEDtilehorz)  // How many LEDs across the array is
#define LEDper (LEDtileheight*LEDtilevert)    // How many LEDs are on each vertical strip
#define LEDsegm (LEDtilewidth*LEDper)         // The number of LEDs on each segment connected to the
#define LEDtotal (LEDstrips*LEDper)
#define NUMleds LEDtotal

cLEDMatrix<LEDtilewidth, LEDtileheight, VERTICAL_MATRIX, LEDtilehorz, LEDtilevert, VERTICAL_BLOCKS> ledmatrix;
CRGB *leds = ledmatrix[0];

uint32_t timer;
#define numbeads 500
uint16_t noise[numbeads][2];
byte hue;

CRGB currentPalette; // make this whatever custom palette you want (= heatcolors;)

void Gentle_Leviathan(){
  FastLED.clear();
  uint16_t lowbound = 14000;
  uint16_t hibound = 52000;

  for(int bead = 1; bead < numbeads; bead++){
    noise[bead][0] = inoise16(noise[bead][0], timer+bead*1000);     // get some noise
    noise[bead][0] = constrain(noise[bead][0], lowbound, hibound);               // keep it within a reasonable scalable boundary (aesthetic judgement)
    uint16_t xpos = map(noise[bead][0], lowbound, hibound, 0, LEDstrips);        // Map it to width

    noise[bead][1] = inoise16(noise[bead][1], timer*2+bead*1000); //random8() this was in the last arg
    noise[bead][1] = constrain(noise[bead][1], lowbound, hibound); 
    uint16_t ypos = map(noise[bead][1], lowbound, hibound, 0, LEDper);

    ledmatrix.DrawFilledCircle(xpos, ypos, 2, blend(ColorFromPalette(currentPalette, hue + bead*1, 255, LINEARBLEND), CRGB::White, 55));
  }

  blur2d(leds, LEDstrips, LEDper, 45);
  timer+=150;
}