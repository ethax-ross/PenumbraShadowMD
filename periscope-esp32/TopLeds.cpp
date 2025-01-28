
#include "TopLeds.h"

TopLeds::TopLeds(CRGB *leds, int numleds){
  this->lastUpdate=millis();
  this->leds=leds;
  this->numleds=numleds;
  this->speed=200;
  this->idx=0;
  this->effect=0;
}

void TopLeds::update(unsigned long currentTime)
{
  if ((currentTime - this->lastUpdate) < this->speed ) return;

  switch (this->effect) {
    case 0:
      this->leftrun();
      break;
    case 1:
      this->leftright();
      break;
    case 2:
      this->tocenter();
      break;
  }

  EVERY_N_MILLISECONDS(5000) { 
    this->effect++;
    this->idx=0;
  }
  if (this->effect>=3){
    this->effect=0;
  }
    
  this->lastUpdate=currentTime;
}


void TopLeds::leftrun()
{
    CRGB *l = this->leds;
    fill_solid(l,this->numleds,CRGB::Black);

    for(int i=0;i<this->numleds;i++){
      if (this->idx==i){
        *l = CRGB::White;
      }
      l++;
    }
    this->idx++;
    if (this->idx==this->numleds){
      this->idx=0;
    }

}

void TopLeds::leftright()
{
    CRGB *l = this->leds;
    fill_solid(l,this->numleds,CRGB::Black);

    int left=this->idx;
    int right=(this->numleds-1)-this->idx;

    for(int i=0;i<this->numleds;i++){
      if (left==i || right==i){
        *(l+i) = CRGB::White;
      }
    }
    this->idx++;
    if (this->idx>=6){
      this->idx=0;
    }

}

void TopLeds::tocenter(){

    CRGB *l = this->leds;
    fill_solid(l,this->numleds,CRGB::Black);

    int left=this->idx;
    int right=(this->numleds-1)-this->idx;

    for(int i=0;i<this->numleds;i++){
      if (left==i || right==i){
        *(l+i) = CRGB::White;
      }
    }
    this->idx++;
    if (this->idx>=4){
      this->idx=0;
    }

}
