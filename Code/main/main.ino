#include <Adafruit_NeoPixel.h>
#include <math.h>

#define N_PIXELS  14 // Number of pixels in strand
#define MIC_PIN   A0  // Microphone is attached to this analog pin
#define LED_PIN1    6  // NeoPixel LED strand is connected to this pin
#define LED_PIN2    9
#define MODE_WINDOWS 60000

unsigned mode;
unsigned long startMode;


Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(N_PIXELS, LED_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(N_PIXELS, LED_PIN2, NEO_GRB + NEO_KHZ800);

void setup() 
{

  mode = 0;
  startMode=millis();
  
  // Serial.begin(9600);
  strip1.begin();
  strip2.begin();
  
  // Initialize all pixels to 'off'
  strip1.show(); 
  strip2.show();
}

void loop() 
{
 
  cyclon(5,randomColor(0,255),1);
 
 
}


// Sprites

void cyclon(int wait,long color,int n)
{
  for (int i = 0 ; i<n;i++)
  {
  for(int i=0; i<strip1.numPixels(); i++) {
    strip1.setPixelColor(i, color);
    //strip1.setPixelColor(32-i, strip1.Color(255, 0, 0));
    strip1.show();
    delay(wait);
    strip1.setPixelColor(i-5, strip1.Color(0,0,0));
    //strip1.setPixelColor(32-i, strip1.Color(0,0,0));
    strip1.show();
    delay(wait);   
  }
    for(int i=0; i<strip1.numPixels(); i++) {
    //strip1.setPixelColor(i, strip1.Color(255, 0, 0));
    strip1.setPixelColor(32-i, color);
    strip1.show();
    delay(wait);
    //strip1.setPixelColor(i, strip1.Color(0,0,0));
    strip1.setPixelColor(37-i, strip1.Color(0,0,0));
    strip1.show();
    delay(wait);       

}
  }
}
  
  uint32_t randomColor(int a,int b)
{
  int c1 = random(a,b);
  int c2 = random(a,b);
  int c3 = random(a,b);
  return strip1.Color(c1,c2,c3);
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
