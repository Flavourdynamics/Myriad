#ifndef Myriad_Palettes_h
#define Myriad_Palettes_h

///////////////////////////Palettes////////////////////////////////////
/*
const TProgmemRGBPalette16 borderrainbow FL_PROGMEM = {
    0,   0,  0,  0,
  115,   255,  0,  0,
  185,   0, 255,  0,
  255,   0,0,255 };
  
const TProgmemRGBPalette16 purplecascade FL_PROGMEM = {
    0,   0,  0,  0,     //black
  120,   155,  0,  255,   //purple
  235,   255,  0,  40,    //crimson
  255,   255, 255, 255 }; //full white
 
const TProgmemRGBPalette16 blackwhite FL_PROGMEM = {
    0,  0, 0, 0,
  127,  0, 0, 0,
  128,  255, 255, 255,
  255,  255, 255, 255};

const TProgmemRGBPalette16 newspaper FL_PROGMEM = {
    0,  0, 0, 0,
  127,  0, 0, 0,
  128,  255, 255, 255,
  255,  255, 0, 0};

const TProgmemRGBPalette16 sunburst FL_PROGMEM = {
    0,  0, 0, 0,
  120,  255, 255, 0,
  235,  255, 255, 255,
  255,  255, 255, 255};

const TProgmemRGBPalette32 gilt FL_PROGMEM = {
    0,  0, 0, 0,
  128,  96, 0, 140,
  150,  255, 100, 0,
  235,  255, 200, 0,
  255,  255, 255, 255};

const TProgmemRGBPalette16 tropicana FL_PROGMEM = {
  0,    0, 0, 0,
  71,   66, 245, 230,
  145,  255, 255, 255,
  255,  245, 66, 203};

const TProgmemRGBPalette32 startup FL_PROGMEM = {
  0,   255,  0,  0,      //red
  51,   255,  0,  0,
  52,   255,  255,  0,    //yellow
  102,  255,  255,  0,     
  103,  0,  255,  0,      //green
  153,  0,  255,  0,
  154,  0,  255,  255,    //cyan
  205,  0,  255,  255,
  206,  0,  0,  255,      //blue
  255,  0,  0,  255};

const TProgmemRGBPalette16 redyell FL_PROGMEM = {
    0,    0, 0, 0,
    120,    240,0,0,
    235,    255,  200,  0,
    255,    255,255,255};
*/
/*
RainbowColors_p;
RainbowStripeColors_p;
CloudColors_p;
PartyColors_p;
LavaColors_p;
OceanColors_p;
ForestColors_p;
HeatColors_p;
*/



DEFINE_GRADIENT_PALETTE( borderrainbow ){
    0,  0,  0,  0,
  115,  255,  0,  0,
  185,  0, 255,  0,
  255,  0, 0, 255 };
  
DEFINE_GRADIENT_PALETTE( purplecascade ){ //GRB
    0,  0, 0, 0,   //black
  120,  155, 0, 255,   //purple
  235,  255, 0, 40,   //crimson
  255,  255, 255, 255 }; //full white
 
DEFINE_GRADIENT_PALETTE( blackwhite ){
    0,    0, 0, 0,
  127,    0, 0, 0,
  128,  255, 255, 255,
  255,  255, 255, 255};

DEFINE_GRADIENT_PALETTE( newspaper ){
    0,    0, 0, 0,
  127,    0, 0, 0,
  128,  255, 255, 255,
  255,  255, 0, 0};

DEFINE_GRADIENT_PALETTE( sunburst ){
    0,    0, 0, 0,
  120,  255, 255, 0,
  235,  255, 255, 255,
  255,  255, 255, 255};

DEFINE_GRADIENT_PALETTE( gilt ){
    0,    0, 0, 0,
  128, 96, 0, 140,
  150, 255, 100, 0,
  235,  255, 200, 0,
  255,  255, 255, 255};

DEFINE_GRADIENT_PALETTE( tropicana ){
    0,  0, 0, 0,
   71,  66, 245, 230,
  145,  255, 255, 255,
  255,  245, 66, 203};

DEFINE_GRADIENT_PALETTE( startup ){ //RYGCB       borderrainbow, purplecascade, blackwhite, newspaper, unburst
    0,  255,  0,  0,      //red
   45,  255,  0,  0,
   52,  255,  255,  0,    //yellow
  98,  255,  255,  0,     
  103,  0,  255,  0,      //green
  149,  0,  255,  0,
  154,  0,  255,  255,    //cyan
  200,  0,  255,  255,
  206,  0,  0,  255,      //blue
  245,  0,  0,  255,
  255,  255,  0,  0};   // back to red?

/*
DEFINE_GRADIENT_PALETTE( startup ){ //RYGCB       borderrainbow, purplecascade, blackwhite, newspaper, unburst
    0,  255,  0,    0,      //red
   65,  255,  255,  0,    //yellow 
  125,  0,    255,  0,      //green
  175,  0,    255,  255,    //cyan
  235,  0,    0,    255,
  255,  255,    0,    0}; 
  */

DEFINE_GRADIENT_PALETTE( redyell ){
    0,    0, 0, 0,
    120,  240, 0, 0,
    235,  255, 200, 0,
    255,  255, 255, 255};

DEFINE_GRADIENT_PALETTE( Warm_Forest ){
  0, 1, 5, 0,
  19, 32, 23, 1,
  38, 161, 55, 1,
  63, 229, 144, 1,
  66, 39, 142, 74,
  255, 1, 4, 1};

DEFINE_GRADIENT_PALETTE( Popsicle ){
  0, 19, 2, 39,
  25, 26, 4, 45,
  51, 33, 6, 52,
  76, 68, 62, 125,
  102, 118, 187, 240,
  109, 163, 215, 247,
  114, 217, 244, 255,
  122, 159, 149, 221,
  149, 113, 78, 188,
  183, 128, 57, 155,
  255, 146, 40, 123};

DEFINE_GRADIENT_PALETTE( Arizona_Trip ){
  0, 0, 0, 255,
  63, 0, 55, 255,
  127, 0, 255, 255,
  191, 42, 255, 45,
  255, 255, 255, 0};

DEFINE_GRADIENT_PALETTE( pit ) {
  0,     3,   3,   3,
  64,   13,   13, 255,  //blue
  128,   3,   3,   3,
  192, 255, 130,   3 ,  //orange
  255,   3,   3,   3
};  

const TProgmemRGBGradientPalettePtr Palette_List[] = {
  Warm_Forest,                            // cool foresty teal                        
  Popsicle,             /// good, add orange
  Arizona_Trip
  };

// Count of how many cpt-city gradients are defined:
const uint8_t Palette_Count = sizeof(Palette_List) / sizeof(TProgmemRGBGradientPalettePtr);
#endif
