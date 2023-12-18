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
  AudioMemory(16);
  audioShield.enable();
  audioShield.volume(0.5);
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.micGain(3);
  delay(1000);
}

bool lastPosition = false;
void loop() {
  audioShield.volume(analogRead(A2) / 1023.0f);

  bool read = digitalRead(0);
  if (read != lastPosition && (lastPosition == false)) {
    lastPosition = !lastPosition;
    Serial.printf("Toggled effect to \"%s\"\n", myDsp.toggleEffect());

  }
  if (!read) lastPosition = false;

}
