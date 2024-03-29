/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-9 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#ifndef DEMOJUCEPLUGINFILTER_H
#define DEMOJUCEPLUGINFILTER_H

#include "Highlife.h"

#define NUM_ZONESLOTS 8

#define DEFAULT_MAXIMUM_CLIPS 16

//==============================================================================
// A wrapper Juce filter that calls through to a Highlife instance to do processing, 
// and exposes a few of highlife's parameters.
class DemoJuceFilter  : public AudioProcessor,
                        public ChangeBroadcaster
{
public:
    //==============================================================================
    DemoJuceFilter();
    ~DemoJuceFilter();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer,
                       MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (const int channelIndex) const;
    const String getOutputChannelName (const int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;

    //==============================================================================
    int getNumPrograms()                                        { return 0; }
    int getCurrentProgram()                                     { return 0; }
    void setCurrentProgram (int index)                          { }
    const String getProgramName (int index)                     { return String::empty; }
    void changeProgramName (int index, const String& newName)   { }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;

    //==============================================================================
    juce_UseDebuggingNewOperator
    
   //==============================================================================
   int getMaxZoneslots() { return NUM_ZONESLOTS; }; // fuc it for dynamic params, up to 8 slots only !
   int getNumZoneslots();
   void setNumZoneslots(int newslots);
   
   String getZoneslotSample(int slot);
   void setZoneslotSample(int slot, const String sampleFile);
   
   void clearZoneslotClips(int zoneslot);
   int getZoneslotNumClips(int zoneslot);
   String getZoneslotClipFile(int zoneslot, int clipIndex);
   void setZoneslotClipFile(int zoneslot, int clipIndex, const String sampleFile);
   
   int getZoneslotCurrentClip(int zoneslot);
   void setZoneslotCurrentClip(int zoneslot, int clipIndex);

   int getPolyMode();
   void setPolyMode(int m);

   // get/set slot parameters in normed range (i.e. 0-1) (used to implement get/setParameter) (misnamed!)
   float getNormedParam(int slot, int paramId);
   void setNormedParam(int slot, int paramId, float val);
    
   // get/set slot parameters in real (aka "raw" - unnormalised, user-meaningful) units
   // (these are the only place we have a switch statement mapping our Parameters to highlife members/methods)
   float getRawParam(int slot, int paramId);
   void setRawParam(int slot, int paramId, float val);
    
    enum Parameters {
       KeyLow = 0,
       KeyCentre,
       KeyHigh,
       Tune,
       Fader,
       BPMSync,
       SyncTicks,
       Attack,
       Decay,
       Sustain,
       Release,
       FilterType, // 0 = off, 1=lowpass, 2=highpass, 3=bandpass, 4=bandreject
       FilterCutoff,
       FilterResonance,
       CurrentClip,
       CueSlices,
       LoopMode,
       LoopStart,
       LoopEnd,
       LowlifeParameterCount
    };
   enum { paramsPerSlot = LowlifeParameterCount }; 
   static const int MinKey;
   static const int MaxKey;
   static const int MinFader;
   static const int MaxFader;
   static const int MinTune;
   static const int MaxTune;
   static const int MinPolyMode;
   static const int MaxPolyMode;
   static const int MinSyncTicks;
   static const int MaxSyncTicks;
   static const int MinSliceExponent;
   static const int MaxSliceExponent;
   static const int MaxLoopMode;
//   static const int MinFilterType;
//   static const int MaxFilterType;

private:
   void setZoneslotCurrentPlayingSample(int slot, const File sampleFile);
   void setZoneslotCueSlices(int slot, int numSlices);

   HIGHLIFE_PROGRAM& getHProgramRef(int zoneslot = 0) // don't care baout zoneslot anymore!!
   {
      return highlifeInstance.highlife_program[0]; // only use the first program
   }
   HIGHLIFE_PROGRAM* getHProgram(int zoneslot = 0) 
   {
      return &highlifeInstance.highlife_program[0];
   }
   HIGHLIFE_ZONE* getHZone(int zoneslot) 
   {
      HIGHLIFE_ZONE* zo = 0;
      if (zoneslot < getNumZoneslots())
      {
         zo = &getHProgramRef().pzones[zoneslot];      
      }
      return zo;
   }

   int slotCurrentClip[NUM_ZONESLOTS];   
   StringArray slotClipFiles[NUM_ZONESLOTS];
   
private:
    float gain;
    
    CHighLife highlifeInstance;
};

#endif
