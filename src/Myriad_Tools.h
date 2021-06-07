#ifndef Myriad_Tools_h
#define Myriad_Tools_h

// Includes lots of core functionality

extern SimplePatternList PATTERNlist;

uint16_t XY(byte x, byte y) {
  uint16_t LEDaddress = x * LEDper + y;
  return LEDaddress;
}

void huepusher(int8_t hueinc, uint8_t zval, uint8_t timer1) {
  EVERY_N_MILLIS(timer1) {
    hue[zval] += hueinc;
  }
}

void peroidictasks(){
  EVERY_N_MILLIS(500){
    random16_add_entropy(analogRead(0));  
    Serial.print("FPS: ");
    Serial.print(LEDS.getFPS());
    Serial.print(" POWER: ");
    Serial.println(calculate_unscaled_power_mW(leds, LEDtotal)/5/100);
    //MPUprint();  
  }
}
void fader() {
  if (currfade > targfade) {              // can change this to make it blend down instead of just up
    currfade = targfade;
  } else if (currfade < targfade) {
    currfade++;
  }
  /*
  Serial.print("curr: ");
  Serial.print(currfade);
  Serial.print("         targ: ");
  Serial.println(targfade);
  */
  fadeToBlackBy( leds, LEDtotal, currfade);
  //fadeToBlackBy( leds2, LEDtotal, currfade);
}

void patcrossproc(int newpatnum){                                // Every time you switch patterns run this to begin crossfading
  Serial.print("proc - new pat num: ");
  Serial.println(newpatnum);
  targfade = faderlookup[newpatnum];
  oldpattern = patternum;                           // set the current pattern to be the old one so we can make it use the same variables
  crossct = 0;                                      // reset the blend amount
  patternum = newpatnum;                                      // increase pattern number   -> this just goes in sequence, but the best part of this is that you can control this by remote or any other system  

  rowcount[0] = rowcount[1];          // copy row status to default rows, and reset them for the new pattern
  rowcount[1] = 0;
  colcount[0] = colcount[1];
  colcount[1] = 0;
  count[0] = count[1];
  count[1] = 0;
  hue[0] = hue[1];
  hue[1] = 0;
}

void shuffler(){
  // Shuffle Pattern
  EVERY_N_SECONDS_I(shufloop, STATEshuffleinterval){
    if(patshuffle == true){
      patcrossproc(random8(1, 20));     // Choose range that skips unwanted shuffles
    }
  }
  shufloop.setPeriod(STATEshuffleinterval);
  // Palette Shuffle
  if(palnum == 0){
    currentPalette = targetPalette;
  } else {
    nblendPaletteTowardPalette( currentPalette, targetPalette, blendspeed);
    nblendPaletteTowardPalette( currentPalette, targetPalette, blendspeed);
  }
}

void blackout() {
  for ( int i = 0; i < LEDtotal; i++) {
    leds[i] = CRGB(0, 0, 0);
    //leds2[i] = CRGB(0,0,0);
  }
}

////////////////////////// Crossfader //////////////////////////////////////////
void crossfader() {
  EVERY_N_MILLIS_I(mainloop, STATEloopinterval){
    fader();
    if (crossct >= 255) {
      PATTERNlist[patternum]();   // run completed pattern only when fading is complete
    }
    else if (crossct < 255) {
      EVERY_N_MILLIS(7) {                                                                                        // half this and below
        crossct += 1;         // higher increase faster xfade
      }
      if (crossct > 255) { // overflow prevention
        crossct = 255;
      }
  
      PATTERNlist[oldpattern]();    // Run the old pattern and save to array
      for (uint16_t i = 0; i < LEDtotal; i++) {
        leds2[i] = leds[i];
      }
  
      PATTERNlist[patternum]();   // Run the new pattern and save to array // Removed extra buffering
  
      for (uint16_t i = 0; i < LEDtotal; i++) {   // blend em
        leds[i] = blend( leds2[i], leds[i], crossct);   // Blend arrays of LEDs, third value is blend %
      }
    }
         
    FastLED.show();
  }
  mainloop.setPeriod(STATEloopinterval);
  if(LEDonoff == 1){
    if (LEDcurbright != LEDtargbright ){
      if(LEDcurbright < LEDtargbright){ // This fades brightness instead of it being sudden
        LEDcurbright++;
      } else if (LEDcurbright > LEDtargbright){
        LEDcurbright--;
      }
      FastLED.setBrightness(LEDcurbright);
    }
  }
  else if(LEDcurbright > 0){
    LEDcurbright--; 
    FastLED.setBrightness(LEDcurbright);
  }
}

uint8_t fetcher(uint8_t oldcheck) {       // Get which counters should be used
  if (oldcheck == oldpattern) {
    return 0;
  } else {
    return 1;
  }
}

// blurColumns: perform a blur1d on each column of a rectangular matrix
void blurColumnsv3(CRGB* leds, uint8_t height, uint8_t width, fract8 blur_amount) {
  // blur columns
  uint8_t keep = 255 - blur_amount;
  uint8_t seep = blur_amount >> 1;

  for ( uint8_t col = 0; col < width; col++) {
    CRGB carryover = CRGB::Black;
    for ( uint8_t i = 0; i < height; i++) {
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

void blur(CRGB* leds, uint8_t height, uint8_t width, fract8 blur_amount) {                        //  leds[i]*=2; to multiply brightness
  blurColumnsv3( leds, height, width, blur_amount);
  blurRows( leds, height, width, blur_amount * 0.89);         //.86 seems to work
}

void palettetargeting(uint8_t paldefault) {     // Select which palette to blend to
  // Match, Random, Startup, Purplecascade, Blackwhite, Newspaper, Sunburst, Gilt, Tropicana, BorderRainbow, Redyell, Rainbow, RainbowStripe, Cloud, Party, Lava, Ocean, Forest, Heat
  uint8_t paltarget = palnum;

  if (palnum == 0) {
    paltarget = paldefault;
  }
  if (palnum == 1) {
    EVERY_N_SECONDS_I(palshufloop, STATEpalshuffleinterval) {
      paltarget = random8(2, 19);
    }
    palshufloop.setPeriod(STATEpalshuffleinterval);
  }
  switch (paltarget) {
    case 2:
      targetPalette = startup;
      //Serial.println("palette: startup");
      break;
    case 3:
      targetPalette = purplecascade;
      //Serial.println("palette: purplecascade");
      break;
    case 4:
      targetPalette = blackwhite;
      //Serial.println("palette: blackwhite");
      break;
    case 5:
      targetPalette = newspaper;
      //Serial.println("palette: newspaper");
      break;
    case 6:
      targetPalette = sunburst;
      //Serial.println("palette: sunburst");
      break;
    case 7:
      targetPalette = gilt;
      //Serial.println("palette: gilt");
      break;
    case 8:
      targetPalette = tropicana;
      //Serial.println("palette: tropicana");
      break;
    case 9:
      targetPalette = borderrainbow;
      //Serial.println("palette: borderrainbow");
      break;
    case 10:
      targetPalette = redyell;
      //Serial.println("palette: redyell");
      break;
    case 11:
      targetPalette = RainbowColors_p;
      //Serial.println("palette: RainbowColors");
      break;
    case 12:
      targetPalette = RainbowStripeColors_p;
      //Serial.println("palette: RainbowStripeColors");
      break;
    case 13:
      targetPalette = CloudColors_p;
      //Serial.println("palette: CloudColors_p");
      break;
    case 14:
      targetPalette = PartyColors_p;
      //Serial.println("palette: PartyColors");
      break;
    case 15:
      targetPalette = LavaColors_p;
      //Serial.println("palette: LavaColors");
      break;
    case 16:
      targetPalette = OceanColors_p;
      //Serial.println("palette: OceanColors");
      break;
    case 17:
      targetPalette = ForestColors_p;
      //Serial.println("palette: ForestColors");
      break;
    case 18:
      targetPalette = HeatColors_p;
      //Serial.println("palette: HeatColors");
      break;
    case 19:
      targetPalette = Warm_Forest;
      //Serial.println("palette: Warm_Forest");
      break;
    case 20:
      targetPalette = Popsicle;
      //Serial.println("palette: Popsicle");
      break;
    case 21:
      targetPalette = Arizona_Trip;
      //Serial.println("palette: Arizona_Trip");
      break;

  }
  if (palnum == 0) {
    currentPalette = targetPalette;
  }
}

/*
  const TProgmemRGBGradientPalettePtr Palette_List[] = {
  startup, purplecascade, blackwhite, newspaper, sunburst, gilt,
  tropicana, borderrainbow, redyell, RainbowColors_p, RainbowStripeColors_p,
  CloudColors_p,PartyColors_p,LavaColors_p, OceanColors_p,
  ForestColors_p, HeatColors_p, Warm_Forest, Popsicle, Arizona_Trip};

  // Count of how many cpt-city gradients are defined:
  const uint8_t gGradientPaletteCount = sizeof(Palette_List) / sizeof(TProgmemRGBGradientPalettePtr);
*/

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
