#include <OctoWS2811.h>
#include <FastLED.h>
/* Global
const uint16_t numPins = 8;
//byte pinList[numPins] = {19,18,14,15,17,16,22,23,  3,4,6,7,8,9,10,11}; // Apparently any T4.x pins can be used in parallel
const uint8_t pinList[numPins] = {19,18,14,15,17,16,22,23}; // stock 4.0 parallel pins
const uint16_t ledsPerPin = 60;
/*
void setup(){
  octo.begin();
  pcontroller = new CTeensy4Controller<GRB, WS2811_800kHz>(&octo);
  FastLED.addLeds(pcontroller, writedata, numPins * ledsPerPin);
}
*/
// These buffers need to be large enough for all the pixels. The total number of pixels is "ledsPerPin * numPins".
// Each pixel needs 3 bytes, so multiply by 3.  An "int" is 4 bytes, so divide by 4.  The array is created using "int"
// so the compiler will align it to 32 bit memory.
DMAMEM int displayMemory[ledsPerPin * numPins * 3 / 4];
int drawingMemory[ledsPerPin * numPins * 3 / 4];
OctoWS2811 octo(ledsPerPin, displayMemory, drawingMemory, WS2811_RGB | WS2811_800kHz, numPins, pinList);
template <EOrder RGB_ORDER = RGB, uint8_t CHIP = WS2811_800kHz>
class CTeensy4Controller : public CPixelLEDController<RGB_ORDER, 8, 0xFF>{
  OctoWS2811 *pocto;
  public:
    CTeensy4Controller(OctoWS2811 *_pocto):pocto(_pocto){};
    virtual void init() {}
    virtual void showPixels(PixelController<RGB_ORDER, 8, 0xFF>&pixels){
      uint32_t i = 0;
      while (pixels.has(1)){
        uint8_t r = pixels.loadAndScale0();
        uint8_t g = pixels.loadAndScale1();
        uint8_t b = pixels.loadAndScale2();
        pocto->setPixel(i++, r, g, b);
        pixels.stepDithering();
        pixels.advanceData();
      }
      pocto->show();
    }
};
CTeensy4Controller<GRB, WS2811_800kHz> *pcontroller;