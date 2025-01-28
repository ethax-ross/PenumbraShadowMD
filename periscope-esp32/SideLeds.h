#ifndef SIDELEDS_H
#define SIDELEDS_H

#include "BaseLeds.h"

class SideLeds: public BaseLeds {
  public:
  
  SideLeds(CRGB *leds, int numleds);

  void update(unsigned long currentTime);
  
  void pulse();
  void strobe();
  void cw_run(int pt);
  
  private:

  unsigned long lastUpdate;
  CRGB *leds;
  int numleds;
  unsigned long speed;
  int idx;

  int pulse_offset;
  bool strobe_ind;

  int effect;
  unsigned long effect_time;
};
  
#endif
