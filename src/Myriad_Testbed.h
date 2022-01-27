#ifndef Myriad_Testbed_h
#define Myriad_Testbed_h
/* Pattern adding proceedure
    1. Add palette targetting, fetcher, fader/blackout, and huepusher to pattern
    2. Add to pattern list in Patterns.h
    3. Add value to default fader
*/
/* This file is where new patterns can be tested directly. The following functions may need to be included
    palettetargeting(12);
    uint8_t z = fetcher(18); 
    blackout();
    huepusher(-1, z, 16);
*/
/* Existing global pattern variables
    int16_t rowcount[2];          // For patterns that roll out
    int16_t colcount[2];
    int16_t count[2];
    bool patshuffle = false;
    uint16_t noise[30][2];
    uint16_t timer;
    uint8_t stars[LEDSx]; // Deprecated, probably
    uint8_t beat = 144;
*/

const uint8_t numtargs = 50;
struct targets {
  uint8_t x;
  uint8_t y;
  uint8_t direction;
  CHSV colour;
};
struct targets target[numtargs][2];

void beach(){
  EVERY_N_MILLIS(50){
    fadeToBlackBy(leds, LEDStotal, 20);
    //fader(20);
    uint8_t z = 1;
    //CRGBPalette16 palette = RainbowColors_p;
    
    for(int bead = 0; bead < 5; bead++){
      target[bead][z].x = beatsin8( (bead+1) * 4, 0, (LEDSx)-1);
      target[bead][z].y = beatsin8( (bead+1) * 3, 0, LEDSy - 1);   
      ledmatrix.DrawFilledCircle(target[bead][z].x, target[bead][z].y, 2, CHSV(hue[z]+bead*15, 255, 255));  // void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color)
    }
  }
  huepusher(1, 1, 24);
}
/*
void DNA(){
  uint8_t threads = 4;
  uint8_t phaseoff = 255/threads;
  for(int y = 0; y < LEDSy; y++){ // For each row
    for(uint8_t p = 0; p < threads; p++){                        // p is phase mod, 128 coefficient is 64 degrees out of phase
      // Format: leds[coordinates oscillate between left and right with next lines out of phase] =  CHSV(base hue+difference between rows, saturation changes a little bit for variety, value varies by row)
      // Increase coefficients of 'y' terms to increase the differences between rows
      //leds[XY(beatsin8(10, 0, LEDSx-1, 0, y*4+p*phaseoff), y)] = CHSV(-hue+y*5+p*phaseoff, beatsin8(25, 200, 255, 0, y*3), beatsin8(5, 55, 255, 0, y*10+p*phaseoff)); 

      leds[XY(beatsin8(10, 0, LEDSx-1, 0, y*4+p*phaseoff), y)] = CHSV(y*5+p*phaseoff-hue, 255, beatsin8(5, 55, 255, 0, y*10+p*phaseoff));                 
    }
  }
  blur( leds, LEDSx, LEDSy, 255);
}*/

uint8_t menoise[2];
uint8_t targetto[50][2];
uint8_t count0;

void midearth(){
  blackout();
  uint8_t items = 3;
  uint8_t rollover = count0;
  
  menoise[0] = inoise8(menoise[0], timer);           // get some noise
  menoise[0] = constrain(menoise[0], 60, 195);
  targetto[count0][0] = map(menoise[0], 60, 195, 0, LEDSx-1);        // Map it to width

  menoise[1] = inoise8(menoise[1], timer*2); 
  menoise[1] = constrain(menoise[1], 60, 195);
  targetto[count0][1] = map(menoise[1], 60, 195, 0, LEDSy-1);
  
  for(byte i = items + 1; i > 1; i--){
    //target[rollover][0], targetto[rollover][1]
    ledmatrix.DrawCircle(targetto[rollover+i][0], targetto[rollover+i][1], i*3, CHSV(hue[1] + rollover*5, 255, 255)); //  CHSV(hue + items*3, 255, 255)
    //ledmatrix.DrawCircle(target[rollover][0], target[rollover][1], i*2, CHSV(hue * 128, 0, 255)); //  CHSV(hue + items*3, 255, 255)
    //matrix->drawCircle(targetto[rollover][0], targetto[rollover][1], items*3, hue[1]+items*5);  // void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color)
    rollover--;
    if(rollover > items){ 
      rollover = items;
    }
  }
  count0++;
  if (count0 >= items){
    count0 = 0;
  }
  EVERY_N_MILLIS(20){
      timer+=1; 
  }
  huepusher(1, 1, 24);// huepusher(1, z, 24);   
  //hue[1]-=1;
  FastLED.show();
  //matrix->show();
}
/*
void midearthorig(){
  matrix->clear();
  uint8_t items = 20;
  uint8_t rollover = count;

  menoise[0] = inoise8(noise[0], timer);           // get some noise
  menoise[0] = constrain(noise[0], 60, 195);
  target[count][1].x = map(noise[0], 60, 195, 0, LEDSx-1);        // Map it to width

  menoise[1] = inoise8(noise[1], timer*2); 
  menoise[1] = constrain(noise[1], 60, 195);
  target[count][1].y = map(noise[1], 60, 195, 0, LEDSy-1);
  
  for(byte i = items+ 1; i > 1; i--){
    //target[rollover][0], target[rollover][1]
    ledmatrix.DrawCircle(target[rollover][1].x, target[rollover][1].y, i*2, CHSV(hue[1] + rollover*5, 255, 255)); //  CHSV(hue + items*3, 255, 255)
    //ledmatrix.DrawCircle(target[rollover][0], target[rollover][1], i*2, CHSV(hue * 128, 0, 255)); //  CHSV(hue + items*3, 255, 255)
    //matrix->drawCircle(target[rollover][0], target[rollover][1], items*3, hue+items*50);  // void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color)
    rollover--;
    if(rollover > items){ 
      rollover = items;
    }
  }
  huepusher(1, 1, 24);
  EVERY_N_MILLIS(100){
    count++;
    if (count > items){
      count = 0;
    }
  }
  EVERY_N_MILLIS(100){
    timer+=20;
  }
}*/

void testillusion(){
  palettetargeting(4);
  uint8_t z = fetcher(4);
  
  for (int y = 0; y < LEDSy/2; y++){
    for (int x = 0; x <LEDSx/2; x++){
      leds[XY(x, y)] = ColorFromPalette(currentPalette, hue[z] + x*25 + y*25, 255, LINEARBLEND);                            //top left
      leds[XY(LEDSx-1-x, y)] = ColorFromPalette(currentPalette, hue[z] + x*25 + y*25, 255, LINEARBLEND);              //top right
      leds[XY(x, LEDSy-1-y)] = ColorFromPalette(currentPalette, hue[z] + x*25 + y*25, 255, LINEARBLEND);                 //bottom left
      leds[XY(LEDSx-1-x, LEDSy-1-y)] = ColorFromPalette(currentPalette, hue[z] + x*25 + y*25, 255, LINEARBLEND);   //bottom right 
    }
  }
  huepusher(1, z, 16);
}

void testbed(){
  testillusion();
  //DNA();
  //midearth();
  //beach();
  FastLED.show();
}

#endif