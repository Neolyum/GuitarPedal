#ifndef faust_teensy_h_
#define faust_teensy_h_

#include "Arduino.h"
#include "AudioStream.h"
#include "Audio.h"
#include "MyDistortion.h"
#include "MyEcho.h"
#include "MyTremolo.h"

#define AUDIO_CHANNELS 2

class MyDsp : public AudioStream
{
  public:
    MyDsp();
    ~MyDsp();
    
    virtual void update(void);
    const char* toggleEffect();
    
  private:
    MyDistortion myDistortion;
    MyEcho echo = MyEcho(44100); // MAgic number yay
    MyTremolo myTremolo = MyTremolo(44100);

    enum Effect {
      NoEffect,
      DistortionEffect,
      EchoEffect,
      TremoloEffect,
      COUNT // has to be last
    };


    Effect currentEffect;

    const char* EffectNames[4] = {
      "NoEffect",
      "DistortionEffect",
      "EchoEffect",
      "TremoloEffect"
    };
};

#endif
