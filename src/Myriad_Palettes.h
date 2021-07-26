#ifndef Myriad_Palettes_h
#define Myriad_Palettes_h
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
#define PALETTE_NAME(x) NamedPalette(x, #x)

struct NamedPalette {
  const TProgmemRGBGradientPalettePtr Palette;
  const String Name;

  NamedPalette(TProgmemRGBGradientPalettePtr p, const String n)
    : Palette(p), Name(n) {}
};

DEFINE_GRADIENT_PALETTE( Border_Rainbow ){
    0,  0,  0,  0,
  115,  255,  0,  0,
  185,  0, 255,  0,
  255,  0, 0, 255 };
  
DEFINE_GRADIENT_PALETTE( Purple_Cascade ){ //GRB
    0,  0, 0, 0,   //black
  120,  155, 0, 255,   //purple
  235,  255, 0, 40,   //crimson
  255,  255, 255, 255 }; //full white
 
DEFINE_GRADIENT_PALETTE( Blackwhite ){
    0,    0, 0, 0,
  127,    0, 0, 0,
  128,  255, 255, 255,
  255,  255, 255, 255};

DEFINE_GRADIENT_PALETTE( Newspaper ){
    0,    0, 0, 0,
  127,    0, 0, 0,
  128,  255, 255, 255,
  255,  255, 0, 0};

DEFINE_GRADIENT_PALETTE( Sunburst ){
    0,    0, 0, 0,
  120,  255, 255, 0,
  235,  255, 255, 255,
  255,  255, 255, 255};

DEFINE_GRADIENT_PALETTE( Gilt ){
    0,    0, 0, 0,
  128, 96, 0, 140,
  150, 255, 100, 0,
  235,  255, 200, 0,
  255,  255, 255, 255};

DEFINE_GRADIENT_PALETTE( Tropicana ){
    0,  0, 0, 0,
   71,  66, 245, 230,
  145,  255, 255, 255,
  255,  245, 66, 203};
/*
DEFINE_GRADIENT_PALETTE( Startup ){ //RYGCB       borderrainbow, purplecascade, blackwhite, newspaper, unburst
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
*/

DEFINE_GRADIENT_PALETTE( Startup ){ //RYGCB       borderrainbow, purplecascade, blackwhite, newspaper, unburst
  0,  255,  0,    0,        //red
  65,  255,  255,  0,       //yellow 
  125,  0,    255,  0,      //green
  175,  0,    255,  255,    //cyan
  235,  0,    0,    255,
  255,  255,    0,    0}; 


DEFINE_GRADIENT_PALETTE( Redyell ){
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

DEFINE_GRADIENT_PALETTE( Pit ) {
  0,     3,   3,   3,
  64,   13,   13, 255,  //blue
  128,   3,   3,   3,
  192, 255, 130,   3 ,  //orange
  255,   3,   3,   3
};

DEFINE_GRADIENT_PALETTE( RainbowStripeColors ){
    0,   0,   0,   0,
   32,  255,   0,   0,
   73,   0,   0,   0,
  109,   0, 255,   0,
  146,   0,   0,   0,
  182,   0, 130,   0,
  219,   0,   0,   0,
  255,   0,   0, 255
};

DEFINE_GRADIENT_PALETTE( RainbowColors ){
    0, 255,   0,   0,
   85,   0, 255,   0,
  128,   0,   0, 255,
};

DEFINE_GRADIENT_PALETTE(Pastel){
  0,    255, 213, 50,  //yellow
  20,   90, 120, 255,  //blue
  39,   70, 190, 120,  //moss
  59,   137, 170, 175, //battleship
  78,   121, 238, 255, //sky
  98,   255, 80, 80,   //red orange
  118,  255, 142, 65,  //tangerine
  137,  228, 73, 255,  //pink
  157,  155, 30, 255,  //purple
  177,  255, 105, 160, //salmon
  196,  40, 40, 255,   //royal blue
  216,  20, 220, 255,  //teal
  235,  167, 244, 255, //pale blue
  255,  50, 205, 255   //bluer teal
};

NamedPalette Palette_List[] = {
  PALETTE_NAME(Border_Rainbow),
  PALETTE_NAME(Warm_Forest),          // cool foresty teal                        
  PALETTE_NAME(Popsicle),             // good, add orange
  PALETTE_NAME(Arizona_Trip),
  PALETTE_NAME(Pit),
  PALETTE_NAME(Redyell),
  PALETTE_NAME(Startup),
  PALETTE_NAME(Tropicana),
  PALETTE_NAME(Gilt),
  PALETTE_NAME(Sunburst),
  PALETTE_NAME(Newspaper),
  PALETTE_NAME(Purple_Cascade),
  PALETTE_NAME(Blackwhite),
  PALETTE_NAME(RainbowStripeColors),
  PALETTE_NAME(RainbowColors),
  PALETTE_NAME(Pastel),
};

// How many palettes are defined
const int NUMpalettes = sizeof(Palette_List)/sizeof(Palette_List[0]);
#endif