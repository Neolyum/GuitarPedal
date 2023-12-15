#ifndef faust_teensy_h_
#define faust_teensy_h_

#include "Arduino.h"
#include "AudioStream.h"
#include "Audio.h"
#include "Distortion.h"

#define AUDIO_CHANNELS 2

class MyDsp : public AudioStream
{
  public:
    MyDsp();
    ~MyDsp();
    
    virtual void update(void);
    void toggleEffect();
    
  private:
    Distortion distortion;
    enum Effect {
      DistortionEffect,
      ChorusEffect,
      COUNT // has to be last
    };
    Effect currentEffect;
};

#endif
