#include "BackLeds.h"

BackLeds::BackLeds(CRGB *leds, int numleds)
{
  this->lastUpdate=millis();
  this->leds=leds;
  this->numleds=numleds;
  this->speed=500;
  this->idx=0;
}

void BackLeds::update(unsigned long currentTime)
{
  if ((currentTime - this->lastUpdate) < this->speed ) return;

  //TODO - update effekt
  this->randomB();

  this->lastUpdate=currentTime;
}

void BackLeds::randomB(){
  CRGB *led = this->leds;
  fill_solid(led,this->numleds,CRGB::Black);

  for(int i=0;i<this->numleds;i++){
    int nr = random(0,2);
    if (nr){
      *led = CRGB::Red;  
    }
    else {
      *led = CRGB::Blue;
    }
    led++;
  }
}
