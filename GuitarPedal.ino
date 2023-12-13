#include "MyDsp.h"
#include <Audio.h>
MyDsp myDsp;
AudioOutputI2S out;
AudioInputI2S in;
AudioControlSGTL5000 audioShield;

AudioConnection patchCord0(myDsp,0,out,0);
AudioConnection patchCord1(myDsp,1,out,1);

AudioConnection patchCord3(in,0,myDsp,0);
AudioConnection patchCord4(in,1,myDsp,1);

void setup() {
  Serial.begin(9600);
  AudioMemory(4);
  audioShield.enable();
  audioShield.volume(0.5);
}

void loop() {
  Serial.println("esiodfh");
  audioShield.volume(analogRead(A2) / 1023.0f);
  delay(1000);
}
