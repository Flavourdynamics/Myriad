#ifndef Myriad_2D_Patterns_h
#define Myriad_2D_Patterns_h
#include <Myriad_Types.h>
#include <Myriad_Arrays.h>
/*
void NewPattern(bool newPL, CRGB *dest){
  //patrunproc(bool newPL, byte fadeamt, int8_t hueinc, uint8_t huespeed, TProgmemRGBGradientPalettePtr matchpal)
  patrunproc(newPL, 255, 1, 16, Blackwhite);
}
*/
void Rando(bool newPL, CRGB *dest){
  blackout(dest);
  patrunproc(newPL, 255, 1, 16, RainbowColors);
  int lowernummorelights = 150;
  if(palnum == palmatch){
    for (int i = 0; i < LEDtotal/lowernummorelights ; i++){  // runs 3 times - better boot pattern? (fade?)
      uint16_t targetx = random16(0, LEDstrips);
      uint16_t targety = random16(0, LEDper);
      dest[XY(targetx, targety)] = CHSV(random8(), random8(), random8(150, 255));
    }
  } else {
    for (int i = 0; i < LEDtotal/lowernummorelights ; i++){  // runs 3 times - better boot pattern? (fade?)
      uint16_t targetx = random16(0, LEDstrips);
      uint16_t targety = random16(0, LEDper);
      dest[XY(targetx, targety)] = ColorFromPalette(currentPalette, random8(), 255, LINEARBLEND);
      //leds[targetx][targety]set.Sat ) random8(155,255)
    }
  }
}
 
void Illusion(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 1, 16, Blackwhite);
  for (int y = 0; y < LEDhalfper; y++){
    for (int x = 0; x < LEDhalfstrips; x++){
      dest[XY(x, y)] = ColorFromPalette(currentPalette, hue[newPL] + x*25 + y*25, 255, LINEARBLEND);                      //top left
      dest[XY(LEDstrips-1-x, y)] = ColorFromPalette(currentPalette, hue[newPL] + x*25 + y*25, 255, LINEARBLEND);          //top right
      dest[XY(x, LEDper-1-y)] = ColorFromPalette(currentPalette, hue[newPL] + x*25 + y*25, 255, LINEARBLEND);             //bottom left
      dest[XY(LEDstrips-1-x, LEDper-1-y)] = ColorFromPalette(currentPalette, hue[newPL] + x*25 + y*25, 255, LINEARBLEND); //bottom right 
    }
  }
}

void Diagonal(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 1, 16, Startup);
  //Serial.println(newPL);
  for (int y = 0; y < rowcount[newPL]; y++){
    for (int x = 0; x < colcount[newPL]; x++){
       dest[XY(x, y)] = ColorFromPalette( currentPalette, hue[newPL]+(x+y)*10, 255, LINEARBLEND); // normal palette access
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

void Scan(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 1, 16, RainbowColors);
  for (int y = 0; y < LEDper; y++){
    for (int x = 0; x < LEDstrips; x++){
      dest[XY(x, y)] = ColorFromPalette(currentPalette, hue[newPL], random8(100,150), LINEARBLEND); 
    } 
  }
}

void Confetti(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 1, 16, RainbowColors);
  //blackout();
  for(int x = 0; x < LEDstrips; x++){
    int y = random16(LEDper);
    dest[XY(x, y)] += ColorFromPalette(currentPalette, hue[newPL] + random8(64), 200, LINEARBLEND);
  }
}

void Sinelon(bool newPL, CRGB *dest){         // A romantic wave of 
  patrunproc(newPL, 20, 1, 16, RainbowColors);
  for(int bead = 1; bead < 10; bead++){
    int xpos = beatsin16((bead * 3 / beatmod) + 1, 0, LEDstrips - 1);
    int ypos = beatsin16((bead * 2 / beatmod) + 1, 0, LEDper - 1);
    dest[XY(xpos, ypos)] += ColorFromPalette(currentPalette, hue[newPL] * bead, 255, LINEARBLEND);
  }
  blur( dest, LEDstrips, LEDper, 128);
  FastLED.show();
}

void Vortex(bool newPL, CRGB *dest ){
  patrunproc(newPL, 20, 1, 16, RainbowColors);
  for(int y = 0; y < LEDper; y++){
    uint8_t pos = sin8(hue[newPL]*(y+1));   
    pos = map(pos, 1, 248, 0, LEDstrips-1);
    dest[XY(pos, y)] += ColorFromPalette(currentPalette, hue[newPL]+y/2, 255, LINEARBLEND);
  }
  
}

void Tropic(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 1, 16, Tropicana);
  uint16_t beat = beatsin16( 14, 25, 455);
  for(int x = 0; x < LEDstrips; x++){
    for( int y = 0; y < LEDper; y++){
      dest[XY(x, y)] = ColorFromPalette(currentPalette, hue[newPL]+(y*10)+x*2, beat-hue[newPL]+(y*10)-x*2, LINEARBLEND);   //dest[x][y] = ColorFromPalette(currentPalette, hue+(y*10)+x*2, beat-hue+(y*10)-x*2, LINEARBLEND);
    }
  }
}

void Juggle(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 1, 16, RainbowColors);
  for(int x = 0; x < LEDstrips; x++){
    byte dothue = 0;
    for( int i = 0; i < 5; i++){
      dest[XY(x, beatsin16( (i+7)/beatmod, 0, LEDper-1 ))] |= ColorFromPalette(currentPalette, hue[newPL] + dothue+x*3, 255, LINEARBLEND);
      dothue += 51;
    }
  }
}

void Flex(bool newPL, CRGB *dest){
  patrunproc(newPL, 20, 1, 16, RainbowColors);
  for (int col = 0; col < LEDstrips; col++){         // Write each row with start colour and a random saturation
    int beatbox = beatsin8(20/beatmod, 3, abs(14-col) + 15);
    for(int row = 0; row < rowcount[newPL]; row++){
      dest[XY(col, row)] = ColorFromPalette(currentPalette, hue[newPL] + beatbox*row, 255, LINEARBLEND); // fill_rainbow(leds[x], number of leds, start hue, hue delta)  
      dest[XY(LEDstrips-1-col, row)] = ColorFromPalette(currentPalette, hue[newPL] + beatbox*row, 255, LINEARBLEND);  
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

void Firetoucher(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 1, 24, Redyell);
  for (int col = 0; col < LEDhalfstrips; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row < LEDhalfper; row++){
      int standin = beatsin8(60, 5, 10);      // BPM, MIN, MAX
      dest[XY(col, row)] = ColorFromPalette(currentPalette, hue[newPL] + standin * -col + row * 5, 255, LINEARBLEND);      // Target palette, start hue
      dest[XY(col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] + standin * -col + row * 5, 255, LINEARBLEND);
      dest[XY(LEDstrips - 1 - col, row)] = ColorFromPalette(currentPalette, hue[newPL] + standin * -col + row * 5, 255, LINEARBLEND);
      dest[XY((LEDstrips - 1 - col), LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] + standin * -col + row * 5, 255, LINEARBLEND);
    }
  } 
}

void Gentle_Leviathan(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 1, 24, RainbowColors);
  //blackout(LEDmatrixCRGB);
  uint16_t numbeads = numnoise/2;
  uint16_t lowbound = 14000;
  uint16_t hibound = 52000;

  for(int bead = 1; bead < numbeads; bead++){
    noise[newPL][bead][0] = inoise16(noise[newPL][bead][0], timer[newPL]+bead*1000);     // get some noise
    noise[newPL][bead][0] = constrain(noise[newPL][bead][0], lowbound, hibound);               // keep it within a reasonable scalable boundary (aesthetic judgement)
    uint16_t xpos = map(noise[newPL][bead][0], lowbound, hibound, 0, LEDstrips);        // Map it to width

    noise[newPL][bead][1] = inoise16(noise[newPL][bead][1], timer[newPL]*2+bead*1000); //random8() this was in the last arg
    noise[newPL][bead][1] = constrain(noise[newPL][bead][1], lowbound, hibound); 
    uint16_t ypos = map(noise[newPL][bead][1], lowbound, hibound, 0, LEDper);

    //ledmatrix.DrawFilledCircle(xpos, ypos, 2, blend(ColorFromPalette(currentPalette, hue[newPL] + bead*1, 255, LINEARBLEND), CRGB::White, 55));
  }
  for (uint16_t i = 0; i < LEDtotal; i++) {   // blend em
    //dest[i] = blend( dest[i], LEDmatrixCRGB[i], 45);   // Blend arrays of LEDs, third value is blend %
  }
  for (uint16_t i = 0; i < LEDtotal; i++) {   // blend em
    dest[i] = blend( dest[i], CRGB::White, 5);   // Blend arrays of LEDs, third value is blend %
  }
  //blend( dest, LEDmatrixCRGB, 255);
  //memcpy8(dest, LEDmatrixCRGB, LEDtotal*3);
  blur(dest, LEDstrips, LEDper, 45);
  timer[newPL]+=150;
}

void Diamondmaw(bool newPL, CRGB *dest){
  patrunproc(newPL, 10, -1, 16, Purple_Cascade);
  for (int col = 0; col < LEDhalfstrips; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row < LEDhalfper; row++){
      
      int standin = beatsin8(14, -5, 25);      // BPM, MIN, MAX
      dest[XY(col, row)] = ColorFromPalette(currentPalette, hue[newPL] - (col+row)*standin, 255, LINEARBLEND);      // Target palette, start hue
      dest[XY(col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] - (col+row)*standin, 255, LINEARBLEND);
      dest[XY(LEDstrips - 1 - col, row)] = ColorFromPalette(currentPalette, hue[newPL] - (col+row)*standin, 255, LINEARBLEND);
      dest[XY(LEDstrips - 1 - col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] - (col+row)*standin, 255, LINEARBLEND);
    }
  }
}

void Him(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, -1, 10, Gilt);
  for (int col = 0; col < LEDhalfstrips; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row < LEDhalfper; row++){   
      int standin = beatsin16(14, 50, 300);      // BPM, MIN, MAX
      dest[XY(col, row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(400-standin)/250 + (col+row*standin*col)/350, 255, LINEARBLEND);      // Target palette, start hue
      dest[XY(col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(400-standin)/250 + (col+row*standin*col)/350, 255, LINEARBLEND);
      dest[XY(LEDstrips - 1 - col, row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(400-standin)/250 + (col+row*standin*col)/350, 255, LINEARBLEND);
      dest[XY(LEDstrips - 1 - col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(400-standin)/250 + (col+row*standin*col)/350, 255, LINEARBLEND);
    }
  }
}

void lilminfuk(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, -1, 16, RainbowStripeColors);
  int standin = beatsin16(8, 75, 175);      // BPM, MIN, MAX
  for (int col = 0; col < LEDhalfstrips; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row < LEDhalfper; row++){      
      dest[XY(col, row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(250-standin)/5 + (col+row*standin*col)/250, 255, LINEARBLEND);      // Target palette, start hue
      dest[XY(col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(250-standin)/5 + (col+row*standin*col)/250, 255, LINEARBLEND);
      dest[XY(LEDstrips - 1 - col, row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(250-standin)/5 + (col+row*standin*col)/250, 255, LINEARBLEND);
      dest[XY(LEDstrips - 1 - col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[newPL] + (col+row)*(250-standin)/5 + (col+row*standin*col)/250, 255, LINEARBLEND);
    }
  }
}

void Barbarism(bool newPL, CRGB *dest){
  patrunproc(newPL, 20, 1, 16, Purple_Cascade);
  static uint16_t xmarksthespot;
  uint16_t barlength = 5;
  uint16_t upperlim = NUMleds - barlength -1;
  EVERY_N_MILLIS(100){
    xmarksthespot = random16(0, upperlim);
  }
  for (uint16_t i = xmarksthespot; i < barlength + xmarksthespot; i++) {
    dest[i] = ColorFromPalette(currentPalette, i*15+hue[newPL], 255, LINEARBLEND);
  }
}

void DNA(bool newPL, CRGB *dest){
  patrunproc(newPL, 20, 1, 16, Purple_Cascade);
  byte threads = 4;
  byte phaseoff = 255/threads;
  for(int y = 0; y < LEDper; y++){ // For each row
    for(byte p = 0; p < threads; p++){                        // p is phase mod, 128 coefficient is 64 degrees out of phase
      // Format: dest[coordinates oscillate between left and right with next lines out of phase] =  CHSV(base hue+difference between rows, saturation changes a little bit for variety, value varies by row)
      // Increase coefficients of 'y' terms to increase the differences between rows
      //dest[XY(beatsin8(10, 0, LEDstrips-1, 0, y*4+p*phaseoff), y)] = CHSV(-hue+y*5+p*phaseoff, beatsin8(25, 200, 255, 0, y*3), beatsin8(5, 55, 255, 0, y*10+p*phaseoff)); 
      dest[XY(beatsin8(10, 0, LEDstrips-1, 0, y*4+p*phaseoff), y)] = CHSV(-hue[newPL]+y*5+p*phaseoff, 255, beatsin8(5, 55, 255, 0, y*10+p*phaseoff));                 
    }
  }
  blur( dest, LEDstrips, LEDper, 255);
}

void Gargyle(bool newPL, CRGB *dest){
  patrunproc(newPL, 1, 1, 16, Purple_Cascade);
  // NO FADE TO MAKE IT CHECKERBOARD
  byte threads = 4;
  byte phaseoff = 255/threads;
  for(int y = 0; y < LEDper; y++){ // For each row
    for(byte p = 0; p < threads; p++){           // p is phase mod, 128 coefficient is 64 degrees out of phase
      // Format: dest[coordinates oscillate between left and right with next lines out of phase] =  CHSV(base hue+difference between rows, saturation changes a little bit for variety, value varies by row)
      // Increase coefficients of 'y' terms to increase the differences between rows
      //dest[XY(beatsin8(10, 0, LEDstrips-1, 0, y*4+p*phaseoff), y)] = CHSV(-hue+y*5+p*phaseoff, beatsin8(25, 200, 255, 0, y*3), beatsin8(5, 55, 255, 0, y*10+p*phaseoff));    
      dest[XY(beatsin8(10, 0, LEDstrips-1, 0, y*4+p*phaseoff), y)] = CHSV(-hue[newPL]+y*5+p*phaseoff, 255, beatsin8(5, 55, 255, 0, y*10+p*phaseoff));              
    }
  }
  //blur2d( leds, LEDstrips, LEDper, 128);
}

void Midearth(bool newPL, CRGB *dest){
  patrunproc(newPL, 0, 1, 16, Purple_Cascade);
  // NO FADE TO MAKE IT CHECKERBOARD
  byte threads = 4;
  byte phaseoff = 255/threads;
  for(int y = 0; y < LEDper; y++){ // For each row
    for(byte p = 0; p < threads; p++){                        // p is phase mod, 128 coefficient is 64 degrees out of phase
      // Format: dest[coordinates oscillate between left and right with next lines out of phase] =  CHSV(base hue+difference between rows, saturation changes a little bit for variety, value varies by row)
      // Increase coefficients of 'y' terms to increase the differences between rows
      //dest[XY(beatsin8(10, 0, LEDstrips-1, 0, y*4+p*phaseoff), y)] = CHSV(-hue+y*5+p*phaseoff, beatsin8(25, 200, 255, 0, y*3), beatsin8(5, 55, 255, 0, y*10+p*phaseoff));    
      dest[XY(beatsin8(10, 0, LEDstrips-1, 0, y*4+p*phaseoff), y)] = CHSV(-hue[newPL]+y*5+p*phaseoff, 255, beatsin8(5, 55, 255, 0, y*10+p*phaseoff));              
    }
  }
  //blur2d( leds, LEDstrips, LEDper, 128);
}

void Waterfall(bool newPL, CRGB *dest){
  patrunproc(newPL, 255, -1, 16, CloudColors);
  for (int row = 0; row < rowcount[newPL]; row++){
    for (int col = 0; col < LEDstrips; col++){         // Write each row with start colour and a random saturation      
      dest[XY(col, row)] = ColorFromPalette(currentPalette, hue[newPL] + row*15+sin8((row*7 + col*10+count[newPL]*2))*(1+row)/80, 255, LINEARBLEND);          // This monster determines how much the wave twists, sin8 function is for base twist, and stuff to the left is for how much twist per row 
    }          
  }
  EVERY_N_MILLIS(100){
    if(rowcount[newPL] < LEDper){
      rowcount[newPL]++;
    }  
   count[newPL]++; 
  }
}

void Quadplexor(bool newPL, CRGB *dest){
  EQ.proc();
  patrunproc(newPL, 255, 1, 16, Tropicana);
  for(int band = 0; band < EQbins; band++){
    byte z = map(EQ.EQflatdecline[band], 0, LEDper, 0, LEDper/2);
    for(int leng = 0; leng < z; leng++){ // Display as 00 11 22 33 44 55 66 66 55 44 33 22 11 00  CHSV(hue+leng*5-s*7, 255, 255); EQscaled[band]
      dest[XY(LEDstrips/2 -1  -band,   LEDper/2    -2   -leng)] = CHSV(hue[newPL] + band*5 + leng*5, 255, 255);  // Top left
      dest[XY(LEDstrips/2     +band,   LEDper/2    -2   -leng)] = CHSV(hue[newPL] + band*5 + leng*5, 255, 255);    // Top right
      dest[XY(LEDstrips/2 -1  -band,   LEDper/2    -1   +leng)] = CHSV(hue[newPL] + band*5 + leng*5, 255, 255);  // Bottom left
      dest[XY(LEDstrips/2     +band,   LEDper/2    -1   +leng)] = CHSV(hue[newPL] + band*5 + leng*5, 255, 255);    // Bottom right
    }
    if(z > 0){ //peaks
      dest[XY(LEDstrips/2 -1  -band,   LEDper/2    -1   -z )] = CHSV(255, 0, 255);  // Top left
      dest[XY(LEDstrips/2     +band,   LEDper/2    -1   -z )] = CHSV(255, 0, 255);    // Top right
      dest[XY(LEDstrips/2 -1  -band,   LEDper/2    -2   +z )] = CHSV(255, 0, 255);  // Bottom left
      dest[XY(LEDstrips/2     +band,   LEDper/2    -2   +z )] = CHSV(255, 0, 255);    // Bottom right
    }
  }
}

void Spectral_Waterfall(bool newPL, CRGB *dest){
  EQ.proc();
  patrunproc(newPL, 0, 1, 16, Heatmap);
  EVERY_N_MILLIS(20){
    memmove8( &dest[XY(0,1)], &dest[XY(0,0)], LEDtotal * sizeof( CRGB) );
    for(int band = 0; band < EQbins; band++){
      byte tempvalue = map(EQ.EQ10000scaled[band], 0, 255, 0, 10000);
      dest[XY(LEDstrips/2+band, 0)] = ColorFromPalette(currentPalette, tempvalue, 127+(tempvalue*0.5), LINEARBLEND);
      dest[XY(LEDstrips/2-1-band, 0)] = ColorFromPalette(currentPalette, tempvalue, 127+(tempvalue*0.5), LINEARBLEND);
    }
  }
}

void Digital_Rain(bool newPL, CRGB *dest){           // Have to randomize target locations on first boot
  EQ.proc();
  patrunproc(newPL, 20, 1, 16, Pastel);
  uint8_t numofcodes = 60;    // struct targets {uint8_t x, uint8_t y, uint8_t direction, CHSV colour};
  uint8_t numspeeds = 3;      // how many different speeds of objects
  EVERY_N_MILLIS(10){
    for(uint16_t b = 0; b < numofcodes; b++){
      uint16_t trianglemap = abs((b+(EQbins-1))%(LEDstrips-1)-(EQbins-1));
      uint8_t hitrate = 2 + EQ.EQ10000scaled[abs((b+(EQbins-1))%(LEDstrips-1)-(EQbins-1))]/LEDper/6; //triangle mapping
      CRGB fullsat = ColorFromPalette(currentPalette, hue[newPL] + trianglemap*5, 255, LINEARBLEND); // fully saturated value for tails
      CRGB desat = blend(fullsat, CRGB::White, 93); //desaturated value for leads
      if(targets[newPL][b].y > LEDper){             // if greater than range, do reset
        if(hitrate > random8(100)){                 // and random chance
          targets[newPL][b] = {random8(LEDstrips), 0, random8(numspeeds), CHSV(101, 0, 255)}; // create a digit at a random x coordinate
          dest[XY(targets[newPL][b].x, targets[newPL][b].y)] = desat;       // write to leds
          targets[newPL][b].y++;       // increase y
        }
      }
      else if (targets[newPL][b].y == LEDper){
        dest[XY(targets[newPL][b].x, targets[newPL][b].y-1)] = fullsat;     // write to leds
        if(targets[newPL][b].direction >= count[newPL]){
          targets[newPL][b].y++;
        }
      }
      else {
        dest[XY(targets[newPL][b].x, targets[newPL][b].y-1)] = fullsat;
        dest[XY(targets[newPL][b].x, targets[newPL][b].y)] = desat;
        if(targets[newPL][b].direction >= count[newPL]){
          targets[newPL][b].y++;
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
  for(int y = 0; y < LEDper; y++){            //read original values from array and add hue mod
    for(int x = 0; x < LEDstrips; x++){
      if(pgm_read_byte_near(&canadaray[y*LEDstrips+x]) == 0){            //white
        dest[XY(x, y)] = CRGB(255, 255, 255);
      } 
      else if(pgm_read_byte_near(&canadaray[y*LEDstrips+x]) == 1){      //red
        dest[XY(x, y)] = CRGB(255, 0, 0);
      } 
      else if(pgm_read_byte_near(&canadaray[y*LEDstrips+x]) == 2){     //pink       just set to red for now
        dest[XY(x, y)] = CRGB(255, 0, 0);
      } 
    }
  }
}
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
  byte cursorx = LEDstrips/2-fontwidth/2;
  
  for(int i = 0; i < strlength; i++){
    uint16_t txtcolor = Color24toColor16(Wheel(hue[newPl]+i*35));
    matrix->setTextColor(txtcolor); //matrix->Color(255,0,255)
    matrix->setCursor(cursorx, count[z]+i*fontheight);
    matrix->print(BTmessage[i]);
  }
  //matrix->show();
  EVERY_N_MILLIS(16){
    count[z]--;
    if( count[z] == min(-LEDper, -fontheight*strlength)){
      count[z] = LEDper+fontheight;
    }
  }
}


void darts(bool newPL, CRGB *dest) {
  const uint8_t numdarts = 20;
  const uint8_t dartRate = 2;     // Higher number is fewer blobs
  uint8_t z = 1;

  EVERY_N_MILLIS(50){
    //blur( leds, LEDstrips, LEDper, 175);
    fadeToBlackBy(leds, LEDtotal, 20);
    //Spawn new dart
    if (random8(dartRate) == 0) {
      uint8_t direction = random8(6);
      uint8_t spawnX = random8(LEDstrips);
      uint8_t spawnY = random8(LEDper);
                
      if (direction == 0 || direction == 4){      // right to left extra since matrix is longer than tall
          target[random8(numdarts)][z] = {0, spawnY, direction, CHSV(random8(), 255, 255)};
        } else if (direction == 1 || direction == 5){   // left to right
          target[random8(numdarts)][z] = {LEDstrips-1, spawnY, direction, CHSV(random8(), 255, 255)};
        } else if (direction == 2){   // top to bottom
          target[random8(numdarts)][z] = {spawnX, 0, direction, CHSV(random8(), 255, 255)};
        } else if (direction == 3){   // bottom to top
          target[random8(numdarts)][z] = {spawnX, LEDper-1, direction, CHSV(random8(), 255, 255)}; 
        }
    }
    
    // Draw the darts
    for (int i = 0; i < numdarts; i++) {
      if(target[i][z].x >= 0 && target[i][z].x < LEDstrips && target[i][z].y >= 0 && target[i][z].y < LEDper){
        leds[XY(target[i][z].x, target[i][z].y)] = target[i][z].colour;
      }
    }
    
    // Move the darts
    for (int i = 0; i < numdarts; i++) {
      if (target[i][z].direction == 0 || target[i][z].direction == 4){      // right to left
        target[i][z].x++;
      } else if (target[i][z].direction == 1 || target[i][z].direction == 5){   // left to right
        target[i][z].x--;
      } else if (target[i][z].direction == 2){   // top to bottom
        target[i][z].y++;
      } else if (target[i][z].direction == 3){   // bottom to top
        target[i][z].y--;
      }     
    }
  }
  //
}
void beach(bool newPL, CRGB *dest){
  EVERY_N_MILLIS(50){
    fadeToBlackBy(leds, LEDtotal, 20);
    //fader(20);
    uint8_t z = 1;
    //CRGBPalette16 palette = RainbowColors_p;
    
    for(int bead = 0; bead < 1; bead++){
      target[bead][z].x = beatsin8( (bead+1) * 4, 0, (LEDstrips)-1);
      target[bead][z].y = beatsin8( (bead+1) * 3, 0, LEDper - 1);   
      ledmatrix.DrawFilledCircle(target[bead][z].x, target[bead][z].y, 2, CHSV(hue*bead, 255, 255));  // void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color)
    }
  }
}
*/
///////////////////////////// to be converted from Ultramagnet ////////////////////////////////
/*
void Visualizer(bool newPL, CRGB *dest){
  patrunproc(newPL, 255, 1, 16, Tropicana);  
  for(int band = 0; band < EQbins; band++){
    for(int leng = 0; leng < EQscaled[band]; leng++){ // Display as 00 11 22 33 44 55 66 66 55 44 33 22 11 00  CHSV(hue+leng*5-s*7, 255, 255); EQscaled[band]
      uint8_t ahue = hue[z] +leng*5 +band*5;
      leds[XY(LEDstrips/2+band, leng)] = ColorFromPalette(currentPalette, ahue, 255, LINEARBLEND);
      leds[XY(LEDstrips/2-1-band, leng)] = ColorFromPalette(currentPalette, ahue, 255, LINEARBLEND);
    }
  }
}

void Initialization(bool newPL, CRGB *dest){
  patrunproc(newPL, 255, 1, 16, Tropicana);

  byte horzgradient = 0;
  byte centerpoint = (LEDstrips-1)/2;
  
  for(int y = 0; y < LEDper; y++){            //read original values from array and add hue mod
    for(int x = 0; x < LEDstrips; x++){
      //byte arrayval = pgm_read_byte_near(&startarray[y*LEDstrips+x]);
      //uint8_t hueindex = arrayval * 52 + hue[z] -(abs(centerpoint +0.5 -x) * horzgradient);
      //leds[XY(x, y)] = ColorFromPalette( currentPalette, hueindex, 255 * arrayval, LINEARBLEND); // dont show 0s
      //leds[XY(x, y)] = ColorFromPalette( currentPalette, pgm_read_byte_near(&startarray[y*LEDstrips+x])* 52 + hue[z] -(abs(centerpoint +0.5 -x) * horzgradient), 255 * pgm_read_byte_near(&startarray[y*LEDstrips+x]), LINEARBLEND);
      
      if(pgm_read_byte_near(&startarray[y*LEDstrips+x]) == 2){            //red
        leds[XY(x, y)] = ColorFromPalette( currentPalette, 0 + hue[z] - abs(centerpoint +0.5 -x) * horzgradient, 255, LINEARBLEND);                                     
      } else if(pgm_read_byte_near(&startarray[y*LEDstrips+x]) == 3){      //yellow
        leds[XY(x, y)] = ColorFromPalette( currentPalette, 52 + hue[z] - abs(centerpoint +0.5 -x) * horzgradient, 255, LINEARBLEND);
      } else if(pgm_read_byte_near(&startarray[y*LEDstrips+x]) == 4){     //green
        leds[XY(x, y)] = ColorFromPalette( currentPalette, 103 + hue[z] - abs(centerpoint +0.5 -x) * horzgradient, 255, LINEARBLEND);
      } else if(pgm_read_byte_near(&startarray[y*LEDstrips+x]) == 5){     //cyan
        leds[XY(x, y)] = ColorFromPalette( currentPalette, 154 + hue[z] - abs(centerpoint +0.5 -x) * horzgradient, 255, LINEARBLEND);
      } else if(pgm_read_byte_near(&startarray[y*LEDstrips+x]) == 6){     //blue
        leds[XY(x, y)] = ColorFromPalette( currentPalette, 206 + hue[z] - abs(centerpoint +0.5 -x) * horzgradient, 255, LINEARBLEND);
      } else if(pgm_read_byte_near(&startarray[y*LEDstrips+x]) == 1){     //white
        leds[XY(x, y)] = CHSV(255, 0, 255);
      } else {                                //black
        leds[XY(x, y)] = CHSV(0, 0, 0);
      }
    }
  }
}
void Sinesides(bool newPL, CRGB *dest){
  patrunproc(newPL, 255, 1, 16, Tropicana);
  CRGBPalette16 palette1 = gilt;
  CRGBPalette16 palette2 = currentPalette;

  for(int y = 0; y < LEDper; y++){ 
    uint8_t pos = sin8(y*3+hue[z]*3) ;                                               // find wavelength, then have it repeatt several times at a less intense wavelength             uint8_t pos = sin8(hue%25*(y%25))  // or sin8(hue*y/5+y%5*256/5
    byte wavestart = beatsin8(6, 0, 5);
    pos = map(pos, 0, 255, wavestart, LEDstrips-1-wavestart);
    leds[XY(pos, y)] += CHSV(255, 0, beatsin8(8, 0, 255)+y*2);
    
    for( int i = 0; i < pos; i++){     
      leds[XY(i, y)] = ColorFromPalette(palette1, hue[z] + y*7, 255, LINEARBLEND);
    }
    for( int i = LEDstrips -1; i > pos; i--){
      leds[XY(i, y)] = ColorFromPalette(palette2, 128 - hue[z] + y*7, 255, LINEARBLEND);
    }
  }
}

void Staticeye(bool newPL, CRGB *dest){
  patrunproc(newPL, 255, -1, 16, Newspaper);
  
  for (int col = 0; col < LEDstrips/2; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row <LEDper/2; row++){
      
      int standin = beatsin8(20, -1, 1);      // Mess with this to create eye layers and adjust how long it stays at 0, which is the static
      leds[XY(col, row)] = ColorFromPalette(currentPalette, hue[z] + (col*standin*row+row), 255, LINEARBLEND);      // Target palette, start hue
      leds[XY(col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[z] + (col*standin*row+row), 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, row)] = ColorFromPalette(currentPalette, hue[z] + (col*standin*row+row), 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[z] + (col*standin*row+row), 255, LINEARBLEND);
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
  byte cursorx = LEDstrips/2-fontwidth/2;
  
  for(int i = 0; i < strlength; i++){
    uint16_t txtcolor = Color24toColor16(Wheel(hue[z]+i*35));
    matrix->setTextColor(txtcolor); //matrix->Color(255,0,255)
    matrix->setCursor(cursorx, count[z]+i*fontheight);
    matrix->print(BTmessage[i]);
  }
  //matrix->show();
  EVERY_N_MILLIS(16){
    count[z]--;
    if( count[z] == min(-LEDper, -fontheight*strlength)){
      count[z] = LEDper+fontheight;
    }
  }
}
*/ ///////////////////////////////////////////////////////////////
/*
void Rainbow(bool newPL, CRGB *dest) {
  palettetargeting(1);
  uint8_t z = fetcher(18);
  fill_rainbow( dest, NUM_LEDS, gHue, 7);
}

void RainbowWithGlitter(bool newPL, CRGB *dest) 
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

void confetti(bool newPL, CRGB *dest) 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

*/
/*
void changingBars(int band, int barHeight) {
  int xStart = BAR_WIDTH * band;
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    for (int y = 0; y < barHeight; y++) {
      leds[XY(x,y)] = CHSV(y * (255 / kMatrixHeight) + colorTimer, 255, 255); 
    }
  }
}

void centerBars(int band, int barHeight) {
  int xStart = BAR_WIDTH * band;
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    if (barHeight % 2 == 0) barHeight--;
    int yStart = ((kMatrixHeight - barHeight) / 2 );
    for (int y = yStart; y <= (yStart + barHeight); y++) {
      int colorIndex = constrain((y - yStart) * (255 / barHeight), 0, 255);
      leds[XY(x,y)] = ColorFromPalette(heatPal, colorIndex);
    }
  }
}

void whitePeak(int band) {
  int xStart = BAR_WIDTH * band;
  int peakHeight = peak[band];
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    leds[XY(x,peakHeight)] = CRGB::White;
  }
}

void outrunPeak(int band) {
  int xStart = BAR_WIDTH * band;
  int peakHeight = peak[band];
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    //leds[XY(x,peakHeight)] = CHSV(peakHeight * (255 / kMatrixHeight), 255, 255);
    leds[XY(x,peakHeight)] = ColorFromPalette(outrunPal, peakHeight * (255 / kMatrixHeight));
  }
}

void waterfall(int band) {
  int xStart = BAR_WIDTH * band;
  double highestBandValue = 60000;        // Set this to calibrate your waterfall

  // Draw bottom line
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    leds[XY(x,0)] = CHSV(constrain(map(bandValues[band],0,highestBandValue,160,0),0,160), 255, 255);
  }

  // Move screen up starting at 2nd row from top
  if (band == NUM_BANDS - 1){
    for (int y = kMatrixHeight - 2; y >= 0; y--) {
      for (int x = 0; x < kMatrixWidth; x++) {
        leds[XY(x,y+1)] = leds[XY(x,y)];
      }
    }
  }
}
*/

NamedPattern Pattern_List[] = {
  {(pattern_func)Sinelon,            F("Sinelon")},
  {(pattern_func)Illusion,           F("Illusion")},
  {(pattern_func)Diagonal,           F("Diagonal")},
  {(pattern_func)Rando,              F("Rando")},
  {(pattern_func)Scan,               F("Scan")},
  {(pattern_func)Confetti,           F("Confetti")},
  {(pattern_func)Vortex,             F("Vortex")},
  {(pattern_func)Tropic,             F("Tropic")},
  {(pattern_func)Juggle,             F("Juggle")},
  {(pattern_func)Flex,               F("Flex")},
  {(pattern_func)Firetoucher,        F("Firetoucher")},
  {(pattern_func)Gentle_Leviathan,   F("Gentle Leviathan")},
  {(pattern_func)Diamondmaw,         F("Diamondmaw")},
  {(pattern_func)Him,                F("Him")},
  {(pattern_func)lilminfuk,          F("lilminfuk")},
  {(pattern_func)Barbarism,          F("Barbarism")},
  {(pattern_func)DNA,                F("DNA")},
  {(pattern_func)Gargyle,            F("Gargyle")},
  {(pattern_func)Midearth,           F("Midearth")},
  {(pattern_func)Waterfall,          F("Waterfall")},
  {(pattern_func)Quadplexor,         F("Quadplexor")},
  {(pattern_func)Spectral_Waterfall, F("Spectral Waterfall")},
  {(pattern_func)Digital_Rain,       F("Digital Rain")},
  //{(pattern_func)Textualizer,   F("Textualizer")},
  {(pattern_func)Canada,             F("Canada")},
};

uint16_t const NUMpatterns = (sizeof(Pattern_List) / sizeof(Pattern_List[0]));

#endif