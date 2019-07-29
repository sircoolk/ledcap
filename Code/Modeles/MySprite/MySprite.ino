#include <Adafruit_NeoPixel.h>

#define PIN1 6
#define PIN2 9

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(14, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(14, PIN2, NEO_GRB + NEO_KHZ800);
void setup() {
  strip1.begin();
  strip2.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.show(); 
 }


void loop()
{ 
   
  test(randomColor(0,255),10,20); // p1 = couleur p2=longeur de trame p3=vitesse
  //fade(10,randomColor(0,200));
  //rFadeLed(randomColor(0,255),100);
  //shine(10,strip1.Color(255,255,255));
  //shineRainbow(10);
  //rColor(10,20);
  //colorBride(50,50,150,100); //  FlashSpeed, brightnessColor , brightnessFlash
  //randomStrip(500,10);
  //FandC(500,50,100,255,1);
  //glideStrip(randomColor(0,255), 500,2,5,10); //bug
  //cyclon(5,randomColor(0,255),1);
  //rainbow(100);
  
}




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


void FandC(long time,long time2, int brightnessColor , int brightnessFlash,int n)
{
  //init
  long color[strip1.numPixels()];
  for(int i = 0; i<strip1.numPixels();i++)
  {
    strip1.setPixelColor(i,randomColor(0,brightnessColor));
    color[i]= strip1.getPixelColor(i);
  }
  strip1.show();
  // flash
  for(int i = 0; i<n;i++)
  {
  delay(time);
   for(int i = 0; i<strip1.numPixels();i++)
  {
    strip1.setPixelColor(i,brightnessFlash,brightnessFlash,brightnessFlash);
  }
  strip1.show();
  for( int i = 0; i<strip1.numPixels();i++)
  {
    strip1.setPixelColor(i,color[i]);
   
  }
  delay(time2);
  strip1.show();
  }
}


void randomStrip(long time,int n)
{
  strip1.setBrightness(255);
  for(int x =0 ; x<n;x++)
  {
  int i = random(0,strip1.numPixels());
  int l = random(1,5);
  long color =randomColor(0,150);
  for(int j = i-l; j<=i+l;j++)
  {
    strip1.setPixelColor(j,color);
  }
  strip1.show();
  delay(time);
  }
}

void colorBride(long time, int brightnessColor , int brightnessFlash,int n)
{
  for(int i = 0; i<strip1.numPixels();i++)
  {
    strip1.setPixelColor(i,randomColor(0,brightnessColor));
  }
  for(int i = 0 ; i<n;i++)
  {
    int led1 = random(0,strip1.numPixels());
    int led2 = random(0,strip1.numPixels());
    strip1.setPixelColor(led1,strip1.Color(brightnessFlash,brightnessFlash,brightnessFlash));
    strip1.show();
    strip1.setPixelColor(led2,strip1.Color(brightnessFlash,brightnessFlash,brightnessFlash));
    strip1.show();
    delay(time);
    strip1.setPixelColor(led1,randomColor(0,brightnessColor));
    strip1.setPixelColor(led2,randomColor(0,brightnessColor));
    strip1.show();

  }
}

void rFadeLed(long color, long time)
{
  int led = random(0,strip1.numPixels());
  for(int i = 0 ;i<256;i++)
    {
      strip1.setPixelColor(led,color);
      strip1.setBrightness(i);
      strip1.show();
      //delay(time);
    }
    delay(time);
  for(int j = 255 ;j>1;j--)
    {
      strip1.setBrightness(j);
      strip1.show();
      //delay(time);
    }
}

void flashColor(long color,long time)
{
  strip1.setBrightness(255);
  for(int i =0; i<strip1.numPixels();i++)
  {
    strip1.setPixelColor(i,color);
  }
  strip1.show();
  delay(time);
}

void glideStrip(uint32_t c, int t,int l,int b, int e)
{
  long color[strip1.numPixels()];
  for(int i = 0 ; i<strip1.numPixels();i++)
  {
    strip1.setPixelColor(i,strip1.Color(255,0,0));
    color[i]= strip1.getPixelColor(i);
  }
  strip1.show();
  
  
  
 /* long color[strip1.numPixels()];
  for(int i = 0; i<strip1.numPixels();i++)
  {
    color[i]= strip1.getPixelColor(i);
  }
  */
    strip1.setBrightness(255);
  if(b<=e)
  {
  for(int i =b;i<=e+l;i++)
    {
      int last ;
      strip1.setPixelColor(i,c);
      if(i<b+l)
      {
        last = i+ e-b-l+1;
      }
      else
      {
      last = i-l;
      }
      strip1.setPixelColor(last,color[last]);
      strip1.show();
      delay(t);
    }
  }
  else 
  {
  }
}
void rColor(uint8_t t,int n)
{
  for(int i = 0 ;i<n;i++)
  {
  int c1 = random(0,256);
  int c2 = random(0,256);
  int c3 = random(0,256);
  int p  = random(0,strip1.numPixels());
  strip1.setBrightness(255);
  strip1.setPixelColor(p,strip1.Color(c1,c2,c3));
  strip1.show();
  delay(t);
  }
}

void shineRainbow(uint8_t time)
{
  int c1 = random(0,256);
  int c2 = random(0,256);
  int c3 = random(0,256);
  int num = strip1.numPixels();
  int d1 = random(0,num/2);
  int d2 = random(num/2,num);
  
 uint32_t col1 = strip1.Color(c1,c2,c3);
 uint32_t col2 = strip1.Color(c2,c3,c1);
  strip1.setBrightness(255);
  strip1.setPixelColor(d1,col1);
  strip1.show();
  delay(time);
  strip1.setPixelColor(d2,col2);
  strip1.show();
  delay(time);
   strip1.setPixelColor(d1,strip1.Color(0,0,0));
  strip1.show();
   delay(time);
   strip1.setPixelColor(d2,strip1.Color(0,0,0));
  strip1.show();
   delay(time);
 
}

void shine(uint8_t time,uint32_t c)
{
    strip1.setBrightness(255);
  int num = strip1.numPixels();
  long d1 = random(0,num/2);
  long d2 = random(num/2,num);
  
 
  
  
  strip1.setPixelColor(d1,c);
  strip1.show();
  delay(time);
  strip1.setPixelColor(d2,c);
  strip1.show();
  delay(time);
   strip1.setPixelColor(d1,strip1.Color(0,0,0));
  strip1.show();
   delay(time);
   strip1.setPixelColor(d2,strip1.Color(0,0,0));
  strip1.show();
   delay(time);
 
  
  
}



void fade(uint8_t time,uint32_t c)
{
  for(int i =10;i<256;i++)
  {
    for(int x = 0; x<strip1.numPixels();x++)
    {
      
    strip1.setPixelColor(x,c);
    strip1.setBrightness(i);
    }
    strip1.show();
    delay(time);
  }
  delay(time*100);
  for(int j = 255; j>=20;j--)
  {
    for(int y = 0; y<strip1.numPixels();y++)
    {
    strip1.setBrightness(j);
    }
    strip1.show();
    delay(time);
  }
  //delay(time);
}


void strobo(uint8_t time)
{
  for(uint8_t i = 0; i<strip1.numPixels();i++)
  {
    strip1.setPixelColor(i,strip1.Color(255,255,255));
  }
  strip1.show();
  delay(time);
  for(uint8_t j = 0; j<strip1.numPixels();j++)
  {
    strip1.setPixelColor(j,strip1.Color(0,0,0));
  }
  strip1.show();
  delay(time);
  
  
}
void test(uint32_t c,uint8_t l,uint8_t s)
{
 
  
  for(int i = 0; i<strip1.numPixels(); i++)
  {
   
    strip1.setPixelColor(i,c);
    int8_t e;
    if(i<l)
    {
      e=i+strip1.numPixels()-l;
    }
    else{
      e=i-l;
    }
    strip1.setPixelColor(e,strip1.Color(0,0,0));
    strip1.show();
    delay(s);
  }
}
void rainbow(uint16_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip1.numPixels(); i++) {
      strip1.setPixelColor(i, Wheel((i+j) & 255));
      strip2.setPixelColor(i, Wheel((i-j) & 255));
    }
    strip1.show();
    strip2.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip1.numPixels(); i++) {
      strip1.setPixelColor(i, Wheel(((i * 256 / strip1.numPixels()) + j) & 255));
    }
    strip1.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip1.numPixels(); i=i+3) {
        strip1.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip1.show();
     
      delay(wait);
     
      for (int i=0; i < strip1.numPixels(); i=i+3) {
        strip1.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip1.numPixels(); i=i+3) {
          strip1.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip1.show();
       
        delay(wait);
       
        for (int i=0; i < strip1.numPixels(); i=i+3) {
          strip1.setPixelColor(i+q, 0);        //turn every third pixel off
        }
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

