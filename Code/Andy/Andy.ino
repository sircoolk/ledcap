#include <Adafruit_NeoPixel.h>
#include <math.h>

#define N_PIXELS 60
// Number of pixels in strand
#define LED_PIN1    8  // NeoPixel LED strand is connected to this pin
#define LED_PIN2    9
#define MODE_WINDOWS 10000
#define INPUT_FLOOR 10 //Lower range of analogRead input
#define INPUT_CEILING 200 //Max range of analogRead input, the lower the value the more sensitive (1023 = max)
#define PEAK_HANG 25 //Time of pause before peak dot falls
#define PEAK_FALL 7 //Rate of falling peak dot

int ctrlReset    = 5;                   // Digital pin 5 = signal to reset MSGEQ7s
int ctrlStrobe   = 4;                   // Digital pin 4 = signal to strobe (read data from) MSGEQ7s
int channel  =  1;                         // Analog pin 0 = spectrum data from left channel
int spectrum[7];                     // Array to store 7 bands of spectrum data from channel 

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(N_PIXELS, LED_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(N_PIXELS, LED_PIN2, NEO_GRB + NEO_KHZ800);

long backColor[28];
unsigned mode;
unsigned long startMode;
unsigned long startEffect;
byte peak = 14;      // Peak level of column; used for falling dots
unsigned int sample;
uint16_t jRainbow,brightness;
int s,col;
byte dotCount = 0;  //Frame counter for peak dot
byte dotHangCount = 0; //Frame counter for holding peak dot
int wait;


void setup() 
{

  
  mode = 0;
  col=0;
  jRainbow=0;
  brightness=0;
  startMode=millis();
  startEffect=0;
  wait =0;
  
  Serial.begin(9600);
  strip1.begin();
  strip2.begin();

  pinMode(ctrlReset,OUTPUT);             // Define reset as output
  pinMode(ctrlStrobe,OUTPUT);            // Define strobe as output
  digitalWrite(ctrlReset,LOW);           // Pull the reset signal low
  digitalWrite(ctrlStrobe,HIGH);         // Drive the strobe signal high

  
  // Initialize all pixels to 'off'
  show_all_strip();
  
  s=1;

  unsigned int signalMax = 0;
  unsigned int signalMin = 1023;
  unsigned int c, y;
  
  // Init backColor
  for(int i =0; i<28;i++)
  {
     backColor[i]=randomColor(0,10);
    }
}

void loop() 
{
  readMSGEQ7s();
  Serial.print(spectrum[0]);
  Serial.print(spectrum[1]);
  Serial.print(spectrum[2]);
  Serial.print(spectrum[3]);
  Serial.print(spectrum[4]);
  Serial.print(spectrum[5]);
  Serial.println(spectrum[6]);
  
    sample = spectrum[0];
     
 
  if(millis()-startMode>MODE_WINDOWS)
 {
   mode++;
   if (mode>=5)
     mode =0;
   startMode=millis();
  }
  
  
  
  if(mode<=4)
  {
    
    if(sample>65){
     
     if(mode==0)
       cyclonR(wait,2);
     if(mode==1)
       FandC(30,255); 
     if(mode==2)
       cyclon(wait, Wheel(((random(0,strip1.numPixels()) * 256 / strip1.numPixels()) + jRainbow) & 255),2);
     if(mode==3)
     cyclon2(wait, Wheel(((random(0,strip1.numPixels()) * 256 / strip1.numPixels()) + jRainbow) & 255),2);
     if(mode==4)
      rainbowFlash(50,255);
     startEffect=millis();
     brightness=0;
     }
    else if(millis()-startEffect > 1500) 
     { 
       rainbowCycle(100,brightness); 
       if(brightness<100) 
         brightness++; 
       //colorBride(50,25); //  FlashSpeed, brightnessFlash 
  }
  }
 }
 



// Sprites

void cyclon(int wait,long color,int n)
{
   set_all_brightness(255);
  for(int i = 0; i<strip1.numPixels();i++)
  {
    set_all_strip(i,0,0,0);
    }
  if(s==1){
  for(int i=0; i<strip1.numPixels()+n; i++) 
    {
    set_all_strip(i, color);
    show_all_strip();
    delay(wait);
    set_all_strip(i-n, strip1.Color(0,0,0));
    show_all_strip();
    delay(wait);   
    }
    s=0;
   }
   else if(s==0)
   {
    for(int i=0; i<strip1.numPixels()+n; i++) 
    {
    set_all_strip(13-i, color);
    show_all_strip();
    delay(wait);
    set_all_strip(13+n-i, strip1.Color(0,0,0));
    show_all_strip();
    delay(wait);
    }
    s=1;
  }
  
  }
  void cyclon2(int wait,long color,int n)
{
  set_all_brightness(255);
  for(int i = 0; i<strip1.numPixels();i++)
  {
    set_all_strip(i,0,0,0);
    }
  if(s==1){
  for(int i=0; i<strip1.numPixels()+n; i++) 
    {
    strip1.setPixelColor(i,color);
    strip2.setPixelColor(strip2.numPixels()-i,color);
    show_all_strip();
    delay(wait);
    strip1.setPixelColor(i-n,strip1.Color(0,0,0));
    strip2.setPixelColor(strip2.numPixels()-i+n,strip2.Color(0,0,0));
    show_all_strip();
    delay(wait);   
    }
    s=0;
   }
   else if(s==0)
   {
    for(int i=0; i<strip1.numPixels()+n; i++) 
    {
    strip2.setPixelColor(i,color);
    strip1.setPixelColor(strip2.numPixels()-i,color);
    show_all_strip();
    delay(wait);
    strip2.setPixelColor(i-n,strip1.Color(0,0,0));
    strip1.setPixelColor(strip2.numPixels()-i+n,strip2.Color(0,0,0));
    show_all_strip();
    delay(wait);
    }
    s=1;
  }
  
  }
  void cyclonR(int wait,int n)
{
  set_all_brightness(255);
  for(int i = 0; i<strip1.numPixels();i++)
  {
    set_all_strip(i,0,0,0);
    }
  if(s==1){
  for(int i=0; i<strip1.numPixels()+n; i++) 
    {
    set_all_strip(i, Wheel(((i * 256 / strip1.numPixels()) + jRainbow) & 255));
    show_all_strip();
    delay(wait);
    set_all_strip(i-n, strip1.Color(0,0,0));
    show_all_strip();
    delay(wait);   
    }
    s=0;
   }
   else if(s==0)
   {
    for(int i=0; i<strip1.numPixels()+n; i++) 
    {
    set_all_strip(strip1.numPixels()-i, Wheel(((i * 256 / strip1.numPixels()) + jRainbow) & 255));
    show_all_strip();
    delay(wait);
    set_all_strip(13+n-i, strip1.Color(0,0,0));
    show_all_strip();
    delay(wait);
    }
    s=1;
  }
  
  }
  
  void FandC(long time, int brightnessFlash)
{
  //init
  long color[strip1.numPixels()];
  
  for(int i = 0; i<strip1.numPixels();i++)
  {
    set_all_strip(i,Wheel(((random(0,strip1.numPixels()) * 256 / strip1.numPixels()) + jRainbow) & 255));
    color[i]= strip1.getPixelColor(i);
  }
  show_all_strip();
  // flash
   for(int i = 0; i<strip1.numPixels();i++)
  {
    set_all_strip(i,brightnessFlash,brightnessFlash,brightnessFlash);
  }
  show_all_strip();
  for( int i = 0; i<strip1.numPixels();i++)
  {
    set_all_strip(i,color[i]);
   
  }
  delay(time);
  show_all_strip();
  
}
void rainbowFlash(long time,int brightnessFlash)
{
 uint16_t i;
 uint32_t col;
 col = Wheel(((random(0,strip1.numPixels()) * 256 / strip1.numPixels()) + jRainbow) & 255);
  for(i=0; i< strip1.numPixels(); i++) {
      set_all_strip(i,col);
    }
    set_all_brightness(brightnessFlash);
    show_all_strip();
    delay(time);
    for(i=0; i< strip1.numPixels(); i++) {
      set_all_strip(i,strip1.Color(0,0,0));
    }
    show_all_strip();
    jRainbow++;
    if(jRainbow>256)
      jRainbow=0;
}

void colorBride(long time, int brightnessFlash)
{
  for(int i = 0; i<28;i++)
  {
    if(i<14)
    strip1.setPixelColor(i,backColor[i]);
    else 
    strip2.setPixelColor(i-14,backColor[i]);
  }
    show_all_strip();
    int led1 = random(0,strip1.numPixels());
    int led2 = random(0,strip1.numPixels());
    strip1.setPixelColor(led1,strip1.Color(brightnessFlash,brightnessFlash,brightnessFlash));
    show_all_strip();
    strip2.setPixelColor(led2,strip1.Color(brightnessFlash,brightnessFlash,brightnessFlash));
    show_all_strip();
    delay(time);
    strip1.setPixelColor(led1,backColor[led1]);
    strip2.setPixelColor(led2,backColor[led2+14]);
    show_all_strip();
}
void rainbowCycle(uint8_t wait,int b)
{
  uint16_t i;
  for(i=0; i< strip1.numPixels(); i++) {
      set_all_strip(i, Wheel(((i * 256 / strip1.numPixels()) + jRainbow) & 255));
    }
    set_all_brightness(b);
    show_all_strip();
    delay(wait);
    jRainbow++;
    if(jRainbow>256)
      jRainbow=0;
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




 void show_all_strip()
 {
   strip1.show();
   //strip2.show();
   }
   
void set_all_strip(int i , uint32_t c)
{
  strip1.setPixelColor(i,c);
  //strip2.setPixelColor(i,c);
  }
  
void set_all_strip(int i , int r, int g ,int b)
{
  strip1.setPixelColor(i,strip1.Color(r,g,b));
 // strip2.setPixelColor(i,strip2.Color(r,g,b));
  }
void set_all_brightness(int b)
{
  strip1.setBrightness(b);
  //strip2.setBrightness(b);
  }
  uint32_t randomColor(int a,int b)
{
  int c1 = random(a,b);
  int c2 = random(a,b);
  int c3 = random(a,b);
  return strip1.Color(c1,c2,c3);
}


void setPixelColorSym(int i,uint32_t c)
{
	if (mode == 5){
		strip1.setPixelColor(i, c);
		strip2.setPixelColor((strip1.numPixels() / 2) - 1 - i, c);
		strip1.setPixelColor((strip1.numPixels() - i - 1), c);
		strip2.setPixelColor((strip1.numPixels() / 2) + i, c);
	}
	else if (mode == 6)
	{
		strip1.setPixelColor(i, c);
		strip2.setPixelColor(i, c);	
		strip1.setPixelColor((strip1.numPixels() - i - 1), c);
		strip2.setPixelColor((strip1.numPixels() - i - 1), c);
	}
	else if (mode == 7)
	{
		strip1.setPixelColor((strip1.numPixels() / 2) - 1 - i, c);
		strip2.setPixelColor((strip1.numPixels() / 2) - 1 - i, c);
		strip1.setPixelColor((strip1.numPixels() / 2) + i, c);
		strip2.setPixelColor((strip1.numPixels() / 2) + i, c);
	}
}
void setPixelColorSym(int i,unsigned int c1,unsigned int c2,unsigned int c3)
{
	if (mode == 5)
	{
		strip1.setPixelColor(i, c1, c2, c3);
		strip2.setPixelColor((strip1.numPixels() / 2) - 1 - i,strip2.Color(c1, c2, c3));
		strip1.setPixelColor((strip1.numPixels() - i - 1), strip1.Color(c1, c2, c3));
		strip2.setPixelColor((strip1.numPixels() / 2) + i, strip2.Color(c1, c2, c3));
	}
	else if (mode == 6)
	{
		strip1.setPixelColor(i, strip1.Color(c1, c2, c3));
		strip2.setPixelColor(i, strip2.Color(c1, c2, c3));
		strip1.setPixelColor((strip1.numPixels() - i - 1), strip1.Color(c1, c2, c3));
		strip2.setPixelColor((strip1.numPixels() - i - 1), strip2.Color(c1, c2, c3));
	}
	else if (mode == 7)
	{
		strip1.setPixelColor((strip1.numPixels() / 2) - 1 - i, strip1.Color(c1, c2, c3));
		strip2.setPixelColor((strip1.numPixels() / 2) - 1 - i, strip2.Color(c1, c2, c3));
		strip1.setPixelColor((strip1.numPixels() / 2) + i, strip1.Color(c1, c2, c3));
		strip2.setPixelColor((strip1.numPixels() / 2) + i, strip2.Color(c1, c2, c3));
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

void readMSGEQ7s()
// Read the seven spectrum bands from the two MSGEQ7 chips
{
  digitalWrite(ctrlReset, HIGH);                     // Pulse the reset signal
  digitalWrite(ctrlReset, LOW);                      // Causes MSGEQ7s to latch spectrum values
  delayMicroseconds(75);                             // Delay to meet minimum reset-to-strobe time

  for(int i=0; i <7; i++)                            // Cycle through the 7 spectrum bands
  {
    digitalWrite(ctrlStrobe,LOW);                    // Read current band (then increment to next band)
    delayMicroseconds(40);                           // Wait for outputs to settle
    
    spectrum[i] = analogRead(channel) /4 ;       // Store current values from left & right channels 
                                                      // Divide 0-1023 by 4 to give 0-255
    
     //if (spectrum[i] < 65) spectrum[i] = 0;   // Filter out the noise at the bottom if there
                                               // is no signal worth looking at
    
    digitalWrite(ctrlStrobe,HIGH);
    delayMicroseconds(40);                           // Delay to meet minimum strobe-to-strobe time
  }
}


