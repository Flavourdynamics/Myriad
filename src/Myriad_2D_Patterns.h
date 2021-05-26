#ifndef Myriad_2D_Patterns_h
#define Myriad_2D_Patterns_h
// See steps to add patterns in Testbed.h

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
      //leds[XY(x, y)] = ColorFromPalette( currentPalette, hueindex, 255 * arrayval /* Don't show 0s */, LINEARBLEND);
      //leds[XY(x, y)] = ColorFromPalette( currentPalette, pgm_read_byte_near(&startarray[y*LEDstrips+x])* 52 + hue[z] -(abs(centerpoint +0.5 -x) * horzgradient), 255 * pgm_read_byte_near(&startarray[y*LEDstrips+x]) /* Don't show 0s */, LINEARBLEND);
      
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

void rando(){
  //uint8_t z = fetcher(3);
  blackout();
  
  if(palnum == 0){
    for (int i = 0; i < 45 ; i++){  // runs 3 times - better boot pattern? (fade?)
      uint8_t targetx = random16(0, LEDstrips);
      uint8_t targety = random16(0, LEDper);
      leds[XY(targetx, targety)] = CHSV(random8(), random8(), random8(150, 255));
    }
  } else {
    palettetargeting(11);
    for (int i = 0; i < 45 ; i++){  // runs 3 times - better boot pattern? (fade?)
      uint8_t targetx = random16(0, LEDstrips);
      uint8_t targety = random16(0, LEDper);
      leds[XY(targetx, targety)] = ColorFromPalette(currentPalette, random8(), 255, LINEARBLEND);
      //leds[targetx][targety]set.Sat ) random8(155,255)
    }
  }
}
 
void illusion(){
  palettetargeting(4);
  uint8_t z = fetcher(4);
  
  for (int y = 0; y < LEDper/2; y++){
    for (int x = 0; x <LEDstrips/2; x++){
      leds[XY(x, y)] = ColorFromPalette(currentPalette, hue[z] + x*25 + y*25, 255, LINEARBLEND);                            //top left
      leds[XY(LEDstrips-1-x, y)] = ColorFromPalette(currentPalette, hue[z] + x*25 + y*25, 255, LINEARBLEND);              //top right
      leds[XY(x, LEDper-1-y)] = ColorFromPalette(currentPalette, hue[z] + x*25 + y*25, 255, LINEARBLEND);                 //bottom left
      leds[XY(LEDstrips-1-x, LEDper-1-y)] = ColorFromPalette(currentPalette, hue[z] + x*25 + y*25, 255, LINEARBLEND);   //bottom right 
    }
  }
  huepusher(1, z, 16);
}
 
void diagonal(){
  palettetargeting(2);
  uint8_t z = fetcher(5); 
  
  for (int y = 0; y < rowcount[z]; y++){
    for (int x = 0; x < colcount[z]; x++){
       leds[XY(x, y)] = ColorFromPalette( currentPalette, hue[z]+(x+y)*10, 255, LINEARBLEND); // normal palette access
    }
  }
  EVERY_N_MILLIS(100){
    if (colcount[z] < LEDstrips){
      colcount[z]++;
    }
    if (rowcount[z] < LEDper) {
      rowcount[z]++;
    }
  }
  huepusher(1, z, 16);
}
 
void scan(){
  palettetargeting(11);
  uint8_t z = fetcher(6);
  
  for (int y = 0; y < LEDper; y++){
    for (int x = 0; x < LEDstrips; x++){
      leds[XY(x, y)] = ColorFromPalette(currentPalette, hue[z], random8(100,150), LINEARBLEND); 
    } 
  }
  huepusher(1, z, 16);
}

void confetti(){
  palettetargeting(11);
  uint8_t z = fetcher(7); 
  //blackout();
  for(int x = 0; x < LEDstrips; x++){
    int y = random16(LEDper);
    leds[XY(x, y)] += ColorFromPalette(currentPalette, hue[z] + random8(64), 200, LINEARBLEND);
  }
  huepusher(1, z, 16);
}

void sinelon(){         // A romantic wave of 
  palettetargeting(11);
  uint8_t z = fetcher(8); 
  
  for(int bead = 1; bead < 10; bead++){
    int xpos = beatsin16( bead * 4, 0, LEDstrips - 1);
    int ypos = beatsin16( bead * 3, 0, LEDper - 1);
    leds[XY(xpos, ypos)] += ColorFromPalette(currentPalette, hue[z] * bead, 255, LINEARBLEND);
  }
  blur( leds, LEDstrips, LEDper, 128);
  huepusher(1, z, 16);
}

void vortex(){
  palettetargeting(11);
  uint8_t z = fetcher(9); 
  
  for(int y = 0; y < LEDper; y++){
    uint8_t pos = sin8(hue[z]*(y+1));   
    pos = map(pos, 1, 248, 0, LEDstrips-1);
    leds[XY(pos, y)] += ColorFromPalette(currentPalette, hue[z]+y/2, 255, LINEARBLEND);
  }
  huepusher(1, z, 16);
}

void tropic(){
  palettetargeting(8);
  uint8_t z = fetcher(10); 
  
  uint8_t beat = beatsin16( 14, 25, 455);
  for(int x = 0; x < LEDstrips; x++){
    for( int y = 0; y < LEDper; y++){
      leds[XY(x, y)] = ColorFromPalette(currentPalette, hue[z]+(y*10)+x*2, beat-hue[z]+(y*10)-x*2, LINEARBLEND);   //leds[x][y] = ColorFromPalette(currentPalette, hue+(y*10)+x*2, beat-hue+(y*10)-x*2, LINEARBLEND);
    }
  }
  huepusher(1, z, 16);
}

void juggle(){
  palettetargeting(11);
  uint8_t z = fetcher(11); 
  
  for(int x = 0; x < LEDstrips; x++){
    byte dothue = 0;
    for( int i = 0; i < 5; i++){
      leds[XY(x, beatsin16( i+7, 0, LEDper-1 ))] |= ColorFromPalette(currentPalette, hue[z] + dothue+x*3, 255, LINEARBLEND);
      dothue += 51;
    }
  }
  huepusher(1, z, 16);
}

void flex(){
  palettetargeting(11);
  uint8_t z = fetcher(12); 
  
  for (int col = 0; col < LEDstrips; col++){         // Write each row with start colour and a random saturation     
    int beatbox = beatsin8(20, 3, abs(14-col) + 15);
    for(int row = 0; row < LEDper; row++){
      leds[XY(col, row)] = ColorFromPalette(currentPalette, hue[z] + beatbox*row, 255, LINEARBLEND); // fill_rainbow(leds[x], number of leds, start hue, hue delta)  
      leds[XY(LEDstrips-1-col, row)] = ColorFromPalette(currentPalette, hue[z] + beatbox*row, 255, LINEARBLEND);  
      //ledmatrix.VerticalMirror();
      //leds[(LEDstrips*LEDper)-(col*LEDper+row)] = ColorFromPalette(currentPalette, hue + beatbox*row, 255, LINEARBLEND); // fill_rainbow(leds[x], number of leds, start hue, hue delta)         fill_rainbow( leds[col], rowcount/2, hue, beatbox); // fill_rainbow(leds[x], number of leds, start hue, hue delta)
    }
  }          
  EVERY_N_MILLIS(100){
    if(rowcount[z] < LEDper){
      rowcount[z] = rowcount[z] + 1;
    }
  }
  huepusher(1, z, 16);
}

void firetoucher(){
  palettetargeting(10);
  uint8_t z = fetcher(13); 
  
  for (int col = 0; col < LEDstrips/2; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row <LEDper/2; row++){
      int standin = beatsin8(60, 5, 10);      // BPM, MIN, MAX
      leds[XY(col, row)] = ColorFromPalette(currentPalette, hue[z] + standin * -col + row * 5, 255, LINEARBLEND);      // Target palette, start hue
      leds[XY(col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[z] + standin * -col + row * 5, 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, row)] = ColorFromPalette(currentPalette, hue[z] + standin * -col + row * 5, 255, LINEARBLEND);
      leds[XY((LEDstrips - 1 - col), LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[z] + standin * -col + row * 5, 255, LINEARBLEND);
    }
  } 
  huepusher(1, z, 24);
}

void starfall() {  // meteorRain(0xff,0xff,0xff,10, 64, true, 30);

  //huepusher(1, z, 16);
}

void dacirc(){
  palettetargeting(11);
  uint8_t z = fetcher(15); 
  
  for(int bead = 1; bead < 30; bead++){                                
    uint8_t xpos;
    uint8_t ypos;
    
    noise[bead][0] = inoise8(noise[bead][0], timer+bead);           // get some noise
    noise[bead][0] = constrain(noise[bead][0], 60, 195);
    xpos = map(noise[bead][0], 60, 195, 0, LEDstrips);        // Map it to width

    noise[bead][1] = inoise8(noise[bead][1], timer*2+random8()+bead); 
    noise[bead][1] = constrain(noise[bead][1], 60, 195); 
    ypos = map(noise[bead][1], 60, 195, 0, LEDper);
    
    ledmatrix.DrawFilledCircle(xpos, ypos, 2, ColorFromPalette(currentPalette, hue[z] + bead*1, 255*bead, LINEARBLEND));
  }
  huepusher(1, z, 24);
  timer+=1;
}

void diamondmaw(){
  palettetargeting(3);
  uint8_t z = fetcher(16); 
  
  for (int col = 0; col < LEDstrips/2; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row <LEDper/2; row++){
      
      int standin = beatsin8(14, -5, 25);      // BPM, MIN, MAX
      leds[XY(col, row)] = ColorFromPalette(currentPalette, hue[z] - (col+row)*standin, 255, LINEARBLEND);      // Target palette, start hue
      leds[XY(col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[z] - (col+row)*standin, 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, row)] = ColorFromPalette(currentPalette, hue[z] - (col+row)*standin, 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[z] - (col+row)*standin, 255, LINEARBLEND);
    }
  }
  huepusher(-1, z, 16);
}

void him(){
  palettetargeting(7);
  uint8_t z = fetcher(17); 
  
  for (int col = 0; col < LEDstrips/2; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row <LEDper/2; row++){   
      int standin = beatsin16(14, 50, 300);      // BPM, MIN, MAX
      leds[XY(col, row)] = ColorFromPalette(currentPalette, hue[z] + (col+row)*(400-standin)/250 + (col+row*standin*col)/350, 255, LINEARBLEND);      // Target palette, start hue
      leds[XY(col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[z] + (col+row)*(400-standin)/250 + (col+row*standin*col)/350, 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, row)] = ColorFromPalette(currentPalette, hue[z] + (col+row)*(400-standin)/250 + (col+row*standin*col)/350, 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[z] + (col+row)*(400-standin)/250 + (col+row*standin*col)/350, 255, LINEARBLEND);
    }
  }  
  huepusher(-1, z, 10);
}

void lilminfuk(){
  palettetargeting(12);
  uint8_t z = fetcher(18); 

  int standin = beatsin16(8, 75, 175);      // BPM, MIN, MAX
  for (int col = 0; col < LEDstrips/2; col++){         // Write each row with start colour and a random saturation    
    for(int row = 0; row <LEDper/2; row++){      
      leds[XY(col, row)] = ColorFromPalette(currentPalette, hue[z] + (col+row)*(250-standin)/5 + (col+row*standin*col)/250, 255, LINEARBLEND);      // Target palette, start hue
      leds[XY(col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[z] + (col+row)*(250-standin)/5 + (col+row*standin*col)/250, 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, row)] = ColorFromPalette(currentPalette, hue[z] + (col+row)*(250-standin)/5 + (col+row*standin*col)/250, 255, LINEARBLEND);
      leds[XY(LEDstrips - 1 - col, LEDper - 1 - row)] = ColorFromPalette(currentPalette, hue[z] + (col+row)*(250-standin)/5 + (col+row*standin*col)/250, 255, LINEARBLEND);
    }
  }  
  huepusher(-1, z, 16);
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
/////////////////////////////////////////////////////////////////////////////////////////////////
SimplePatternList PATTERNlist = {initialization, visualizer, waterfall, rando, illusion,
  diagonal, scan, confetti, sinelon, vortex, tropic, juggle, flex, firetoucher, starfall,
  dacirc, diamondmaw, him, lilminfuk, sinesides, staticeye, canada, textdisp
  };    // PATTERNlist[PATTERNnum]();
  
#endif
