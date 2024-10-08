
#include <FastLED.h>

FASTLED_USING_NAMESPACE

// Modified DemoReel for Music Sync
// Waveform visualization used with BPM analog input


#define DATA_PIN    3
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#define NUM_LEDS    200
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          200
#define FRAMES_PER_SECOND  120
// INPUT: Potentiometer should be connected to 5V and GND
int potPin = A0; // Potentiometer output connected to analog pin 3
int potVal = 0; // Variable to store the input from the potentiometer

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon2, juggle, bpm, test};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  //potVal = analogRead(potPin);
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[3]();//0rainbow, 1rainbowWithGlitter, 2confetti, 3sinelon, 4juggle, 5bpm, 6test

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS(100) { 
    gHue++; 
  } // slowly cycle the "base color" through the rainbow //20
  EVERY_N_MILLISECONDS( 2000 ) { 
    potVal = analogRead(potPin);
  }
  //EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
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
  //uint8_t BeatsPerMinute = (map(potVal, 0, 1023, 60, 160)/2)*2;//60-160-240
  uint8_t BeatsPerMinute = 118;//original 118
  //int brightness = potVal;
  fadeToBlackBy( leds, NUM_LEDS, 100);
  //uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  int pos = beatsin16( BeatsPerMinute/4.0, 0, 9 );//(NUM_LEDS/2)-1 
  int beat = beatsin16( BeatsPerMinute/4.0, 0, 1);
  for(int i = 0; i<NUM_LEDS; i++){
    int intensity = 10;
    if(i<NUM_LEDS/2){
      leds[i] = CHSV(255-gHue,200,beat*intensity);
    }else{
      leds[i] = CHSV(255-gHue,200,intensity-(beat*intensity));
    }
  }
  for(int i = 0; i <10;i++){
  leds[(pos*10)+i] = CHSV( gHue, 255, 252);
  leds[NUM_LEDS-1-(pos*10)-i] = CHSV( gHue, 255, 252);
  }
}
int ledNum (int x, int y){
  int num = 0;
  if(x<10){
    num+=y*10;
    if(y%2==0){
      num+=9-x;
    }else{
      num+=x;
    }
  }else{
    int tempx = x-10;
    num+=199-(y*10);
    if(y%2==0){
      num-=tempx;
    }else{
      num-=9-tempx;
    }
  }
  return num;
}
void sinelon2()
{
  // a colored dot sweeping back and forth, with fading trails
  //uint8_t BeatsPerMinute = (map(potVal, 0, 1023, 60, 160)/2)*2;//6-240
  uint8_t BeatsPerMinute = 118/2;//6-240
  //int brightness = potVal;
  fadeToBlackBy( leds, NUM_LEDS, 30);//75
  //uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  int pos = beatsin16( BeatsPerMinute/4.0, 0, 9 );//(NUM_LEDS/2)-1 
  int beat = beatsin16( BeatsPerMinute/2.0, 0, 1);
  for(int i = 0; i<NUM_LEDS; i++){
    int intensity = 50;
    if(i<NUM_LEDS/2){
      leds[i] = CHSV(255-gHue,200,beat*intensity);
    }else{
      leds[i] = CHSV(255-gHue,200,intensity-(beat*intensity));
    }
  }
  for(int i = 0; i <20;i++){
  pos = beatsin16( BeatsPerMinute/4.0, 0, 9,i*(100));
  leds[ledNum(i,pos)] = CHSV( gHue, 255, 252);
  }
}

void test(){
  // a colored dot sweeping back and forth, with fading trails
  //uint8_t BeatsPerMinute = (map(potVal, 0, 1023, 60, 160)/2)*2;//6-240
  uint8_t BeatsPerMinute = 118;//6-240
  //int brightness = potVal;
  fadeToBlackBy( leds, NUM_LEDS, 100);
  //uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  int pos = beatsin16( BeatsPerMinute/4.0, 0, 9 );//(NUM_LEDS/2)-1 
  int beat = beatsin16( BeatsPerMinute, 0, 1);
  leds[ledNum(10,pos)] = CHSV( gHue, 255, 252);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 80;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

