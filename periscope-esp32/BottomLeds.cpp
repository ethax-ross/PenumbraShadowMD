#include "BottomLeds.h"

BottomLeds::BottomLeds(CRGB *leds, int numleds){
  this->lastUpdate=millis();
  this->leds=leds;
  this->numleds=numleds;
  this->speed=200;
  this->idx=0;
  this->effect=0;
}

void BottomLeds::update(unsigned long currentTime)
{
  if ((currentTime - this->lastUpdate) < this->speed ) return;

  switch (this->effect) {
    case 0:
      this->superscan();
      break;
    case 1:
       this->scan();
       break;
    case 2:
       this->simple();
       break;
  }

  EVERY_N_MILLISECONDS(10000) { 
    this->effect++;
    this->idx=0; 
  }
  if (this->effect>=3){
    this->effect=0;
  }
  
  this->lastUpdate=currentTime;
}

void BottomLeds::simple()
{
  CRGB *l = this->leds;
  fill_solid(l,this->numleds,CRGB::Black);
  
  for(int i=0;i<this->numleds;i++){
    if (this->idx==i){
      *l = CRGB::Red;
    }
    l++;
  }
  this->idx++;
  if (this->idx==this->numleds){
    this->idx=0;
  }
}

void BottomLeds::scan()
{
  CRGB *l = this->leds;
  fill_solid(l,this->numleds,CRGB::Black);

  for(int i=0;i<this->numleds;i++){
    if (i == this->idx || i == this->idx+1){
      *l = CRGB::Red;
    }
    l++;
  }

  this->idx+=2;
  
  if (this->idx==8){
      this->idx=0;
  }
  
}

void BottomLeds::superscan()
{
  CRGB *l = this->leds;
  fill_solid(l,this->numleds,CRGB::Black);

  switch (this->idx){
    case 0:
      l[0] = CRGB::Red;
      l[1] = CRGB::Red;
      l[6] = CRGB::Red;
      l[7] = CRGB::Red;
      break;
    case 1:
      l[2] = CRGB::Red;
      l[3] = CRGB::Red;
      l[4] = CRGB::Red;
      l[5] = CRGB::Red;
      break;
    case 2:
      l[3] = CRGB::Red;
      l[5] = CRGB::Red;
      break;
    case 3:
      l[2] = CRGB::Red;
      l[4] = CRGB::Red;
      break;
    case 4:
      l[3] = CRGB::Red;
      l[5] = CRGB::Red;
      break;
    case 5:
      l[2] = CRGB::Red;
      l[3] = CRGB::Red;
      l[4] = CRGB::Red;
      l[5] = CRGB::Red;
      break;
      
  }

  this->idx++;
  if (this->idx==6){
    this->idx=0;
  }
}
