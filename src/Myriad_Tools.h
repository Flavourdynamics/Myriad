#ifndef Myriad_Tools_h
#define Myriad_Tools_h
#include <FastLED.h>
#include <Myriad_Types.h>
// Includes lots of core functionality
extern NamedPattern Pattern_List[];
extern NamedPalette Palette_List[];

void SDwriteCSV();
void peroidictasks(){ // Run every 500ms
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
    Serial.println(calculate_unscaled_power_mW(writedata, LEDStotal)/5*LEDcurbright);
    #ifdef Myriad_Prefs
      Prefs.update();
    #endif
    #ifdef TEENSY
    EVERY_N_SECONDS(30){
      SDwriteCSV();
    }
    #endif
  }
}

uint16_t XY(uint16_t x, uint16_t y) { // Map XY coordiate to a linear LED address
  uint16_t LEDaddress = x * LEDSy + y;
  return LEDaddress;
}

uint16_t XY(uint16_t x, uint16_t y, uint8_t scalor){ // Superscaling XY targeting
  uint16_t LEDaddress = x * LEDSy * scalor + y;
  return LEDaddress;
}

void huepusher(bool newPL, int8_t hueinc,  uint8_t huespeed){ // Advance appropriate hue
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
    fadeToBlackBy( firstbuffer, LEDStotal, fadeval);    // fade first array if pattern is new
  } else if (newPL == false){
    fadeToBlackBy( secondbuffer, LEDStotal, fadeval);   // fade second array if pattern is old
  }
}

void blackout(CRGB *targetarray) { // wipes target array
  for ( int i = 0; i < LEDStotal; i++) {
    targetarray[i] = CRGB(0, 0, 0);
  }
}

void patchangeproc(int newpatnum){ // Every time you switch patterns run this to begin crossfading
  blackout(secondbuffer);                           //purge the old buffer
  memcpy8 (secondbuffer, firstbuffer, LEDStotal);    // copy data from first buffer to second, for continuity
  blackout(firstbuffer);                            // wipe the new buffer for incoming pattern
  crossct = 0;                  // reset the blend amount
  oldpattern = patternum;       // set the current pattern to be the old one so we can make it use the same variables
  patternum = newpatnum;        // save input to current pattern
  rowcount[0] = rowcount[1];    // copy row status to default rows, and reset them for the new pattern
  rowcount[1] = 0;
  colcount[0] = colcount[1];    // new pattern is 1, because it is TRUE with newPL
  colcount[1] = 0;
  uint16_t noisesize = sizeof(noise[0]);
  //for (uint16_t i = 0; i < numnoise; i++){
  memcpy(&noise[0], &noise[1], noisesize);
  memcpy(&targets[0], &targets[1], noisesize);
  //}
  //target
  //noise[2][numnoise][2]
  //target targets[2][numnoise]

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
    //nblendPaletteTowardPalette( currentPalette, targetPalette, blendspeed);
  }
}

void patrunproc(bool newPL, byte fadeamt, int8_t hueinc, uint8_t huespeed, TProgmemRGBGradientPalettePtr procpal){ // Run every time a pattern is called
  // This function does applies all the settings that change between patterns
  huepusher(newPL, hueinc, huespeed);  // Increment the relevant hue interval
  fader(newPL, fadeamt);
  palettetargeting(procpal);
}

void fastruntasks(){  // Every 10 ms
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
      //memcpy8 (leds2, leds, LEDStotal); // __attribute__((noinline))
    }
    for (uint16_t i = 0; i < LEDStotal; i++) {   // blend em
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
  if (crossct >= 255) { 
    Pattern_List[patternum].Pattern(true);   // run completed pattern only when fading is complete
  }
  else if (crossct < 255) {
    EVERY_N_MILLIS(20) {
      crossct += 1;           // higher increase faster xfade
    }
    Pattern_List[oldpattern].Pattern(false);    // Run the old pattern and save to array, bool false = oldpat
    //memcpy8 (leds2, leds, LEDStotal); // __attribute__((noinline))
    leds2 = leds;
    Pattern_List[patternum].Pattern(true);   // Run the new pattern and save to array, bool true = newpat
    for (uint16_t i = 0; i < LEDStotal; i++) {   // blend em
      leds3[i] = blend( leds[i], leds2[i], crossct);   // Blend arrays of LEDs, third value is blend %
    }
  }
  VIRTUALshow();
}*/
#else
void crossfader() {  ////////////////////////// Crossfader //////////////////////////////////////////
  if (crossct >= 255) { 
    // This can be optimized to not copy
    Pattern_List[patternum].Pattern(true, firstbuffer);   // run only newest pattern if crossfading complete
    for (uint16_t i = 0; i < LEDStotal; i++) {   // Need to use writedata for continuity
      writedata[i] = firstbuffer[i];   // Blend arrays of LEDs, third value is blend %
    }
    napplyGamma_video(writedata, LEDStotal, STATEpalshuffleinterval/10);
  }
  else if (crossct < 255) {
    EVERY_N_MILLIS(20) {
      crossct += 1;           // higher increase faster xfade
    }
    Pattern_List[patternum].Pattern(true, firstbuffer);   // Run the newest pattern and save to array, bool true = newpat
    napplyGamma_video(firstbuffer, LEDStotal, STATEpalshuffleinterval/10);
    Pattern_List[oldpattern].Pattern(false, secondbuffer);    // Run the old pattern and save to array, bool false = oldpat
    napplyGamma_video(secondbuffer, LEDStotal, STATEpalshuffleinterval/10);
    //memcpy8 (leds2, leds, LEDStotal); // __attribute__((noinline))
    for (uint16_t i = 0; i < LEDStotal; i++) {   // blend em
      writedata[i] = blend( secondbuffer[i], firstbuffer[i], crossct);   // Blend arrays of LEDs, third value is blend %
    }
  }
  FastLED.show();
  framerate = FastLED.getFPS();
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
  FastLED.addLeds<WS2813, LEDpin, GRB>(leds, 0, LEDStotal);
  FastLED.clear(true);
  LEDShalfx = max(1, LEDSx/2);
  LEDShalfy = max(1, LEDSy/2);
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

void wu_pixel(uint32_t x, uint32_t y, CRGB * col, CRGB *dest) {
  // extract the fractional parts and derive their inverses
  uint8_t xx = x & 0xff, yy = y & 0xff, ix = 255 - xx, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  #define WU_WEIGHT(a,b) ((uint8_t) (((a)*(b)+(a)+(b))>>8))
  uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
                   WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
  #undef WU_WEIGHT
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint8_t i = 0; i < 4; i++) {
    uint16_t xy = XY((x >> 8) + (i & 1), (y >> 8) + ((i >> 1) & 1));
    //writedata[xy].r = qadd8(writedata[xy].r, (col->r * wu[i]) >> 8);
    //writedata[xy].g = qadd8(writedata[xy].g, (col->g * wu[i]) >> 8);
    //writedata[xy].b = qadd8(writedata[xy].b, (col->b * wu[i]) >> 8);
    dest[xy].r = qadd8(writedata[xy].r, (col->r * wu[i]) >> 8);
    dest[xy].g = qadd8(writedata[xy].g, (col->g * wu[i]) >> 8);
    dest[xy].b = qadd8(writedata[xy].b, (col->b * wu[i]) >> 8);
  }
}

CRGB ColorFromPaletteExtended(const CRGBPalette16& pal, uint16_t index, uint8_t brightness, TBlendType blendType) {
  // Extract the four most significant bits of the index as a palette index.
  uint8_t index_4bit = (index >> 12);
  // Calculate the 8-bit offset from the palette index.
  uint8_t offset = (uint8_t)(index >> 4);
  // Get the palette entry from the 4-bit index
  const CRGB* entry = &(pal[0]) + index_4bit;
  uint8_t red1   = entry->red;
  uint8_t green1 = entry->green;
  uint8_t blue1  = entry->blue;

  uint8_t blend = offset && (blendType != NOBLEND);
  if (blend) {
    if (index_4bit == 15) {
      entry = &(pal[0]);
    } else {
      entry++;
    }

    // Calculate the scaling factor and scaled values for the lower palette value.
    uint8_t f1 = 255 - offset;
    red1   = scale8_LEAVING_R1_DIRTY(red1,   f1);
    green1 = scale8_LEAVING_R1_DIRTY(green1, f1);
    blue1  = scale8_LEAVING_R1_DIRTY(blue1,  f1);

    // Calculate the scaled values for the neighbouring palette value.
    uint8_t red2   = entry->red;
    uint8_t green2 = entry->green;
    uint8_t blue2  = entry->blue;
    red2   = scale8_LEAVING_R1_DIRTY(red2,   offset);
    green2 = scale8_LEAVING_R1_DIRTY(green2, offset);
    blue2  = scale8_LEAVING_R1_DIRTY(blue2,  offset);
    cleanup_R1();

    // These sums can't overflow, so no qadd8 needed.
    red1   += red2;
    green1 += green2;
    blue1  += blue2;
  }
  if (brightness != 255) {
    // nscale8x3_video(red1, green1, blue1, brightness);
    nscale8x3(red1, green1, blue1, brightness);
  }
  return CRGB(red1, green1, blue1);
}

#endif
