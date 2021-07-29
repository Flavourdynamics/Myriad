#ifndef Myriad_2D_Patterns_h
#define Myriad_2D_Patterns_h
// See steps to add patterns in Testbed.h
/*
void NewPattern(bool newPL){
  //patrunproc(bool newPL, byte fadeamt, int8_t hueinc, uint8_t huespeed, TProgmemRGBGradientPalettePtr matchpal)
  patrunproc(newPL, 255, 1, 16, Blackwhite);
}
*/
void Rando(bool newPL){
  blackout();
  patrunproc(newPL, 255, 1, 16, RainbowColors);
  int lowernummorelights = 150;
  if(palnum == palmatch){
    for (int i = 0; i < LEDtotal/lowernummorelights ; i++){  // runs 3 times - better boot pattern? (fade?)
      uint16_t targetx = random16(0, LEDstrips);
      uint16_t targety = random16(0, LEDper);
      leds[XY(targetx, targety)] = CHSV(random8(), random8(), random8(150, 255));
    }
  } else {
    for (int i = 0; i < LEDtotal/lowernummorelights ; i++){  // runs 3 times - better boot pattern? (fade?)
      uint16_t targetx = random16(0, LEDstrips);
      uint16_t targety = random16(0, LEDper);
      leds[XY(targetx, targety)] = ColorFromPalette(currentPalette, random8(), 255, LINEARBLEND);
      //leds[targetx][targety]set.Sat ) random8(155,255)
    }
  }
}
 
void Illusion(bool newPL){
  patrunproc(newPL, 255, 1, 16, Blackwhite);
  for (int y = 0; y < LEDhalfper; y++){
    for (int x = 0; x < LEDhalfstrips; x++){
      leds[XY(x, y)] = ColorFromPalette(currentPalette, hue[newPL] + x*25 + y*25, 255, LINEARBLEND);                      //top left
      leds[XY(LEDstrips-1-x, y)] = ColorFromPalette(currentPalette, hue[newPL] + x*25 + y*25, 255, LINEARBLEND);          //top right
      leds[XY(x, LEDper-1-y)] = ColorFromPalette(currentPalette, hue[newPL] + x*25 + y*25, 255, LINEARBLEND);             //bottom left
      leds[XY(LEDstrips-1-x, LEDper-1-y)] = ColorFromPalette(currentPalette, hue[newPL] + x*25 + y*25, 255, LINEARBLEND); //bottom right 
    }
  }
}

void Diagonal(bool newPL){
  patrunproc(newPL, deffade, 1, 16, Startup);
  //Serial.println(newPL);
  for (int y = 0; y < rowcount[newPL]; y++){
    for (int x = 0; x < colcount[newPL]; x++){
       leds[XY(x, y)] = ColorFromPalette( currentPalette, hue[newPL]+(x+y)*10, 255, LINEARBLEND); // normal palette access
    }
  }
  EVERY_N_MILLIS(100){
    if (colcount[newPL] < LEDstrips){
      colcount[newPL]++;
    }
    if (rowcount[newPL] < LEDper) {
      rowcount[newPL]++;
    }
  }
}

void Scan(bool newPL){
  patrunproc(newPL, deffade, 1, 16, RainbowColors);
  for (int y = 0; y < LEDper; y++){
    for (int x = 0; x < LEDstrips; x++){
      leds[XY(x, y)] = ColorFromPalette(currentPalette, hue[newPL], random8(100,150), LINEARBLEND); 
    } 
  }
}

void Confetti(bool newPL){
  patrunproc(newPL, deffade, 1, 16, RainbowColors);
  //blackout();
  for(int x = 0; x < LEDstrips; x++){
    int y = random16(LEDper);
    leds[XY(x, y)] += ColorFromPalette(currentPalette, hue[newPL] + random8(64), 200, LINEARBLEND);
  }
}

void Sinelon(bool newPL){         // A romantic wave of 
  patrunproc(newPL, 20, 1, 16, RainbowColors);
  for(int bead = 1; bead < 10; bead++){
    int xpos = beatsin16((bead * 3 / beatmod) + 1, 0, LEDstrips - 1);
    int ypos = beatsin16((bead * 2 / beatmod) + 1, 0, LEDper - 1);
    leds[XY(xpos, ypos)] += ColorFromPalette(currentPalette, hue[newPL] * bead, 255, LINEARBLEND);
  }
  blur( leds, LEDstrips, LEDper, 128);
  FastLED.show();
}

void Vortex(bool newPL){
  patrunproc(newPL, 20, 1, 16, RainbowColors);
  for(int y = 0; y < LEDper; y++){
    uint8_t pos = sin8(hue[newPL]*(y+1));   
    pos = map(pos, 1, 248, 0, LEDstrips-1);
    leds[XY(pos, y)] += ColorFromPalette(currentPalette, hue[newPL]+y/2, 255, LINEARBLEND);
  }
  
}

void Tropic(bool newPL){
  patrunproc(newPL, 128, 1, 16, Tropicana);
  uint16_t beat = beatsin16( 14, 25, 455);
  for(int x = 0; x < LEDstrips; x++){
    for( int y = 0; y < LEDper; y++){
      leds[XY(x, y)] = ColorFromPalette(currentPalette, hue[newPL]+(y*10)+x*2, beat-hue[newPL]+(y*10)-x*2, LINEARBLEND);   //leds[x][y] = ColorFromPalette(currentPalette, hue+(y*10)+x*2, beat-hue+(y*10)-x*2, LINEARBLEND);
    }
  }
  
}

void Juggle(bool newPL){
  patrunproc(newPL, deffade, 1, 16, RainbowColors);
  for(int x = 0; x < LEDstrips; x++){
    byte dothue = 0;
    for( int i = 0; i < 5; i++){
      leds[XY(x, beatsin16( (i+7)/beatmod, 0, LEDper-1 ))] |= ColorFromPalette(currentPalette, hue[newPL] + dothue+x*3, 255, LINEARBLEND);
      dothue += 51;
    }
  }
}

void Flex(bool newPL){
  patrunproc(newPL, 20, 1, 16, RainbowColors);
  for (int col = 0; col < LEDstrips; col++){         // Write each row with start colour and a random saturation
    int beatbox = beatsin8(20/beatmod, 3, abs(14-col) + 15);
    for(int row = 0; row < rowcount[newPL]; row++){
      leds[XY(col, row)] = ColorFromPalette(currentPalette, hue[newPL] + beatbox*row, 255, LINEARBLEND); // fill_rainbow(leds[x], number of leds, start hue, hue delta)  
      leds[XY(LEDstrips-1-col, row)] = ColorFromPalette(currentPalette, hue[newPL] + beatbox*row, 255, LINEARBLEND);  
      //ledmatrix.VerticalMirror();
      //leds[(LEDstrips*LEDper)-(col*LEDper+row)] = ColorFromPalette(currentPalette, hue + beatbox*row, 255, LINEARBLEND); // fill_rainbow(leds[x], number of leds, start hue, hue delta)         fill_rainbow( leds[col], rowcount/2, hue, beatbox); // fill_rainbow(leds[x], number of leds, start hue, hue delta)
    }
  }          
  EVERY_N_MILLIS(100){
    if(rowcount[newPL] < LEDper){
      rowcount[newPL]++;
    }
  }
}

void Firetoucher(bool newPL){
  patrunproc(newPL, deffade, 1, 24, Redyell);
  for (int col = 0; col < LEDhalfstrips; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row < LEDhalfper; row++){
      int standin = beatsin8(60, 5, 10);      // BPM, MIN, MAX
      leds[XY(col, row)] = ColorFromPalette(currentPalette, hue[newPL] + standin * -col + row * 5, 255, LINEARBLEND);      // Target palette, start hue
      leds[XY(col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] + standin * -col + row * 5, 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, row)] = ColorFromPalette(currentPalette, hue[newPL] + standin * -col + row * 5, 255, LINEARBLEND);
      leds[XY((LEDstrips - 1 - col), LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] + standin * -col + row * 5, 255, LINEARBLEND);
    }
  } 
}

void dacirc(bool newPL){
  patrunproc(newPL, deffade, 1, 24, RainbowColors);
  for(int bead = 1; bead < 30; bead++){                                
    uint16_t xpos;
    uint16_t ypos;
    
    noise[bead][0] = inoise8(noise[bead][0], timer+bead);           // get some noise
    noise[bead][0] = constrain(noise[bead][0], 60, 195);
    xpos = map(noise[bead][0], 60, 195, 0, LEDstrips);        // Map it to width

    noise[bead][1] = inoise8(noise[bead][1], timer*2+random8()+bead); 
    noise[bead][1] = constrain(noise[bead][1], 60, 195); 
    ypos = map(noise[bead][1], 60, 195, 0, LEDper);
    
    //ledmatrix.DrawFilledCircle(xpos, ypos, 2, ColorFromPalette(currentPalette, hue[newPL] + bead*1, 255*bead, LINEARBLEND));
  }
  
  timer+=1;
}

void Diamondmaw(bool newPL){
  patrunproc(newPL, 10, -1, 16, Purple_Cascade);
  for (int col = 0; col < LEDhalfstrips; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row < LEDhalfper; row++){
      
      int standin = beatsin8(14, -5, 25);      // BPM, MIN, MAX
      leds[XY(col, row)] = ColorFromPalette(currentPalette, hue[newPL] - (col+row)*standin, 255, LINEARBLEND);      // Target palette, start hue
      leds[XY(col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] - (col+row)*standin, 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, row)] = ColorFromPalette(currentPalette, hue[newPL] - (col+row)*standin, 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] - (col+row)*standin, 255, LINEARBLEND);
    }
  }
}

void Him(bool newPL){
  patrunproc(newPL, deffade, -1, 10, Gilt);
  for (int col = 0; col < LEDhalfstrips; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row < LEDhalfper; row++){   
      int standin = beatsin16(14, 50, 300);      // BPM, MIN, MAX
      leds[XY(col, row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(400-standin)/250 + (col+row*standin*col)/350, 255, LINEARBLEND);      // Target palette, start hue
      leds[XY(col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(400-standin)/250 + (col+row*standin*col)/350, 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(400-standin)/250 + (col+row*standin*col)/350, 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(400-standin)/250 + (col+row*standin*col)/350, 255, LINEARBLEND);
    }
  }
}

void lilminfuk(bool newPL){
  patrunproc(newPL, deffade, -1, 16, RainbowStripeColors);
  int standin = beatsin16(8, 75, 175);      // BPM, MIN, MAX
  for (int col = 0; col < LEDhalfstrips; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row < LEDhalfper; row++){      
      leds[XY(col, row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(250-standin)/5 + (col+row*standin*col)/250, 255, LINEARBLEND);      // Target palette, start hue
      leds[XY(col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(250-standin)/5 + (col+row*standin*col)/250, 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(250-standin)/5 + (col+row*standin*col)/250, 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(250-standin)/5 + (col+row*standin*col)/250, 255, LINEARBLEND);
    }
  }
}

void Barbarism(bool newPL){
  patrunproc(newPL, 20, 1, 16, Purple_Cascade);
  static uint16_t xmarksthespot;
  uint16_t barlength = 5;
  uint16_t upperlim = NUMleds - barlength -1;
  EVERY_N_MILLIS(100){
    xmarksthespot = random16(0, upperlim);
  }
  for (uint16_t i = xmarksthespot; i < barlength + xmarksthespot; i++) {
    leds[i] = ColorFromPalette(currentPalette, i*15+hue[newPL], 255, LINEARBLEND);
  }
FastLED.show();
}
/*
void Rainbow() {
  palettetargeting(1);
  uint8_t z = fetcher(18);
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void RainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
*/
//Sinelon,Illusion,Diagonal,Rando,Scan,Confetti,Vortex,Tropic,Juggle,Flex,Firetoucher,Diamondmaw,Him,lilminfuk,Barbarism
/////////////////////////////////////////////////////////////////////////////////////////////////
NamedPattern Pattern_List[] = {
  {(pattern_func)Sinelon,       F("Sinelon")},
  {(pattern_func)Illusion,      F("Illusion")},
  {(pattern_func)Diagonal,      F("Diagonal")},
  {(pattern_func)Rando,         F("Rando")},
  {(pattern_func)Scan,          F("Scan")},
  {(pattern_func)Confetti,      F("Confetti")},
  {(pattern_func)Vortex,        F("Vortex")},
  {(pattern_func)Tropic,        F("Tropic")},
  {(pattern_func)Juggle,        F("Juggle")},
  {(pattern_func)Flex,          F("Flex")},
  {(pattern_func)Firetoucher,   F("Firetoucher")},
  {(pattern_func)dacirc,        F("dacirc")},
  {(pattern_func)Diamondmaw,    F("Diamondmaw")},
  {(pattern_func)Him,           F("Him")},
  {(pattern_func)lilminfuk,     F("lilminfuk")},
  {(pattern_func)Barbarism,     F("Barbarism")},
};

const uint16_t NUMpatterns = (sizeof(Pattern_List) / sizeof(Pattern_List[0]));

#endif