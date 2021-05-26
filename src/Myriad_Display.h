#ifndef Myriad_Display_h
#define Myriad_Display_h

#include "heltec.h"

extern uint16_t EQscaled[EQbins];

void OLEDbarz(){
  int barwideness = DISPLAY_WIDTH/14;
  for(int i = 0; i < EQbins; i++){
    int barhightness = map(EQscaled[i], 0, LEDper, 0, DISPLAY_HEIGHT); 
    Heltec.display->fillRect(i*barwideness, DISPLAY_HEIGHT-barhightness, barwideness-1, barhightness); // void fillRect(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height);  
  }
  //Heltec.display->display();
}

void OLEDstartcircle(){
  Heltec.display->clear();
  for (int z = 0; z < DISPLAY_WIDTH; z++){
    for (int16_t i=1; i < 15; i+=1) {
      Heltec.display->drawCircle(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, z/i);
      Heltec.display->drawCircle(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, DISPLAY_WIDTH-z/i);
    }
    Heltec.display->clear();
    Heltec.display->display();        
  }
  for (int z = DISPLAY_WIDTH; z > 0; z--){
    for (int16_t i=1; i < 15; i+=1) {
      Heltec.display->drawCircle(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, z/i);
      Heltec.display->drawCircle(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, DISPLAY_WIDTH-z/i);
    }
    Heltec.display->clear();
    Heltec.display->display();       
    
  }
  Heltec.display->clear();
  Heltec.display->display();
}

void OLEDperiodic(){
  Heltec.display->clear();
  Heltec.display->setLogBuffer(5, 30);
  Heltec.display->print("FPS: ");
  Heltec.display->print(LEDS.getFPS());
  Heltec.display->print(" PWR: ");
  Heltec.display->println(calculate_unscaled_power_mW(leds, LEDtotal)/5/100);
  Heltec.display->drawLogBuffer(0, 0);
  Heltec.display->display();
}

void OLEDsetup(){
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  Heltec.display->setContrast(255);
  //Heltec.display->setLogBuffer(5, 30);
   //OLEDstartcircle();
}

#endif