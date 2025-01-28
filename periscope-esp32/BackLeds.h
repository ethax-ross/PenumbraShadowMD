#ifndef BACKLEDS_H
#define BACKLEDS_H

#include "BaseLeds.h"

class BackLeds: public BaseLeds {
  public:
  
  BackLeds(CRGB *leds, int numleds);

  void update(unsigned long currentTime);

  void randomB();
  
  private:

  unsigned long lastUpdate;
  CRGB *leds;
  int numleds;
  unsigned long speed;
  int idx;
};
  
#endif
