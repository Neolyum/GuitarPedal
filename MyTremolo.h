#pragma once
#include "Effect.h"

#include "Sine.h"

class MyTremolo : Effect {
public:
  MyTremolo(float rate);
  void setGain(float g);
  float tick(float input) override;

  void setStrength(float f) override {
    setGain(f);
  }

private:
  Sine sine;
  float gain = 0.8;
};
