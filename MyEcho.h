#pragma once
#include "Effect.h"
#include "Echo.h"

class MyEcho : Effect {
public:
  MyEcho(float rate);
  void setGain(float g);
  float tick(float input) override;

  void setStrength(float f) override {
    setGain(f);
  }

private:
  Echo echo;
  float g = 0.8;
};