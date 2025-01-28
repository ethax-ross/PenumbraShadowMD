#include "SideLeds.h"

SideLeds::SideLeds(CRGB *leds, int numleds)
{
  this->lastUpdate=millis();
  this->leds=leds;
  this->numleds=numleds;
  this->speed=200;
  this->idx=0;

  this->pulse_offset=10;
  this->strobe_ind = false;

  this->effect=0;
  this->effect_time=millis();
}

void SideLeds::update(unsigned long currentTime)
{
  if ((currentTime - this->lastUpdate) < this->speed ) return;

  switch(this->effect){
    case 0:
      this->pulse();
      break;
    case 1:
      this->cw_run(1);
      break;
    case 2:
      this->cw_run(2);
      break;
    case 3:
      this->cw_run(3);
      break;
    case 4:
      this->cw_run(4);
      break;
    case 5:
      this->strobe();
      break;
  }
/*
  EVERY_N_MILLISECONDS(5000) { 
    this->effect++;
    this->speed=200;
    this->idx=0;
    this->pulse_offset=10;
    this->strobe_ind = false;
  }
  if (this->effect>=6){
    this->effect=0;
  }
*/   
  if ( (currentTime - this->effect_time) > 5000 )
  {
    this->effect++;
    this->speed=200;
    this->idx=0;
    this->pulse_offset=10;
    this->strobe_ind = false;

    this->effect_time = currentTime;
  }
  if (this->effect>=6){
    this->effect=0;
  }
  
  this->lastUpdate=currentTime;
}

void SideLeds::pulse(){
  CRGB *leds = this->leds;

  for(int i=0;i<this->numleds;i++){
    *leds = CHSV(255,0,this->idx);
    leds++;
  }
  this->idx+=this->pulse_offset;
  if (this->idx>=255 || this->idx<=0){
    this->pulse_offset = - this->pulse_offset;
    this->idx+=this->pulse_offset;
  }
}

void SideLeds::strobe(){
  fill_solid(this->leds,this->numleds,CRGB::Black);
  if (this->strobe_ind){
    fill_solid(this->leds,this->numleds,CRGB::White);
  }
  this->strobe_ind= ! this->strobe_ind;
}

void SideLeds::cw_run(int pt)
{
  CRGB *led = this->leds;
  fill_solid(led,this->numleds,CRGB::Black);
  
  int bright=255;
  CHSV cl = CHSV( 100, 0, bright); //white
  
  for(int x=0;x<pt;x++){
    led = this->leds;
    int y = this->idx+x;
    if (y>7){
      y = y%8;
    }
    cl = CHSV(100,0,bright);
    for(int i=0;i<this->numleds-1;i++){
      if (y==i){
        *led = cl;
      }
      led++;
    }
    *led=CRGB::White;
  }
  this->idx++;
  if (this->idx == 8){
    this->idx=0;
  }
}
