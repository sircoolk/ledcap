/*
LED VU meter for Arduino and Adafruit NeoPixel LEDs.
 
 Hardware requirements:
 - Most Arduino or Arduino-compatible boards (ATmega 328P or better).
 - Adafruit Electret Microphone Amplifier (ID: 1063)
 - Adafruit Flora RGB Smart Pixels (ID: 1260)
 OR
 - Adafruit NeoPixel Digital LED strip (ID: 1138)
 - Optional: battery for portable use (else power through USB or adapter)
 Software requirements:
 - Adafruit NeoPixel library
 
 Connections:
 - 3.3V to mic amp +
 - GND to mic amp -
 - Analog pin to microphone output (configurable below)
 - Digital pin to LED data input (configurable below)
 See notes in setup() regarding 5V vs. 3.3V boards - there may be an
 extra connection to make and one line of code to enable or disable.
 
 Written by Adafruit Industries.  Distributed under the BSD license.
 This paragraph must be included in any redistribution.
 
 fscale function:
 Floating Point Autoscale Function V0.1
 Written by Paul Badger 2007
 Modified from code by Greg Shakar
 
 */

#include <Adafruit_NeoPixel.h>
#include <math.h>

#define N_PIXELS  14 // Number of pixels in strand
#define MIC_PIN   A0  // Microphone is attached to this analog pin
#define LED_PIN1    6  // NeoPixel LED strand is connected to this pin
#define LED_PIN2    9
#define SAMPLE_WINDOW   5  // Sample window for average level
#define MODE_WINDOWS 60000
#define PEAK_HANG 25 //Time of pause before peak dot falls
#define PEAK_FALL 7 //Rate of falling peak dot
#define INPUT_FLOOR 20 //Lower range of analogRead input
#define INPUT_CEILING 800 //Max range of analogRead input, the lower the value the more sensitive (1023 = max)



byte peak = 14;      // Peak level of column; used for falling dots
unsigned int sample;
unsigned int col;
unsigned mode;
unsigned long startMode;
int j =1;
unsigned int jrainbow=1;

byte dotCount = 0;  //Frame counter for peak dot
byte dotHangCount = 0; //Frame counter for holding peak dot

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(N_PIXELS, LED_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(N_PIXELS, LED_PIN2, NEO_GRB + NEO_KHZ800);

void setup() 
{
  // This is only needed on 5V Arduinos (Uno, Leonardo, etc.).
  // Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this
  // line.  Audio samples are 'cleaner' at 3.3V.
  // COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
  //  analogReference(EXTERNAL);
  col=0;
  mode = 0;
  startMode=millis();
  // Serial.begin(9600);
  strip1.begin();
  strip2.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.show();
}

void loop() 
{
  unsigned long startMillis= millis();  // Start of sample window
  
 
  float peakToPeak = 0;   // peak-to-peak level
  
  unsigned int signalMax = 0;
  unsigned int signalMin = 1023;
  unsigned int c, y;
  
  
 
 if(millis()-startMode>MODE_WINDOWS)
 {
   mode++;
   if (mode>=3)
     mode =0;
    startMode=millis();
   }


  // collect data for length of sample window (in mS)
  while (millis() - startMillis < SAMPLE_WINDOW)
  {
    sample = analogRead(MIC_PIN);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
 
  // Serial.println(peakToPeak);


  //Fill the strip with rainbow gradient
  for (int i=0;i<=(strip1.numPixels()/2)-1;i++){
    setPixelColorSym(i,Wheel(col),30,150);
  }
  col++;
  if(col>255){col=0;}
  //Scale the input logarithmically instead of linearly
  c = fscale(INPUT_FLOOR, INPUT_CEILING, strip1.numPixels()/2, 0, peakToPeak, 2);

  


  if(c < peak) {
    peak = c;        // Keep dot on top
    dotHangCount = 0;    // make the dot hang before falling
  }
  
  /*if (jrainbow>10||jrainbow<=0){j=-j;}
  jrainbow=jrainbow+j;*/
  if (c <= strip1.numPixels()/2) { // Fill partial column with off pixels
   drawLine(strip1.numPixels()/2, (strip1.numPixels()/2)-c,0,0,0);//Wheel(jrainbow));
  }
  
  
  

  // Set the peak dot to match the rainbow gradient
  y = (strip1.numPixels()/2) - peak;
  
  setPixelColorSym(y - 1, Wheel(map(y, 0, (strip1.numPixels() / 2) - 1, 30, 150)));

  strip1.show();
  strip2.show();

  // Frame based peak dot animation
  if(dotHangCount > PEAK_HANG) { //Peak pause length
    if(++dotCount >= PEAK_FALL) { //Fall rate 
      peak++;
      dotCount = 0;
    }
  } 
  else {
    dotHangCount++; 
  }
}

//Used to draw a line between two points of a given color
void drawLine(uint8_t from, uint8_t to, uint32_t c1,uint32_t c2,uint32_t c3) {
  uint8_t fromTemp;
  if (from > to) {
    fromTemp = from;
    from = to;
    to = fromTemp;
  }
  for(int i=from; i<=to; i++){
    setPixelColorSym(i, c1,c2,c3);
  }
}

void drawLine(uint8_t from, uint8_t to, uint32_t c) {
  uint8_t fromTemp;
  if (from > to) {
    fromTemp = from;
    from = to;
    to = fromTemp;
  }
  for(int i=from; i<=to; i++){
    setPixelColorSym(i, c);
  }
}


float fscale( float originalMin, float originalMax, float newBegin, float
newEnd, float inputValue, float curve){

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;


  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output 
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  /*
   Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution  
   Serial.println(); 
   */

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin){ 
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd; 
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine 
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0){
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else     // invert the ranges
  {   
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange); 
  }

  return rangedValue;
}

//Div the strip symetric


void setPixelColorSym(int i,unsigned int c)
{
	if (mode == 0){
		strip1.setPixelColor(i, c);
		strip2.setPixelColor((strip1.numPixels() / 2) - 1 - i, c);
		strip1.setPixelColor((strip1.numPixels() - i - 1), c);
		strip2.setPixelColor((strip1.numPixels() / 2) + i, c);
	}
	else if (mode == 1)
	{
		strip1.setPixelColor(i, c);
		strip2.setPixelColor(i, c);	
		strip1.setPixelColor((strip1.numPixels() - i - 1), c);
		strip2.setPixelColor((strip1.numPixels() - i - 1), c);
	}
	else if (mode == 2)
	{
		strip1.setPixelColor((strip1.numPixels() / 2) - 1 - i, c);
		strip2.setPixelColor((strip1.numPixels() / 2) - 1 - i, c);
		strip1.setPixelColor((strip1.numPixels() / 2) + i, c);
		strip2.setPixelColor((strip1.numPixels() / 2) + i, c);
	}
}
void setPixelColorSym(int i,unsigned int c1,unsigned int c2,unsigned int c3)
{
	if (mode == 0)
	{
		strip1.setPixelColor(i, c1, c2, c3);
		strip2.setPixelColor((strip1.numPixels() / 2) - 1 - i, c1, c2, c3);
		strip1.setPixelColor((strip1.numPixels() - i - 1), c1, c2, c3);
		strip2.setPixelColor((strip1.numPixels() / 2) + i, c1, c2, c3);
	}
	else if (mode == 1)
	{
		strip1.setPixelColor(i, c1, c2, c3);
		strip2.setPixelColor(i, c1, c2, c3);
		strip1.setPixelColor((strip1.numPixels() - i - 1), c1, c2, c3);
		strip2.setPixelColor((strip1.numPixels() - i - 1), c1, c2, c3);
	}
	else if (mode == 2)
	{
		strip1.setPixelColor((strip1.numPixels() / 2) - 1 - i, c1, c2, c3);
		strip2.setPixelColor((strip1.numPixels() / 2) - 1 - i, c1, c2, c3);
		strip1.setPixelColor((strip1.numPixels() / 2) + i, c1, c2, c3);
		strip2.setPixelColor((strip1.numPixels() / 2) + i, c1, c2, c3);
	}
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

