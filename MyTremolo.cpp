#include "MyTremolo.h"

MyTremolo::MyTremolo(float rate) : 
sine(rate)
{
  sine.setFrequency(5);
  }

float MyTremolo::tick(float input) {
  float temp = sine.tick()*0.25+0.5;
  return input*temp*gain;
}

void MyTremolo::setGain(float g) {
  this->gain = g;
}
