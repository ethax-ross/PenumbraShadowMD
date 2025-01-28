#ifndef BOTTOM_H
#define BOTTOM_H

#include "BaseLeds.h"

class BottomLeds: public BaseLeds {
  public:
  
  BottomLeds(CRGB *leds, int numleds);
  
  void update(unsigned long currentTime);
  
  private:

  void simple();
  void scan();
  void superscan();
  
  unsigned long lastUpdate;
  CRGB *leds;
  int numleds;
  unsigned long speed;
  int idx;
  int effect;
};

#endif
