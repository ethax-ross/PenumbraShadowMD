#ifndef TOPLEDS_H
#define TOPLEDS_H

#include "BaseLeds.h"

class TopLeds: public BaseLeds {
  public:
  
  TopLeds(CRGB *leds, int numleds);
  
  void update(unsigned long currentTime);

  void leftrun();
  void leftright();
  void tocenter();
  
  private:
  
  unsigned long lastUpdate;
  CRGB *leds;
  int numleds;
  unsigned long speed;
  int idx;

  int effect;
};

#endif
