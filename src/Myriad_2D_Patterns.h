#ifndef Myriad_2D_Patterns_h
#define Myriad_2D_Patterns_h
#include <Myriad_Types.h>
#include <Myriad_Tools.h>
#include "Myriad_Palettes.h"
#include "Myriad_Arrays.h"
/* New Pattern Addition Procedure
1. Use "NewPattern() template to create new pattern
2. Include defaults for patrunproc based on characteristics of pattern
3. Add pattern to the list with both function name and stylized name

void NewPattern(bool newPL, CRGB *dest){
  //patrunproc(bool newPL, byte fadeamt, int8_t hueinc, uint8_t huespeed, TProgmemRGBGradientPalettePtr matchpal)
  patrunproc(newPL, 255, 1, 16, Blackwhite);
}
*/
void Rando(bool newPL, CRGB *dest){
  //blackout(dest);
  patrunproc(newPL, deffade, 1, 16, RainbowColors);
  int lowernummorelights = 255;
  uint16_t targetx = random16(0, LEDSx);
  uint16_t targety = random16(0, LEDSy);
  CRGB colour;
  if(palnum == palmatch){
    for (int i = 0; i < LEDStotal/lowernummorelights ; i++){
      colour = CHSV(random8(), random8(), random8(150, 255));
    }
  } else {
    for (int i = 0; i < LEDStotal/lowernummorelights ; i++){
      colour = ColorFromPalette(currentPalette, random8(), 255, LINEARBLEND);
    }
  }
  dest[XY(targetx, targety)] = colour;
}
 
void Illusion(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 1, 8, Blackwhite);
  //CRGB wucolour;
  for (uint32_t y = 0; y < LEDShalfy; y++){
    for (uint32_t x = 0; x < LEDShalfx; x++){
      dest[XY(x, y)] = ColorFromPalette(currentPalette, hue[newPL] + x*25 + y*25, 255, LINEARBLEND);                      //top left
      dest[XY(LEDSx-1-x, y)] = ColorFromPalette(currentPalette, hue[newPL] + x*25 + y*25, 255, LINEARBLEND);          //top right
      dest[XY(x, LEDSy-1-y)] = ColorFromPalette(currentPalette, hue[newPL] + x*25 + y*25, 255, LINEARBLEND);             //bottom left
      dest[XY(LEDSx-1-x, LEDSy-1-y)] = ColorFromPalette(currentPalette, hue[newPL] + x*25 + y*25, 255, LINEARBLEND); //bottom right
      
      //wucolour = ColorFromPalette(currentPalette, hue[newPL] + x*25 + y*25, 255, LINEARBLEND);
      //wucolour = CRGB(55, 0, 55);
      //wu_pixel(x,y, &wucolour, dest);  //top left
      //wu_pixel(LEDSx-1-x,y, &wucolour, dest);  //top right
      //wu_pixel(x,LEDSy-1-y, &wucolour, dest);  //bottom left
      //wu_pixel(LEDSx-1-x, LEDSy-1-y, &wucolour, dest);  //bottom right 
    }
  }
}

void Diagong(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 1, 16, Startup);
  //Serial.println(newPL);
  for (int y = 0; y < rowcount[newPL]; y++){
    for (int x = 0; x < colcount[newPL]; x++){
       dest[XY(x, y)] = ColorFromPalette( currentPalette, hue[newPL]+(x+y)*10, 255, LINEARBLEND); // normal palette access
    }
  }
  EVERY_N_MILLIS(100){
    if (colcount[newPL] < LEDSx){
      colcount[newPL]++;
    }
    if (rowcount[newPL] < LEDSy) {
      rowcount[newPL]++;
    }
  }
}

void Confetti(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 1, 16, RainbowColors);
  //blackout(dest);
  for(int x = 0; x < LEDSx; x++){
    int y = random16(LEDSy);
    dest[XY(x, y)] += ColorFromPalette(currentPalette, hue[newPL] + random8(64), 200, LINEARBLEND);
  }
}

void Sinelon(bool newPL, CRGB *dest){         // A romantic wave of 
  patrunproc(newPL, 20, 1, 16, RainbowColors);
  for(int bead = 1; bead < 10; bead++){
    int xpos = beatsin16((bead * 3 / beatmod) + 1, 0, LEDSx - 1);
    int ypos = beatsin16((bead * 2 / beatmod) + 1, 0, LEDSy - 1);
    dest[XY(xpos, ypos)] += ColorFromPalette(currentPalette, hue[newPL] * bead, 255, LINEARBLEND);
  }
  blur( dest, LEDSx, LEDSy, 128);
  FastLED.show();
}

void Vortex(bool newPL, CRGB *dest ){
  //EVERY_N_MILLIS(100){
    patrunproc(newPL, 10, 1, 16, RainbowColors);
    for(uint32_t y = 0; y < LEDSy; y++){
      uint16_t xpos = beatsin16(hue[newPL]/2*(y+1));
      //if(y == 5){
        //Serial.println(xpos);
      //  Serial.printf("%02d",xpos);
      //  Serial.println();
        //Serial.print(", ");
       // Serial.println(y);
      //}
      //xpos = map(xpos, 1, 248, 0, (LEDSx-1)*255);
      xpos = map(xpos, 1, 65536, 0, (LEDSx-1)*255);
      //uint32_t xpos = beatsin16(hue[newPL]*(y+1),0,(LEDSy-1)*255);
      //uint32_t xpos = beatsin16(5,0,(LEDSy-1)*255);
      //uint32_t xpos = beatsin16(hue[newPL]/10+(y+1), 0, LEDSx-1); // same as beatsin just beats*255
      
      //pos = map(pos, 1, sizeof(uint16_t), 0, (LEDSx-1)*255);
      
      CRGB wucolour = ColorFromPalette(currentPalette, hue[newPL]+(y+1), 255, LINEARBLEND);
      //if(y == LEDSy*254){wu_pixel(xpos,y, &wucolour, dest);}
      wu_pixel(xpos,y*255, &wucolour, dest);
      //dest[XY(xpos, y)] += wucolour;
    }
  //}
}

void Tropic(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 1, 16, Tropicana);
  uint16_t beat = beatsin16( 14, 25, 455);
  for(int x = 0; x < LEDSx; x++){
    for( int y = 0; y < LEDSy; y++){
      dest[XY(x, y)] = ColorFromPalette(currentPalette, hue[newPL]+(y*10)+x*2, beat-hue[newPL]+(y*10)-x*2, LINEARBLEND);   //dest[x][y] = ColorFromPalette(currentPalette, hue+(y*10)+x*2, beat-hue+(y*10)-x*2, LINEARBLEND);
    }
  }
}

void Juggle(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 1, 16, RainbowColors);
  for(int x = 0; x < LEDSx; x++){
    byte dothue = 0;
    for( int i = 0; i < 5; i++){
      dest[XY(x, beatsin16( (i+7)/beatmod, 0, LEDSy-1 ))] |= ColorFromPalette(currentPalette, hue[newPL] + dothue+x*3, 255, LINEARBLEND);
      dothue += 51;
    }
  }
}

void Flex(bool newPL, CRGB *dest){
  patrunproc(newPL, 20, 1, 16, RainbowColors);
  for (int col = 0; col < LEDSx; col++){         // Write each row with start colour and a random saturation
    int beatbox = beatsin8(20/beatmod, 3, abs(14-col) + 15);
    for(int row = 0; row < rowcount[newPL]; row++){
      CRGB colour = ColorFromPalette(currentPalette, hue[newPL] + beatbox*row, 255, LINEARBLEND);
      dest[XY(col, row)] = colour;
      dest[XY(LEDSx-1-col, row)] = colour;
      //leds[(LEDSx*LEDSy)-(col*LEDSy+row)] = ColorFromPalette(currentPalette, hue + beatbox*row, 255, LINEARBLEND); // fill_rainbow(leds[x], number of leds, start hue, hue delta)         fill_rainbow( leds[col], rowcount/2, hue, beatbox); // fill_rainbow(leds[x], number of leds, start hue, hue delta)
    }
  }          
  EVERY_N_MILLIS(100){
    if(rowcount[newPL] < LEDSy){
      rowcount[newPL]++;
    }
  }
}

void Firetoucher(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 1, 24, Redyell);
  for (int col = 0; col < LEDShalfx; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row < LEDShalfy; row++){
      int standin = beatsin8(60, 5, 10);      // BPM, MIN, MAX
      CRGB colour =  ColorFromPalette(currentPalette, hue[newPL] + standin * -col + row * 5, 255, LINEARBLEND);
      dest[XY(col, row)] = colour;
      dest[XY(col, LEDSy - 1 - row)] = colour;
      dest[XY(LEDSx - 1 - col, row)] = colour;
      dest[XY((LEDSx - 1 - col), LEDSy - 1 - row)] = colour;
    }
  } 
}


void Gentle_Leviathan(bool newPL, CRGB *dest){
  patrunproc(newPL, 55, 1, 24, RainbowColors);
  const uint16_t numbeads = 250;
  static uint16_t noise[numbeads][2]; // create two arrays with x and y coordinates
  uint16_t lowbound = 14000; // The average boundaries of inoise16
  uint16_t hibound = 52000;  // The average boundaries of inoise16

  for(int bead = 1; bead < numbeads; bead++){
    noise[bead][0] = inoise16(noise[bead][0], timer[newPL]+bead*1000);     // get some noise
    noise[bead][0] = constrain(noise[bead][0], lowbound, hibound);               // keep it within a reasonable scalable boundary (aesthetic judgement)
    uint16_t xpos = map(noise[bead][0], lowbound, hibound, 0, LEDSx);        // Map it to width

    noise[bead][1] = inoise16(noise[bead][1], timer[newPL]*2+bead*1000); //random8() this was in the last arg
    noise[bead][1] = constrain(noise[bead][1], lowbound, hibound); 
    uint16_t ypos = map(noise[bead][1], lowbound, hibound, 0, LEDSy);

    ledmatrix.DrawFilledCircle(xpos, ypos, 2, blend(ColorFromPalette(currentPalette, hue[newPL] + bead*1, 255, LINEARBLEND), CRGB::White, 55));
  }
  for (uint16_t i = 0; i < LEDStotal; i++) {   // blend em
    dest[i] = blend( dest[i], writedata[i], 45);   // Blend arrays of LEDs, third value is blend %
  }
  //memcpy8(dest, LEDmatrixCRGB, LEDStotal*3);
  blur(dest, LEDSx, LEDSy, 45);
  EVERY_N_MILLIS(1){
    timer[newPL]+=55;
  }
}

void Diamondmaw(bool newPL, CRGB *dest){
  patrunproc(newPL, 10, -1, 16, Purple_Cascade);
  for (int col = 0; col < LEDShalfx; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row < LEDShalfy; row++){
      int standin = beatsin8(14, -5, 25);      // BPM, MIN, MAX
      CRGB colour =  ColorFromPalette(currentPalette, hue[newPL] - (col+row)*standin, 255, LINEARBLEND);
      dest[XY(col, row)] = colour;
      dest[XY(col, LEDSy - 1 - row)] = colour;
      dest[XY(LEDSx - 1 - col, row)] = colour;
      dest[XY(LEDSx - 1 - col, LEDSy - 1 - row)] = colour;
    }
  }
}

void Him(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, -1, 10, Gilt);
  for (int col = 0; col < LEDShalfx; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row < LEDShalfy; row++){   
      int standin = beatsin16(14, 50, 300);      // BPM, MIN, MAX
      CRGB colour =  ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(400-standin)/250 + (col+row*standin*col)/350, 255, LINEARBLEND);
      dest[XY(col, row)] = colour;
      dest[XY(col, LEDSy - 1 - row)] = colour;
      dest[XY(LEDSx - 1 - col, row)] = colour;
      dest[XY(LEDSx - 1 - col, LEDSy - 1 - row)] = colour;
    }
  }
}

void Praxis(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 0, 16, Pit); // -1
  uint16_t standin = beatsin16(14, -125, -1);      // 14, -125, -1
  uint16_t mutator = beatsin16(2, 550, 900);
  //uint16_t standin = map(beat16(0, -125, -1);      // 14, -125, -1
  //uint16_t mutator = map(beat16(2, 525, 900);
  for (uint16_t col = 0; col < LEDShalfx; col++){         // Write each row with start colour and a random saturation    
    for(uint16_t row = 0; row < LEDShalfy; row++){
      //CRGB colour = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(250-standin)/5 + (col+row*standin*col)/1000, 255, LINEARBLEND);
      CRGB colour = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(250-standin)/5 + (col+row*standin*col)/mutator, 255, LINEARBLEND);
      dest[XY(col, row)] =                          blend(dest[XY(col, row)], colour, 4);
      dest[XY(col, LEDSy - 1 - row)] =              blend(dest[XY(col, LEDSy - 1 - row)], colour, 4);
      dest[XY(LEDSx - 1 - col, row)] =              blend(dest[XY(LEDSx - 1 - col, row)], colour, 4);
      dest[XY(LEDSx - 1 - col, LEDSy - 1 - row)] =  blend(dest[XY(LEDSx - 1 - col, LEDSy - 1 - row)], colour, 4);
    }
  }
}

void Barbarism(bool newPL, CRGB *dest){
  patrunproc(newPL, 1, 1, 16, Purple_Cascade);
  static uint16_t xmarksthespot;
  static uint16_t ymarksthespot;
  static uint16_t xrand;
  static uint16_t yrand;
  uint16_t barlength = 5;
  uint16_t ylim = LEDSy - barlength;  // to prevent overflow vert
  uint16_t xlim = LEDSx - barlength;

  EVERY_N_MILLIS(10){
    xmarksthespot = random16(0, xlim);
    ymarksthespot = random16(0, ylim);
    xrand = random16(LEDSx);
    yrand = random16(LEDSy);
  }
  for (uint16_t i = xmarksthespot; i < barlength + xmarksthespot; i++) {
    dest[XY(i, yrand)] = ColorFromPalette(currentPalette, i*15+hue[newPL], 255, LINEARBLEND);
  }
  for (uint16_t i = ymarksthespot; i < barlength + ymarksthespot; i++) {
    dest[XY(xrand, i)] = ColorFromPalette(currentPalette, i*15+hue[newPL], 255, LINEARBLEND);
  }
}

void DNA(bool newPL, CRGB *dest){
  patrunproc(newPL, 20, 1, 16, Purple_Cascade);
  byte threads = 4;
  byte phaseoff = 255/threads;
  for(int y = 0; y < LEDSy; y++){ // For each row
    for(byte p = 0; p < threads; p++){                        // p is phase mod, 128 coefficient is 64 degrees out of phase
      // Format: dest[coordinates oscillate between left and right with next lines out of phase] =  CHSV(base hue+difference between rows, saturation changes a little bit for variety, value varies by row)
      // Increase coefficients of 'y' terms to increase the differences between rows
      //dest[XY(beatsin8(10, 0, LEDSx-1, 0, y*4+p*phaseoff), y)] = CHSV(-hue+y*5+p*phaseoff, beatsin8(25, 200, 255, 0, y*3), beatsin8(5, 55, 255, 0, y*10+p*phaseoff)); 
      dest[XY(beatsin8(10, 0, LEDSx-1, 0, y*4+p*phaseoff), y)] = CHSV(-hue[newPL]+y*5+p*phaseoff, 255, beatsin8(5, 55, 255, 0, y*10+p*phaseoff));                 
    }
  }
  blur( dest, LEDSx, LEDSy, 255);
}

void Gargyle(bool newPL, CRGB *dest){
  patrunproc(newPL, 0, 1, 16, Purple_Cascade);
  // NO FADE TO MAKE IT CHECKERBOARD
  byte threads = 4;
  byte phaseoff = 255/threads;
  for(int y = 0; y < LEDSy; y++){ // For each row
    for(byte p = 0; p < threads; p++){                        // p is phase mod, 128 coefficient is 64 degrees out of phase
      // Format: dest[coordinates oscillate between left and right with next lines out of phase] =  CHSV(base hue+difference between rows, saturation changes a little bit for variety, value varies by row)
      // Increase coefficients of 'y' terms to increase the differences between rows
      //dest[XY(beatsin8(10, 0, LEDSx-1, 0, y*4+p*phaseoff), y)] = CHSV(-hue+y*5+p*phaseoff, beatsin8(25, 200, 255, 0, y*3), beatsin8(5, 55, 255, 0, y*10+p*phaseoff));    
      dest[XY(beatsin8(10, 0, LEDSx-1, 0, y*4+p*phaseoff), y)] = CHSV(-hue[newPL]+y*5+p*phaseoff, 255, beatsin8(5, 55, 255, 0, y*10+p*phaseoff));              
    }
  }
  //blur2d( leds, LEDSx, LEDSy, 128);
}

static uint32_t timering[2];
static uint16_t hues;
void Midearth(bool newPL, CRGB *dest){
  patrunproc(newPL, 255, -1, 8, CloudColors);
  static const uint8_t items = 60; // How many rings
  static uint16_t menoise[items][2];
  static bool beatexpand;
  static byte beatmult;

  if (EQbeat == true){
  beatexpand = true;
  }
  if (beatexpand == true && beatmult < 3){
    beatmult++;
  }
  if (beatexpand == true && beatmult >= 3){
    beatexpand = false;
  }
  if (beatexpand == false && beatmult > 0){
    beatmult--;
  }
  for(uint16_t i = items; i > 0; i--){ // This loop is backwards for consistency below
    menoise[i][0] = inoise16(menoise[i][0], timering[0] - i * 2560);
    menoise[i][1] = inoise16(menoise[i][1], timering[1] - i * 3560);
    uint16_t xcord = map(menoise[i][0], 0, 65535, 0, LEDSx-1);
    uint16_t ycord = map(menoise[i][1], 0, 65535 , 0, LEDSy-1);
    CRGB colour = ColorFromPaletteExtended(Gilt, hues + i*256*14, 255, LINEARBLEND);
    //ledmatrix.DrawFilledCircle(xcord, ycord, i + beatmult, colour);
    ledmatrix.DrawCircle(xcord, ycord, i + beatmult, colour);
  }
  memcpy8(dest, writedata, LEDStotal*3);
  EVERY_N_MILLIS(10){
    timering[0]+=400;
    timering[1]+=651;
    hues-=50;
  }
}

void Waterfall(bool newPL, CRGB *dest){
  patrunproc(newPL, 255, -1, 8, CloudColors);
  int16_t twistdir = beatsin16(3, -30000, 30000); // this will underflow
  //Serial.println(twistdir);
  for (int row = 0; row < rowcount[newPL]; row++){
    for (int col = 0; col < LEDSx; col++){
      // This monster determines how much the wave twists
      //int16_t twist = sin16((row + col+count[newPL]))*(1+row)/80; // test
      int16_t twist = sin8((row*7 + col*10+count[newPL]))*(1+row)/80; // functional
      // int16_t twisto = sin16(col+count[newPL])/80;  // test
      twist = map(twist, 0, 255, -127,127);  // functional
      //if (row == 35 && col == 0){Serial.println(twist);}
      dest[XY(col, row)] = ColorFromPalette(currentPalette, hue[newPL] + row*15 + twist, 255, LINEARBLEND);
      //dest[XY(col, row)] = ColorFromPaletteExtended(currentPalette, hue[newPL] + row*15 + twist*twistdir/10000, 255, LINEARBLEND);
    }
  }
  EVERY_N_MILLIS(50){
    if(rowcount[newPL] < LEDSy){
      rowcount[newPL]++;
    }  
   //count[newPL]++; 
  }
  EVERY_N_MILLIS(10){
    count[newPL]++; 
  }
}

void Quadplexor(bool newPL, CRGB *dest){
  patrunproc(newPL, 255, 1, 16, Border_Rainbow);
  for(int band = 0; band < 14; band++){ // EQbins
    uint16_t barheight = map(EQscaled[band], 0, LEDSy, 0, LEDShalfy);
    for(int leng = 0; leng < barheight; leng++){ // Display as 00 11 22 33 44 55 66 66 55 44 33 22 11 00
      CRGB barhue = ColorFromPalette(currentPalette, hue[newPL] + leng*5 + band*5, 255, LINEARBLEND);
      dest[XY(LEDShalfx -1 -band, LEDShalfy -1 -leng)] = barhue;  // Top left
      dest[XY(LEDShalfx    +band, LEDShalfy -1 -leng)] = barhue;  // Top right
      dest[XY(LEDShalfx -1 -band, LEDShalfy    +leng)] = barhue;  // Bottom left
      dest[XY(LEDShalfx    +band, LEDShalfy    +leng)] = barhue;  // Bottom right
    }
    uint16_t peak = map(EQflatdecline[band], 0, LEDSy, 0, LEDShalfy - 1); // Get peak position
    if(peak > 0){ // No peaks on zero
      CRGB peaknative = ColorFromPalette(currentPalette, hue[newPL] + band*5 + peak*5, 255, LINEARBLEND);
      uint8_t desaturation = map(EQflatdecline[band], 0, LEDSy, 80, 255); // Get relative height from raw flat decline data
      CRGB peakhue = blend(peaknative, CRGB::White, desaturation);  // Blend between native and white in proportion to height
      dest[XY(LEDShalfx -1 -band,  LEDShalfy -1 -peak)] = peakhue;  // Top left
      dest[XY(LEDShalfx    +band,  LEDShalfy -1 -peak)] = peakhue;  // Top right
      dest[XY(LEDShalfx -1 -band,  LEDShalfy    +peak)] = peakhue;  // Bottom left
      dest[XY(LEDShalfx    +band,  LEDShalfy    +peak)] = peakhue;  // Bottom right
    }
  }
}

void Spectral_Waterfall(bool newPL, CRGB *dest){
  patrunproc(newPL, 0, 1, 16, Heatmap);
  EVERY_N_MILLIS(20){
    memmove8( &dest[XY(0,1)], &dest[XY(0,0)], LEDStotal * 3);
    for(int band = 0; band < EQbins; band++){
      byte tempvalue = map(EQ10000scaled[band], 0, 10000, 0, 255);
      dest[XY(LEDSx/2+band, 0)] = ColorFromPalette(currentPalette, tempvalue, 127+(tempvalue*0.5), LINEARBLEND);
      dest[XY(LEDSx/2-1-band, 0)] = ColorFromPalette(currentPalette, tempvalue, 127+(tempvalue*0.5), LINEARBLEND);
    }
  }
}

void Digital_Rain(bool newPL, CRGB *dest){           // Have to randomize target locations on first boot
  patrunproc(newPL, 20, 1, 16, Pastel);
  const uint8_t numofcodes = 60;    // struct targets {uint8_t x, uint8_t y, uint8_t direction, CHSV colour};
  static struct target raindrops[numofcodes];
  uint8_t numspeeds = 3;      // how many different speeds of objects
  EVERY_N_MILLIS(10){
    for(uint16_t b = 0; b < numofcodes; b++){
      uint16_t trianglemap = abs((b+(EQbins-1))%(LEDSx-1)-(EQbins-1));
      uint8_t hitrate = 2 + EQ10000scaled[abs((b+(EQbins-1))%(LEDSx-1)-(EQbins-1))]/LEDSy/6; //triangle mapping
      CRGB fullsat = ColorFromPalette(currentPalette, hue[newPL] + trianglemap*5, 255, LINEARBLEND); // fully saturated value for tails
      CRGB desat = blend(fullsat, CRGB::White, 93); //desaturated value for leads
      if(raindrops[b].y > LEDSy){             // if greater than range, do reset
        if(hitrate > random8(100)){                 // and random chance
          raindrops[b] = {random8(LEDSx), 0, random8(numspeeds), CHSV(101, 0, 255)}; // create a digit at a random x coordinate
          dest[XY(raindrops[b].x, raindrops[b].y)] = desat;       // write to leds
          raindrops[b].y++;       // increase y
        }
      }
      else if (raindrops[b].y == LEDSy){
        dest[XY(raindrops[b].x, raindrops[b].y-1)] = fullsat;     // write to leds
        if(raindrops[b].direction >= count[newPL]){
          raindrops[b].y++;
        }
      }
      else {
        dest[XY(raindrops[b].x, raindrops[b].y-1)] = fullsat;
        dest[XY(raindrops[b].x, raindrops[b].y)] = desat;
        if(raindrops[b].direction >= count[newPL]){
          raindrops[b].y++;
        }
      }
    }
    count[newPL]++;
    if(count[newPL] == numspeeds){
      count[newPL] = 0;
    }
  }
}

void Canada(bool newPL, CRGB *dest){
  patrunproc(newPL, 255, -1, 16, Newspaper);
  for(int y = 0; y < LEDSy; y++){
    for(int x = 0; x < LEDSx; x++){
      if(pgm_read_byte_near(&canadaray[y*LEDSx+x]) == 0){            //white
        dest[XY(x, y)] = CRGB(255, 255, 255);
      }
      else if(pgm_read_byte_near(&canadaray[y*LEDSx+x]) == 1){      //red
        dest[XY(x, y)] = CRGB(255, 0, 0);
      }
      else if(pgm_read_byte_near(&canadaray[y*LEDSx+x]) == 2){     //pink       just set to red for now
        dest[XY(x, y)] = CRGB(255, 0, 0);
      }
    }
  }
}

void Staticeye(bool newPL, CRGB *dest){
  patrunproc(newPL, 255, -1, 16, Newspaper);
  for (int col = 0; col < LEDShalfx; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row < LEDShalfy; row++){
      int standin = beatsin8(12, -1, 1);      // Mess with this to create eye layers and adjust how long it stays at 0, which is the static
      dest[XY(col, row)] = ColorFromPalette(currentPalette, hue[newPL] + (col*standin*row+row), 255, LINEARBLEND);      // Target palette, start hue
      dest[XY(col, LEDSy - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] + (col*standin*row+row), 255, LINEARBLEND);
      dest[XY(LEDSx - 1 - col, row)] = ColorFromPalette(currentPalette, hue[newPL] + (col*standin*row+row), 255, LINEARBLEND);
      dest[XY(LEDSx - 1 - col, LEDSy - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] + (col*standin*row+row), 255, LINEARBLEND);
    }
  }
}

struct dart { // for darts
  uint8_t x;
  uint8_t y;
  uint8_t direction;
  CHSV colour;
};

void Darts(bool newPL, CRGB *dest) {
  patrunproc(newPL, 5, -1, 16, Popsicle);
  const uint8_t numdarts = 20;
  struct dart darto[numdarts];
  const uint8_t dartRate = 2;     // Higher number is fewer blobs
  uint8_t speed = 25;
  EVERY_N_MILLIS(speed){
    //blur( leds, LEDSx, LEDSy, 175);
    //fadeToBlackBy(dest, LEDStotal, 20);
    //Spawn new dart
    if (random8(dartRate) == 0) {
      uint8_t direction = random8(6);
      uint8_t spawnX = random8(LEDSx);
      uint8_t spawnY = random8(LEDSy);
                
      if (direction == 0 || direction == 4){      // right to left extra since matrix is longer than tall
        darto[random8(numdarts)] = {0, spawnY, direction, CHSV(random8(), 255, 255)};
      } else if (direction == 1 || direction == 5){   // left to right
        darto[random8(numdarts)] = {LEDSx-1, spawnY, direction, CHSV(random8(), 255, 255)};
      } else if (direction == 2){   // top to bottom
        darto[random8(numdarts)] = {spawnX, 0, direction, CHSV(random8(), 255, 255)};
      } else if (direction == 3){   // bottom to top
        darto[random8(numdarts)] = {spawnX, LEDSy-1, direction, CHSV(random8(), 255, 255)}; 
      }
    }
  }

  // Draw the darts
  for (int i = 0; i < numdarts; i++) {
    if(darto[i].x >= 0 && darto[i].x < LEDSx && darto[i].y >= 0 && darto[i].y < LEDSy){
      dest[XY(darto[i].x, darto[i].y)] = darto[i].colour;
    }
  }
    
  EVERY_N_MILLIS(speed){
    // Move the darts
    for (int i = 0; i < numdarts; i++) {
      if (darto[i].direction == 0 || darto[i].direction == 4){      // right to left
        darto[i].x++;
      } else if (darto[i].direction == 1 || darto[i].direction == 5){   // left to right
        darto[i].x--;
      } else if (darto[i].direction == 2){   // top to bottom
        darto[i].y++;
      } else if (darto[i].direction == 3){   // bottom to top
        darto[i].y--;
      }
    }
  }
}

void Orbits(bool newPL, CRGB *dest){

}

/*
void beach(bool newPL, CRGB *dest){
  EVERY_N_MILLIS(50){
    fadeToBlackBy(leds, LEDStotal, 20);
    //fader(20);
    uint8_t z = 1;
    //CRGBPalette16 palette = RainbowColors_p;
    
    for(int bead = 0; bead < 1; bead++){
      target[bead][z].x = beatsin8( (bead+1) * 4, 0, (LEDSx)-1);
      target[bead][z].y = beatsin8( (bead+1) * 3, 0, LEDSy - 1);   
      ledmatrix.DrawFilledCircle(target[bead][z].x, target[bead][z].y, 2, CHSV(hue*bead, 255, 255));  // void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color)
    }
  }
}
*/
///////////////////////////// to be converted from Ultramagnet ////////////////////////////////

/*
void Textualizer(bool newPL, CRGB *dest){
  patrunproc(newPL, 255, 1, 16, Tropicana);

  matrix->setTextWrap(false);
  //matrix->setFont( &Roboto_Mono_Medium_24 ); // matrix->setTextSize(size); for defaults
  matrix->setRotation(0);  //change to 1
  matrix->setTextColor(timer);
  matrix->setTextSize(2);
  //strcpy(BTmessage, "WONGLE"); 
  byte strlength = strlen(BTmessage);
  byte fontwidth = 10;
  byte fontheight = 16;
  byte cursorx = LEDSx/2-fontwidth/2;
  
  for(int i = 0; i < strlength; i++){
    uint16_t txtcolor = Color24toColor16(Wheel(hue[newPl]+i*35));
    matrix->setTextColor(txtcolor); //matrix->Color(255,0,255)
    matrix->setCursor(cursorx, count[z]+i*fontheight);
    matrix->print(BTmessage[i]);
  }
  //matrix->show();
  EVERY_N_MILLIS(16){
    count[z]--;
    if( count[z] == min(-LEDSy, -fontheight*strlength)){
      count[z] = LEDSy+fontheight;
    }
  }
}
*/

/*
void Initialization(bool newPL, CRGB *dest){
  patrunproc(newPL, 255, 1, 16, Tropicana);

  byte horzgradient = 0;
  byte centerpoint = (LEDSx-1)/2;
  
  for(int y = 0; y < LEDSy; y++){            //read original values from array and add hue mod
    for(int x = 0; x < LEDSx; x++){
      //byte arrayval = pgm_read_byte_near(&startarray[y*LEDSx+x]);
      //uint8_t hueindex = arrayval * 52 + hue[z] -(abs(centerpoint +0.5 -x) * horzgradient);
      //leds[XY(x, y)] = ColorFromPalette( currentPalette, hueindex, 255 * arrayval, LINEARBLEND); // dont show 0s
      //leds[XY(x, y)] = ColorFromPalette( currentPalette, pgm_read_byte_near(&startarray[y*LEDSx+x])* 52 + hue[z] -(abs(centerpoint +0.5 -x) * horzgradient), 255 * pgm_read_byte_near(&startarray[y*LEDSx+x]), LINEARBLEND);
      
      if(pgm_read_byte_near(&startarray[y*LEDSx+x]) == 2){            //red
        leds[XY(x, y)] = ColorFromPalette( currentPalette, 0 + hue[z] - abs(centerpoint +0.5 -x) * horzgradient, 255, LINEARBLEND);                                     
      } else if(pgm_read_byte_near(&startarray[y*LEDSx+x]) == 3){      //yellow
        leds[XY(x, y)] = ColorFromPalette( currentPalette, 52 + hue[z] - abs(centerpoint +0.5 -x) * horzgradient, 255, LINEARBLEND);
      } else if(pgm_read_byte_near(&startarray[y*LEDSx+x]) == 4){     //green
        leds[XY(x, y)] = ColorFromPalette( currentPalette, 103 + hue[z] - abs(centerpoint +0.5 -x) * horzgradient, 255, LINEARBLEND);
      } else if(pgm_read_byte_near(&startarray[y*LEDSx+x]) == 5){     //cyan
        leds[XY(x, y)] = ColorFromPalette( currentPalette, 154 + hue[z] - abs(centerpoint +0.5 -x) * horzgradient, 255, LINEARBLEND);
      } else if(pgm_read_byte_near(&startarray[y*LEDSx+x]) == 6){     //blue
        leds[XY(x, y)] = ColorFromPalette( currentPalette, 206 + hue[z] - abs(centerpoint +0.5 -x) * horzgradient, 255, LINEARBLEND);
      } else if(pgm_read_byte_near(&startarray[y*LEDSx+x]) == 1){     //white
        leds[XY(x, y)] = CHSV(255, 0, 255);
      } else {                                //black
        leds[XY(x, y)] = CHSV(0, 0, 0);
      }
    }
  }
}
void Sinesides(bool newPL, CRGB *dest){
  patrunproc(newPL, 255, 1, 16, Tropicana);
  // uint8_t altpal = NUMpalettes%palnum;
  CRGBPalette16 palette1 = Palette_List[altpal];
  CRGBPalette16 palette2 = currentPalette;

  for(int y = 0; y < LEDSy; y++){ 
    // Find wavelength, then have it repeat several times at a less intense wavelength
    uint8_t pos = sin8(y*3+hue[newPL]*3);    // uint8_t pos = sin8(hue%25*(y%25))  // or sin8(hue*y/5+y%5*256/5
    byte wavestart = beatsin8(6, 0, 5);
    pos = map(pos, 0, 255, wavestart, LEDSx-1-wavestart);
    dest[XY(pos, y)] += CHSV(255, 0, beatsin8(8, 0, 255)+y*2);
    
    for( int i = 0; i < pos; i++){
      dest[XY(i, y)] = ColorFromPalette(palette1, hue[nwePL] + y*7, 255, LINEARBLEND);
    }
    for( int i = LEDSx -1; i > pos; i--){
      dest[XY(i, y)] = ColorFromPalette(palette2, 128 - hue[newPL] + y*7, 255, LINEARBLEND);
    }
  }
}

void textdisp(bool newPL, CRGB *dest){
  patrunproc(newPL, 255, 1, 16, Tropicana);

  matrix->setTextWrap(false);
  //matrix->setFont( &Roboto_Mono_Medium_24 ); // matrix->setTextSize(size); for defaults
  matrix->setRotation(0);  //change to 1
  matrix->setTextColor(timer);
  matrix->setTextSize(2);
  //strcpy(BTmessage, "WONGLE"); 
  byte strlength = strlen(BTmessage);
  byte fontwidth = 10;
  byte fontheight = 16;
  byte cursorx = LEDSx/2-fontwidth/2;
  
  for(int i = 0; i < strlength; i++){
    uint16_t txtcolor = Color24toColor16(Wheel(hue[z]+i*35));
    matrix->setTextColor(txtcolor); //matrix->Color(255,0,255)
    matrix->setCursor(cursorx, count[z]+i*fontheight);
    matrix->print(BTmessage[i]);
  }
  //matrix->show();
  EVERY_N_MILLIS(16){
    count[z]--;
    if( count[z] == min(-LEDSy, -fontheight*strlength)){
      count[z] = LEDSy+fontheight;
    }
  }
}
*/ ///////////////////////////////////////////////////////////////

NamedPattern Pattern_List[] = {
  {(pattern_func)Sinelon,            F("Sinelon")},
  {(pattern_func)Illusion,           F("Illusion")},
  {(pattern_func)Diagong,            F("Diagong")},
  {(pattern_func)Rando,              F("Rando")},
  {(pattern_func)Confetti,           F("Confetti")},
  {(pattern_func)Vortex,             F("Vortex")},
  {(pattern_func)Tropic,             F("Tropic")},
  {(pattern_func)Juggle,             F("Juggle")},
  {(pattern_func)Flex,               F("Flex")},
  {(pattern_func)Firetoucher,        F("Firetoucher")},
  {(pattern_func)Gentle_Leviathan,   F("Gentle Leviathan")},
  {(pattern_func)Diamondmaw,         F("Diamondmaw")},
  {(pattern_func)Him,                F("Him")},
  {(pattern_func)Praxis,             F("Praxis")},
  {(pattern_func)Barbarism,          F("Barbarism")},
  {(pattern_func)DNA,                F("DNA")},
  {(pattern_func)Gargyle,            F("Gargyle")},
  {(pattern_func)Midearth,           F("Midearth")},
  {(pattern_func)Waterfall,          F("Waterfall")},
  {(pattern_func)Quadplexor,         F("Quadplexor")},
  {(pattern_func)Spectral_Waterfall, F("Spectral Waterfall")},
  {(pattern_func)Digital_Rain,       F("Digital Rain")},
  {(pattern_func)Darts,              F("Darts")},
  {(pattern_func)Orbits,             F("Orbits")},
  {(pattern_func)Canada,             F("Canada")},
  {(pattern_func)Staticeye,          F("Static Eye")},
};

uint16_t const NUMpatterns = (sizeof(Pattern_List) / sizeof(Pattern_List[0]));

#endif