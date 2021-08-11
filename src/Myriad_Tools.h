#ifndef Myriad_Tools_h
#define Myriad_Tools_h
#include <FastLED.h>
#include <Myriad_Types.h>
// Includes lots of core functionality
extern NamedPattern Pattern_List[];
extern NamedPalette Palette_List[];

#ifdef Myriad_Prefs
void peroidictasks(){
  EVERY_N_MILLIS(500){
    random16_add_entropy(analogRead(A5));  
    Serial.print("Pattern: ");
    Serial.print(Pattern_List[patternum].Name.c_str());
    Serial.print("  ---  Palette: ");
    if(palmatch == false){
      Serial.print(Palette_List[palnum].Name);
    }
    else {
      Serial.print("Match");
    }
    Serial.print("  ---  FPS: ");
    Serial.print(framerate);
    Serial.print("  ---  POWER: ");
    Serial.println(calculate_unscaled_power_mW(writedata, LEDtotal)/5*LEDcurbright);
    Prefs.update();
  }
}
#else
void peroidictasks(){
  EVERY_N_MILLIS(500){
    random16_add_entropy(analogRead(A5));  
    Serial.print("Pattern: ");
    Serial.print(Pattern_List[patternum].Name.c_str());
    Serial.print("  ---  Palette: ");
    if(palmatch == false){
      Serial.print(Palette_List[palnum].Name);
    }
    else {
      Serial.print("Match");
    }
    Serial.print("  ---  FPS: ");
    Serial.print(framerate);
    Serial.print("  ---  POWER: ");
    Serial.println(calculate_unscaled_power_mW(writedata, LEDtotal)/5*LEDcurbright);
    //Serial.print("  ---  FREEMEM: ");
    //Serial.println(ESP.getFreeHeap());
  }
}
#endif

uint16_t XY(uint16_t x, uint16_t y) {
  uint16_t LEDaddress = x * LEDper + y;
  return LEDaddress;
}

void huepusher(bool newPL, int8_t hueinc,  uint8_t huespeed) {
  if(newPL == false){
    EVERY_N_MILLIS(huespeed) {
      hue[0] += hueinc;
    }
  }
  if(newPL == true){
    EVERY_N_MILLIS(huespeed) {
      hue[1] += hueinc;
    }
  }
}

void fader(bool newPL, byte fadeval) {  // fades led array by given amount
  if (newPL == true){
    fadeToBlackBy( firstbuffer, LEDtotal, fadeval);    // fade first array if pattern is new
  } else if (newPL == false){
    fadeToBlackBy( secondbuffer, LEDtotal, fadeval);   // fade second array if pattern is old
  }
}

void blackout(CRGB *targetarray) {
  for ( int i = 0; i < LEDtotal; i++) {
    targetarray[i] = CRGB(0, 0, 0);
  }
}

void patchangeproc(int newpatnum){         // Every time you switch patterns run this to begin crossfading
  blackout(secondbuffer);                           //purge the old buffer
  memcpy8 (secondbuffer, firstbuffer, LEDtotal);    // copy data from first buffer to second, for continuity
  blackout(firstbuffer);                            // wipe the new buffer for incoming pattern
  crossct = 0;                  // reset the blend amount
  oldpattern = patternum;       // set the current pattern to be the old one so we can make it use the same variables
  patternum = newpatnum;        // save input to current pattern
  rowcount[0] = rowcount[1];    // copy row status to default rows, and reset them for the new pattern
  rowcount[1] = 0;
  colcount[0] = colcount[1];    // new pattern is 1, because it is TRUE with newPL
  colcount[1] = 0;
  //uint8_t noisesize = sizeof(noise[0]);
  //for (uint16_t i = 0; i < numnoise; i++){
  //  memcpy(noise[0], noise[1], noisesize);
  //}
  count[0] = count[1];
  count[1] = 0;
  hue[0] = hue[1];
  hue[1] = 0;
  Serial.print("New pattern : ");
  Serial.print(newpatnum);
  Serial.print(" - ");
  Serial.println(Pattern_List[patternum].Name);
}

extern const uint16_t NUMpatterns;
extern uint8_t palnum;
extern uint16_t patternselectID;
extern uint16_t paletteselectID;
void shuffler(){
  // Shuffle Pattern
  EVERY_N_SECONDS_I(shufloop, STATEpatshuffleinterval){
    if(patshuffle == true){
      patchangeproc(random8(NUMpatterns));     // Choose range that skips unwanted shuffles
    }
  }
  shufloop.setPeriod(STATEpatshuffleinterval);

  // Palette Shuffle
  if(palshuff == true){
    EVERY_N_SECONDS_I(palshufloop, STATEpalshuffleinterval) {
      palnum = random(NUMpalettes);
      targetPalette = Palette_List[palnum].Palette;
      Serial.print("New palette : ");
      Serial.print(palnum);
      Serial.print(" - ");
      Serial.println(Palette_List[palnum].Name);
    }
    palshufloop.setPeriod(STATEpalshuffleinterval);
  }
}

#ifdef ESP32Virtual
  void brightnessfader(){ // Move brightness to set value gradually
    EVERY_N_MILLIS(20){
      if(LEDonoff == true){                         // If leds are ssupposed to be on
        if (LEDcurbright != LEDtargbright ){        // and the current brightness is not at the target
          if(LEDcurbright < LEDtargbright){         // and if the current brightness is LOWER than target
            LEDcurbright++;                         // increase the current brightness
          }
          else if (LEDcurbright > LEDtargbright){   // Otherwise, if the current brightness is HIGHER than target
            LEDcurbright--;                         // decrease current brightness
          }
          driver.setBrightness(LEDcurbright);       // If we had to change current brightness, then apply change
        }
      }
      else if (LEDcurbright > 0){                   // If LEDonoff is set to off, and 
        LEDcurbright--;                             // reduce current brightness
        driver.setBrightness(LEDcurbright);         // and apply the change
      }
    }
  }
#else
  void brightnessfader(){ // Move brightness to set value gradually
    EVERY_N_MILLIS(20){
      if(LEDonoff == true){                         // If leds are ssupposed to be on
        if (LEDcurbright != LEDtargbright ){        // and the current brightness is not at the target
          if(LEDcurbright < LEDtargbright){         // and if the current brightness is LOWER than target
            LEDcurbright++;                         // increase the current brightness
          }
          else if (LEDcurbright > LEDtargbright){   // Otherwise, if the current brightness is HIGHER than target
            LEDcurbright--;                         // decrease current brightness
          }
          FastLED.setBrightness(LEDcurbright);      // If we had to change current brightness, then apply change
        }
      }
      else if (LEDcurbright > 0){                   // If LEDonoff is set to off, and 
        LEDcurbright--;                             // reduce current brightness
        FastLED.setBrightness(LEDcurbright);        // and apply the change
      }
    }
  }
#endif

void palettetargeting(TProgmemRGBGradientPalettePtr matchpal) {     // Select which palette behaviour
  if(palmatch == true){
    // If the palette is to match the pattern, then switch it immediately every time
    // Must swap other time to keep unique palettes during crossfading
    currentPalette = matchpal;
  }
  else {
    // Otherwise, if setting palettes manually, just set the target and allow it to be blended
    targetPalette = Palette_List[palnum].Palette;
    // Blend towards target
    nblendPaletteTowardPalette( currentPalette, targetPalette, blendspeed); // there are 2, because blendspeed is capped at ~48
    nblendPaletteTowardPalette( currentPalette, targetPalette, blendspeed);
  }
}

void patrunproc(bool newPL, byte fadeamt, int8_t hueinc, uint8_t huespeed, TProgmemRGBGradientPalettePtr procpal){
  // This function does applies all the settings that change between patterns
  huepusher(newPL, hueinc, huespeed);  // Increment the relevant hue interval
  fader(newPL, fadeamt);
  palettetargeting(procpal);
}

void fastruntasks(){
  EVERY_N_MILLIS(10){
    brightnessfader();
    shuffler();
  }
}

#ifdef ESP32Virtual
void crossfader() {  ////////////////////////// Crossfader //////////////////////////////////////////
  static uint32_t executiontime;
  EVERY_N_MILLIS_I(mainloop, STATEloopinterval){
    //uint32_t starttime = ESP.getCycleCount();
    if (crossct >= 255) { 
      Pattern_List[patternum].Pattern(true, firstbuffer);   // run only newest pattern if crossfading complete
    }
    else if (crossct < 255) {
      EVERY_N_MILLIS(20) {
        crossct += 1;           // higher increase faster xfade
      }
      Pattern_List[patternum].Pattern(true, firstbuffer);   // Run the newest pattern and save to array, bool true = newpat
      Pattern_List[oldpattern].Pattern(false, secondbuffer);    // Run the old pattern and save to array, bool false = oldpat
      //memcpy8 (leds2, leds, LEDtotal); // __attribute__((noinline))
    }
    for (uint16_t i = 0; i < LEDtotal; i++) {   // blend em
      writedata[i] = blend( secondbuffer[i], firstbuffer[i], crossct);   // Blend arrays of LEDs, third value is blend %
    }
    Vshow();
    framerate = framerate *.99 + .01*(1000000/(micros() - executiontime));
    executiontime = micros();
    //framerate = 240000000/(ESP.getCycleCount()-starttime); //millis;//(starttime-millis()); //moving average framerate
  }
  mainloop.setPeriod(STATEloopinterval);
}
/*
void crossfader() {  ////////////////////////// Crossfader //////////////////////////////////////////
  EVERY_N_MILLIS_I(mainloop, STATEloopinterval){
    if (crossct >= 255) { 
      Pattern_List[patternum].Pattern(true);   // run completed pattern only when fading is complete
    }
    else if (crossct < 255) {
      EVERY_N_MILLIS(20) {
        crossct += 1;           // higher increase faster xfade
      }
      Pattern_List[oldpattern].Pattern(false);    // Run the old pattern and save to array, bool false = oldpat
      //memcpy8 (leds2, leds, LEDtotal); // __attribute__((noinline))
      leds2 = leds;
      Pattern_List[patternum].Pattern(true);   // Run the new pattern and save to array, bool true = newpat
      for (uint16_t i = 0; i < LEDtotal; i++) {   // blend em
        leds3[i] = blend( leds[i], leds2[i], crossct);   // Blend arrays of LEDs, third value is blend %
      }
    }
    VIRTUALshow();
  }
  mainloop.setPeriod(STATEloopinterval);
}*/
#else
void crossfader() {  ////////////////////////// Crossfader //////////////////////////////////////////
  EVERY_N_MILLIS_I(mainloop, STATEloopinterval){
    if (crossct >= 255) { 
      Pattern_List[patternum].Pattern(true);   // run completed pattern only when fading is complete
    }
    else if (crossct < 255) {
      EVERY_N_MILLIS(20) {
        crossct += 1;           // higher increase faster xfade
      }
      Pattern_List[oldpattern].Pattern(false);    // Run the old pattern and save to array, bool false = oldpat
      //for (uint16_t i = 0; i < LEDtotal; i++) {   // blend em
      //  leds2[i] = leds[i];   // Blend arrays of LEDs, third value is blend %
      //}
      //memcpy8 (leds2, leds, LEDtotal); // __attribute__((noinline))
      leds2 = leds;
  
      Pattern_List[patternum].Pattern(true);   // Run the new pattern and save to array, bool true = newpat
      for (uint16_t i = 0; i < LEDtotal; i++) {   // blend em
        leds[i] = blend( leds2[i], leds[i], crossct);   // Blend arrays of LEDs, third value is blend %
      }
    }
  FastLED.show();
  framerate = FastLED.getFPS();
  }
  mainloop.setPeriod(STATEloopinterval);
}
#endif

// blurColumns: perform a blur1d on each column of a rectangular matrix
void blurColumnsv3(CRGB* leds, uint16_t height, uint16_t width, fract8 blur_amount) {
  // blur columns
  uint8_t keep = 255 - blur_amount;
  uint8_t seep = blur_amount >> 1;

  for ( uint16_t col = 0; col < width; col++) {
    CRGB carryover = CRGB::Black;
    for ( uint16_t i = 0; i < height; i++) {
      //CRGB cur = leds[XY(col,i)];
      CRGB cur = leds[XY(i, col)];
      CRGB part = cur;
      part.nscale8( seep);
      cur.nscale8( keep);
      cur += carryover;
      //if( i) leds[XY(col,i-1)] += part;
      if ( i) leds[XY(i - 1, col)] += part;
      //leds[XY(col,i)] = cur;
      leds[XY(i, col)] = cur;
      carryover = part;
    }
  }
}

void blurRows1( CRGB* leds, uint16_t width, uint16_t height, fract8 blur_amount){
/*    for( uint8_t row = 0; row < height; row++) {
        CRGB* rowbase = leds + (row * width);
        blur1d( rowbase, width, blur_amount);
    }
*/
    // blur rows same as columns, for irregular matrix
    uint8_t keep = 255 - blur_amount;
    uint8_t seep = blur_amount >> 1;
    for( uint16_t row = 0; row < height; row++) {
        CRGB carryover = CRGB::Black;
        for( uint16_t i = 0; i < width; i++) {
            CRGB cur = leds[XY(i,row)];
            CRGB part = cur;
            part.nscale8( seep);
            cur.nscale8( keep);
            cur += carryover;
            if( i) leds[XY(i-1,row)] += part;
            leds[XY(i,row)] = cur;
            carryover = part;
        }
    }
}

void blur(CRGB* leds, uint16_t height, uint16_t width, fract8 blur_amount) { //  leds[i]*=2; to multiply brightness
  blurColumnsv3( leds, height, width, blur_amount);
  blurRows1( leds, height, width, blur_amount * 0.89);         //.86 seems to work
}
/*
extern uint8_t LEDcurbright;
void LEDsetup(){
  FastLED.setBrightness(LEDcurbright);
  //FastLED.setMaxRefreshRate(125);
  FastLED.addLeds<WS2813, LEDpin, GRB>(leds, 0, LEDtotal);
  FastLED.clear(true);
  LEDhalfstrips = max(1, LEDstrips/2);
  LEDhalfper = max(1, LEDper/2);
  Pattern_List[patternum].Pattern(true);
}*/

////////////////////////// Colour Conversion //////////////////////////////////////////
// uint16_t txtcolor = Color24toColor16(Wheel(map(letters[l], '0', 'Z', 255, 0)));

uint16_t Color24toColor16(uint32_t color) {
  return ((uint16_t)(((color & 0xFF0000) >> 16) & 0xF8) << 8) |
         ((uint16_t)(((color & 0x00FF00) >>  8) & 0xFC) << 3) |
         (((color & 0x0000FF) >>  0)         >> 3);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  uint32_t wheel = 0;

  // Serial.print(WheelPos);
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    wheel = (((uint32_t)(255 - WheelPos * 3)) << 16) + (WheelPos * 3);
  }
  if (!wheel && WheelPos < 170) {
    WheelPos -= 85;
    wheel = (((uint32_t)(WheelPos * 3)) << 8) + (255 - WheelPos * 3);
  }
  if (!wheel) {
    WheelPos -= 170;
    wheel = (((uint32_t)(WheelPos * 3)) << 16) + (((uint32_t)(255 - WheelPos * 3)) << 8);
  }
  // Serial.print(" -> ");
  // Serial.println(wheel, HEX);
  return (wheel);
}

#endif
