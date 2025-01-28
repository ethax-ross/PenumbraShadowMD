#ifndef BASELED_H
#define BASELED_H

#include <FastLED.h>

class BaseLeds {
  virtual void update(unsigned long currentTime)=0;
};

#endif
