#include "MyEcho.h"

MyEcho::MyEcho(float rate) : 
echo(rate,20000)
{
  // setting up DSP objects
  echo.setDel(20000);
  echo.setFeedback(0.4);
}

float MyEcho::tick(float input) {
  return echo.tick(input) * g;
}

void MyEcho::setGain(float g) {
  this->g = g;
}