#include <Adafruit_NeoPixel.h>

#define lednum 16   //number of leds in connected in series
#define shft 5      //weight of change
#define activationPC 2  //percentage probability that an led will activate
#define ringpin 3   // hardware arduino pin to first led serial-in

Adafruit_NeoPixel ring = Adafruit_NeoPixel(lednum, ringpin, NEO_GRB + NEO_KHZ800);

int r[lednum];
int g[lednum];
int b[lednum];

bool on[lednum];

int rAve;
int gAve;
int bAve;

unsigned int avecalc=0;
int i=0;



void setup()
{
  ring.begin();
  ring.show();
  Serial.begin(19200);
  for(int set; set < lednum; set++)
  {
    on[set]=false;
  }

}




void loop()
{

  rAve = calcaverage(r, rAve);
  gAve = calcaverage(g, gAve);
  bAve = calcaverage(b, bAve);
  Serial.print("R: ");
  Serial.print(rAve);
  Serial.print("\tG: ");
  Serial.print(gAve);
  Serial.print("\tB: ");
  Serial.print(bAve);
  Serial.print('\n');
  for(i=0; i<lednum; i++)
  {
    r[i] = (1+shift(r[i], rAve, i)+rAve)/2;
    //g[i] = (1+shift(g[i], gAve, i)+gAve)/2;
    //b[i] = (1+shift(b[i], bAve, i)+bAve)/2;
        g[i] = 0;
        b[i] = 0;
        //r[i] = 127;    
    ring.setPixelColor(i, ring.Color(r[i],(g[i]),(b[i])));

    //ring.setPixelColor(i, ring.Color(random(32),random(32),random(32)));    //test pattern
  }
  ring.show();
  delay(25);
}




int calcaverage(int param[], int aveNo)
{
  avecalc=0;
  for(int cnt=0; cnt< lednum; cnt++)  
  {
    if(on[cnt])
    {
      avecalc = avecalc+param[cnt];
    }
  }
  return constrain((avecalc/lednum),random(4,aveNo+10), 96);
}




int shift(int param, int aveNo, int iNo)
{
  if (on[iNo])
  {
    if(param=aveNo)
    {
      return abs(random(aveNo-4,aveNo+4));
      if(random(100)<=10) on[iNo] = false;
    }
    else
    {
      return ((param*100*shft)+aveNo)/((100*shft)+1);
    }
  }
  else
  {
    if(random(400)<activationPC) 
    {
      on[iNo] = true;
      return constrain(random(aveNo+5,aveNo-5),0,255);
    }
    else return 0;
  }
}

