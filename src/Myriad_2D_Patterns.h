#ifndef Myriad_2D_Patterns_h
#define Myriad_2D_Patterns_h
#include <Myriad_Types.h>
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

void dacirc(bool newPL, CRGB *dest){
  patrunproc(newPL, deffade, 1, 24, RainbowColors);
  blackout(LEDmatrixCRGB);
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

    ledmatrix.DrawFilledCircle(xpos, ypos, 2, blend(ColorFromPalette(currentPalette, hue[newPL] + bead*1, 255, LINEARBLEND), white, 55));
  }
  for (uint16_t i = 0; i < LEDtotal; i++) {   // blend em
    dest[i] = blend( dest[i], LEDmatrixCRGB[i], 45);   // Blend arrays of LEDs, third value is blend %
  }
  for (uint16_t i = 0; i < LEDtotal; i++) {   // blend em
    dest[i] = blend( dest[i], white, 5);   // Blend arrays of LEDs, third value is blend %
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
  patrunproc(newPL, 0, 1, 16, Purple_Cascade);
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

void Quadplexor(bool newPL, CRGB *dest){  
  patrunproc(newPL, 255, 1, 16, Tropicana);
  for(int band = 0; band < EQbins; band++){
    byte z = map(EQ.EQscaled[band], 0, LEDper, 0, LEDper/2);
    for(int leng = 0; leng < z; leng++){                // Display as 00 11 22 33 44 55 66 66 55 44 33 22 11 00  CHSV(hue+leng*5-s*7, 255, 255); EQscaled[band]
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
///////////////////////////// to be converted from Ultramagnet ////////////////////////////////
/*
void visualizer(){
  palettetargeting(11);
  uint8_t z = fetcher(1);
  
  blackout();
  for(int band = 0; band < EQbins; band++){
    for(int leng = 0; leng < EQscaled[band]; leng++){ // Display as 00 11 22 33 44 55 66 66 55 44 33 22 11 00  CHSV(hue+leng*5-s*7, 255, 255); EQscaled[band]
      uint8_t ahue = hue[z] +leng*5 +band*5;
      leds[XY(LEDstrips/2+band, leng)] = ColorFromPalette(currentPalette, ahue, 255, LINEARBLEND);
      leds[XY(LEDstrips/2-1-band, leng)] = ColorFromPalette(currentPalette, ahue, 255, LINEARBLEND);
    }
  }
  huepusher(1, z, 16);
}

void waterfall(){
  palettetargeting(13);
  uint8_t z = fetcher(2);
  
  for (int row = 0; row < rowcount[z]; row++){
    for (int col = 0; col < LEDstrips; col++){         // Write each row with start colour and a random saturation      
      leds[XY(col, row)] = ColorFromPalette(currentPalette, hue[z] + row*15+sin8((row*7 + col*10+count[z]*2))*(1+row)/80, 255, LINEARBLEND);          // This monster determines how much the wave twists, sin8 function is for base twist, and stuff to the left is for how much twist per row 
    }          
  }
  huepusher(-5, z, 8); //huepush(uint8_t hueinc, uint8_t zvalue, uint8_t timer)

  EVERY_N_MILLIS(100){
    if(rowcount[z] < LEDper){
      rowcount[z]++;
    }  
   count[z]++; 
  }
}

void initialization(){
  //fader(deffade);
  palettetargeting(2);
  uint8_t z = fetcher(0);
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
  huepusher(1, z, 16);
}
void sinesides(){
  palettetargeting(8);
  CRGBPalette16 palette1 = gilt;
  CRGBPalette16 palette2 = currentPalette;
  uint8_t z = fetcher(19); 

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
  huepusher(1, z, 16);
}

void staticeye(){
  palettetargeting(5);
  uint8_t z = fetcher(20); 
  
  for (int col = 0; col < LEDstrips/2; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row <LEDper/2; row++){
      
      int standin = beatsin8(20, -1, 1);      // Mess with this to create eye layers and adjust how long it stays at 0, which is the static
      leds[XY(col, row)] = ColorFromPalette(currentPalette, hue[z] + (col*standin*row+row), 255, LINEARBLEND);      // Target palette, start hue
      leds[XY(col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[z] + (col*standin*row+row), 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, row)] = ColorFromPalette(currentPalette, hue[z] + (col*standin*row+row), 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[z] + (col*standin*row+row), 255, LINEARBLEND);
    }
  }  
  huepusher(-1, z, 16);
}

void canada(){
  for(int y = 0; y < LEDper; y++){            //read original values from array and add hue mod
    for(int x = 0; x < LEDstrips; x++){
      if(pgm_read_byte_near(&canadaray[y*LEDstrips+x]) == 0){            //white
        leds[XY(x, y)] = CRGB(255, 255, 255);
      } 
      else if(pgm_read_byte_near(&canadaray[y*LEDstrips+x]) == 1){      //red
        leds[XY(x, y)] = CRGB(255, 0, 0);
      } 
      else if(pgm_read_byte_near(&canadaray[y*LEDstrips+x]) == 2){     //pink       just set to red for now
        leds[XY(x, y)] = CRGB(255, 0, 0);
      } 
    }
  }
}

void textdisp(){
  uint8_t z = fetcher(22);
  blackout();
  
  matrix->setTextWrap(false);
  //matrix->setFont( &Roboto_Mono_Medium_24 ); // matrix->setTextSize(size); for defaults
  matrix->setRotation(0);  //change to 1
  matrix->setTextColor(timer);
  matrix->setTextSize(2);
  strcpy(BTmessage, "WONGLE"); 
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
void Rainbow() {
  palettetargeting(1);
  uint8_t z = fetcher(18);
  fill_rainbow( dest, NUM_LEDS, gHue, 7);
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
  {(pattern_func)DNA,           F("DNA")},
  {(pattern_func)Gargyle,       F("Gargyle")},
  {(pattern_func)Midearth,      F("Midearth")},
  {(pattern_func)Quadplexor,    F("Quadplexor")},
};

uint16_t const NUMpatterns = (sizeof(Pattern_List) / sizeof(Pattern_List[0]));

#endif