#ifndef MAINLEDS_H
#define MAINLEDS_H

#include "BaseLeds.h"

class MainLeds:public BaseLeds {
  public:
  
  MainLeds(CRGB *leds, int numleds);

  void update(unsigned long currentTime);
  
  private:

  void pulseCenter();
  void pulseAll();
  void cw_run(int pt);
  void cw_split2();
  void cw_split3();
  void cw_split4();
  void strobe();

  unsigned long lastUpdate;
  CRGB *leds;
  int numleds;
  unsigned long speed;
  int idx;

  unsigned long pulse_speed;
  int pulse;
  int pulse_offset;

  boolean strobe_ind;

  int effect;
};
  
#endif
