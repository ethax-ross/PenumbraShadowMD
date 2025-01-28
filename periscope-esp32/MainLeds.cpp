#include "MainLeds.h"

MainLeds::MainLeds(CRGB *leds, int numleds)
{
  this->lastUpdate=millis();
  this->leds=leds;
  this->numleds=numleds;
  this->speed=100;
  
  this->idx=0;

  this->pulse_speed=20;
  this->pulse=50;
  this->pulse_offset=1;

  this->strobe_ind=false;

  this->effect=0;
}

void MainLeds::update(unsigned long currentTime)
{
  if ((currentTime - this->lastUpdate) > this->pulse_speed )
  {
    this->pulseCenter();
  }
  
  if ((currentTime - this->lastUpdate) < this->speed) return;

  switch(this->effect) {
    case 0:
      this->pulseAll();
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
      this->pulseAll();
      break;
    case 6:
      this->cw_run(1);
      break;
    case 7:
      this->cw_split2();
      break;
    case 8:
      this->cw_split3();
      break;
    case 9:
      this->cw_split4();
      break;
    case 10:
      this->strobe();
      break;
  }
  EVERY_N_MILLISECONDS(10000) { 
    this->effect++;
    this->speed=100;
    this->idx=0;
    this->pulse_speed=20;
    this->pulse=50;
    this->pulse_offset=1;  
    this->strobe_ind=false;
  }
  if (this->effect>=11){
    this->effect=0;
  }
  
  this->lastUpdate=currentTime;
}

void MainLeds::pulseCenter()
{
  CHSV hsv = rgb2hsv_approximate( CRGB::White );
  hsv.v = this->pulse;
  *(this->leds) = hsv;
  
  this->pulse += this->pulse_offset;  
  if (this->pulse == 255 || this->pulse == 50){
      this->pulse_offset = - (this->pulse_offset);
  }
}

void MainLeds::pulseAll()
{
  CHSV hsv = rgb2hsv_approximate( CRGB::White );
  hsv.v = this->pulse;
  CRGB *led = this->leds;
  for(int  i=0;i<this->numleds;i++){
    *led = hsv;
    led++;
  }
  this->pulse += this->pulse_offset;  
  if (this->pulse == 255 || this->pulse == 50){
      this->pulse_offset = - (this->pulse_offset);
  }
}

void MainLeds::cw_run(int pt)
{
  CRGB *led = this->leds+1;
  fill_solid(led,this->numleds-1,CRGB::Black);

  int bright=255;
  CHSV cl = CHSV( 100, 0, bright); //white
  
  for(int x=0;x<pt;x++){
    led = this->leds+1;
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
  }
  this->idx++;
  if (this->idx == 8){
    this->idx=0;
  }
}

void MainLeds::cw_split2()
{
  CRGB *led = this->leds+1;
  fill_solid(led,this->numleds-1,CRGB::Black);

  for(int i=0;i<this->numleds;i++){
    int x = this->idx + ( (this->numleds-1)/2 );
    if (x>=8){
      x = x%8;
    }
    if (this->idx==i || x == i){
      *led = CRGB::White;
    }
    led++;
  }
  this->idx++;
  if (this->idx>=8){
    this->idx=0;
  }
}

void MainLeds::cw_split3()
{
  CRGB *led = this->leds+1;
  fill_solid(led,this->numleds-1,CRGB::Black);

  for(int i=0;i<this->numleds;i++){
    int offset = (this->numleds-1)/3;
    int x = this->idx + offset;
    if (x>=8){
      x = x%8;
    }
    int y =this->idx + (offset*2);
    if (y>=8){
      y = y%8;
    }
    
    if (this->idx==i || x == i || y == i){
      *led = CRGB::White;
    }
    led++;
  }
  this->idx++;
  if (this->idx>=8){
    this->idx=0;
  }
}

void MainLeds::cw_split4()
{
  CRGB *led = this->leds+1;
  fill_solid(led,this->numleds-1,CRGB::Black);

  for(int i=0;i<this->numleds;i++){
    int offset = (this->numleds-1)/4;
    int x = this->idx + offset;
    if (x>=8){
      x = x%8;
    }
    int y =this->idx + (offset*2);
    if (y>=8){
      y = y%8;
    }
    int z =this->idx + (offset*3);
    if (z>=8){
      z = z%8;
    }
    
    if (this->idx==i || x == i || y == i || z == i){
      *led = CRGB::White;
    }
    led++;
  }
  this->idx++;
  if (this->idx>=8){
    this->idx=0;
  }
}

void MainLeds::strobe() {
  CRGB *led = this->leds;
  fill_solid(led,this->numleds,CRGB::Black);
  if (this->strobe_ind){
    fill_solid(led,this->numleds,CRGB::White);
  }
  this->strobe_ind = ! this->strobe_ind;
}
