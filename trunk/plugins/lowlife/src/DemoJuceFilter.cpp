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

#include "includes.h"
#include "JucePluginCharacteristics.h"
#include "DemoJuceFilter.h"
#include "LowlifeEditorComponent.h"

//==============================================================================
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DemoJuceFilter();
}

//==============================================================================
DemoJuceFilter::DemoJuceFilter()
{
    lastUIWidth = 400;
    lastUIHeight = 300;
    
    getHProgramRef().ply_mode = 2;
}

DemoJuceFilter::~DemoJuceFilter()
{
}

//==============================================================================
const String DemoJuceFilter::getName() const
{
    return JucePlugin_Name;
}

const int DemoJuceFilter::MinKey = 0;
const int DemoJuceFilter::MaxKey = 127;
const int DemoJuceFilter::MinFader = -60;
const int DemoJuceFilter::MaxFader = 3;
const int DemoJuceFilter::MinTune = 0;
const int DemoJuceFilter::MaxTune = 100;
const int DemoJuceFilter::MinPolyMode = 0;
const int DemoJuceFilter::MaxPolyMode = 2;
const int DemoJuceFilter::MinSyncTicks = 0;
const int DemoJuceFilter::MaxSyncTicks = 999; // ticks are 16th notes
const int DemoJuceFilter::MinFilterType = 0;
const int DemoJuceFilter::MaxFilterType = 4; 

float DemoJuceFilter::getNormedParam(int slot, int paramId)
{
   float paramVal = getRawParam(slot, paramId);
   // map special parameters into their special ranges
   switch (paramId) 
   {
      case KeyLow:
      case KeyCentre:
      case KeyHigh:
         paramVal = ((paramVal - MinKey) +1) / static_cast<float>(1 + MaxKey - MinKey);
         break;
      case Tune:
         paramVal = (paramVal - MinTune) / static_cast<float>(MaxTune - MinTune);
         break;
      case Fader:
         paramVal = (paramVal - MinFader) / static_cast<float>(MaxFader - MinFader);
         break;
      case BPMSync:
         paramVal = (paramVal > 0.5);
         break;
      case SyncTicks:
         paramVal = (paramVal - MinSyncTicks) / static_cast<float>(MaxSyncTicks - MinSyncTicks);
         break;
      case FilterType:
         paramVal = paramVal / MaxFilterType;
         break;
   }
   return paramVal;
}

void DemoJuceFilter::setNormedParam(int slot, int paramId, float val)
{
   // map special parameters from their special ranges
   switch (paramId) 
   {
      case KeyLow:
      case KeyCentre:
      case KeyHigh:
         val = (val * (1 + MaxKey - MinKey)) + MinKey;
         break;
      case Tune:
         val = (val * (1 + MaxTune - MinTune)) + MinTune;
         break;
      case Fader:
         val = (val * (MaxFader - MinFader)) + MinFader;
         break;
      case BPMSync:
         val = (val > 0.5);
         break;
      case SyncTicks:
         val = (MaxSyncTicks - MinSyncTicks) + MinSyncTicks;
         break;
      case FilterType:
         val = round(val * MaxFilterType);
         break;
      default:
         break;
   }

   setRawParam(slot, paramId, val);
}

int DemoJuceFilter::getNumParameters()
{
   int numParams = 0;
   
   numParams = getMaxZoneslots() * paramsPerSlot;
   
   return numParams;
}

float DemoJuceFilter::getParameter (int index)
{
   int slotOfConcern = index / paramsPerSlot;
   int paramOfInterest = index % paramsPerSlot;

   return getNormedParam(slotOfConcern, paramOfInterest);
}

void DemoJuceFilter::setParameter (int index, float newValue)
{
   int slotOfConcern = index / paramsPerSlot;
   int paramOfInterest = index % paramsPerSlot;

   setNormedParam(slotOfConcern, paramOfInterest, newValue);

   sendChangeMessage(this);
}

const String DemoJuceFilter::getParameterName (int index)
{
   int slotOfConcern = index / paramsPerSlot;
   int paramOfInterest = index % paramsPerSlot;

   String slotName = String("Slot") + String(slotOfConcern+1) + (" ");

   switch (paramOfInterest)
   {
      case KeyLow:
         slotName += String("Low key");
      break;
      case KeyCentre:
         slotName += String("Base/centre key");
      break;
      case KeyHigh:
         slotName += String("High key");
      break;
      case Fader:
         slotName += String("Fader");
      break;
      case Tune:
         slotName += String("Tuniness");
      break;
      case BPMSync:
         slotName += String("BPM Sync");
      break;
      case SyncTicks:
         slotName += String("Sample length in 16ths (for BPM sync)");
      break;
      case Attack:
         slotName += String("Env Attack");
      break;
      case Decay:
         slotName += String("Env Decay");
      break;
      case Sustain:
         slotName += String("Env Sustain");
      break;
      case Release:
         slotName += String("Env Release");
      break;
      case FilterType:
         slotName += String("Filter Type");
      break;
      case FilterCutoff:
         slotName += String("Cutoff");
      break;
      case FilterResonance:
         slotName += String("Resonance");
      break;
   }

   return slotName;
}

const String DemoJuceFilter::getParameterText (int index)
{
   int slot = index / paramsPerSlot;
   int paramOfInterest = index % paramsPerSlot;
   String paramVal = String(getRawParam(slot, paramOfInterest));
   return paramVal;
}

float DemoJuceFilter::getRawParam(int slot, int paramId)
{
   float value = 0;
   
   HIGHLIFE_ZONE* pz = getHZone(slot);
   if (pz)
      switch (paramId)
      {
      case KeyLow:
         value = pz->lo_input_range.midi_key;
         break;
      case KeyCentre:
         value = pz->midi_root_key;
         break;
      case KeyHigh:
         value = pz->hi_input_range.midi_key;
         break;
      case Tune:
         value = pz->midi_keycents;
         break;
      case Fader:
         value = pz->mp_gain;
         break;
      case BPMSync:
         value = pz->mp_synchro;
         break;
      case SyncTicks:
         value = pz->mp_num_ticks / 2.0; // our ticks are in beats, highlife ticks are 2 per beat
         break;
      case Attack:
         value = pz->amp_env_att.value;
         break;
      case Decay:
         value = pz->amp_env_dec.value;
         break;
      case Sustain:
         value = pz->amp_env_sus.value;
         break;
      case Release:
         value = pz->amp_env_rel.value;
         break;
      case FilterType:
         value = pz->flt_type;
         break;
      case FilterCutoff:
         value = pz->flt_cut_frq.value;
         break;
      case FilterResonance:
         value = pz->flt_res_amt.value;
         break;
      default:
         break;
      }
   
   return value;
}

void DemoJuceFilter::setRawParam(int slot, int paramId, float value)
{
   HIGHLIFE_ZONE* pz = getHZone(slot);
   if (pz)
      switch (paramId)
      {
      case KeyLow:
         pz->lo_input_range.midi_key = value;
         break;
      case KeyCentre:
         pz->midi_root_key = value;
         break;
      case KeyHigh:
         pz->hi_input_range.midi_key = value;
         break;
      case Tune:
         pz->midi_keycents = value;
         break;
      case Fader:
         pz->mp_gain = value;
         break;
      case BPMSync:
         pz->mp_synchro = value;
         break;
      case SyncTicks:
         pz->mp_num_ticks = value * 2.0; // our ticks are in beats, highlife ticks are 2 per beat
         break;
      case Attack:
         pz->amp_env_att.value = value;
         break;
      case Decay:
         pz->amp_env_dec.value = value;
         break;
      case Sustain:
         pz->amp_env_sus.value = value;
         break;
      case Release:
         pz->amp_env_rel.value = value;
         break;
      case FilterType:
         pz->flt_type = value;
         break;
      case FilterCutoff:
         pz->flt_cut_frq.value = value;
         break;
      case FilterResonance:
         pz->flt_res_amt.value = value;
         break;
      default:
         break;
      }
   sendChangeMessage(this);
}

int DemoJuceFilter::getNumZoneslots()
{
   int zoneslots = 1;
   zoneslots = getHProgramRef().num_zones;
   return zoneslots;
}

void DemoJuceFilter::setNumZoneslots(int newslots)
{
   int oldslots = getNumZoneslots();
   if (newslots > oldslots)
   {
      // making new zoneslots
      for (int zoneslotCount=oldslots; zoneslotCount<newslots; zoneslotCount++)
      {
         HIGHLIFE_PROGRAM* pprg = getHProgram(zoneslotCount);
         if (pprg)
            highlifeInstance.tool_alloc_zone(pprg);
      }
      sendChangeMessage(this);
   }
   else if (newslots < oldslots)
   {
      // killing zoneslots
      for (int zoneslotCount=oldslots-1; zoneslotCount>=newslots; zoneslotCount--)
      {
         HIGHLIFE_PROGRAM* pprg = getHProgram(zoneslotCount);
         HIGHLIFE_ZONE* pz = getHZone(zoneslotCount);
         if (pprg && pz)
            highlifeInstance.tool_init_program(pprg); // zeroes out the prog so we know that it is not in use
      }
      sendChangeMessage(this);
   }
}

File DemoJuceFilter::getZoneslotSample(int slot)
{
   HIGHLIFE_ZONE* zo = getHZone(slot);
   File samfile;
   if (zo)
      samfile = File(zo->path);
   return samfile;
}

void DemoJuceFilter::setZoneslotSample(int slot, const File sampleFile)
{
   HIGHLIFE_ZONE* pz = getHZone(slot);
   if (pz)
   {
      highlifeInstance.tool_delete_wave (pz);
      highlifeInstance.tool_load_sample(pz, sampleFile);
   }
   sendChangeMessage (this);
}

int DemoJuceFilter::getPolyMode()
{
   int polymode = 0;
   HIGHLIFE_PROGRAM* pz = getHProgram();
   if (pz)
      polymode = pz->ply_mode;
   if (polymode < MinPolyMode || polymode > MaxPolyMode)
      polymode = MaxPolyMode;

   return polymode;
}

void DemoJuceFilter::setPolyMode(int poly)
{
   if (poly < MinPolyMode || poly > MaxPolyMode)
      poly = MaxPolyMode;

   HIGHLIFE_PROGRAM* pz = getHProgram();
   if (pz)
      pz->ply_mode = poly;
   sendChangeMessage (this);
}

const String DemoJuceFilter::getInputChannelName (const int channelIndex) const
{
    return String(channelIndex + 1);
}

const String DemoJuceFilter::getOutputChannelName (const int channelIndex) const
{
    return String(channelIndex + 1);
}

bool DemoJuceFilter::isInputChannelStereoPair (int index) const
{
    return false;
}

bool DemoJuceFilter::isOutputChannelStereoPair (int index) const
{
    return false;
}

bool DemoJuceFilter::acceptsMidi() const
{
    return true;
}

bool DemoJuceFilter::producesMidi() const
{
    return true;
}

//==============================================================================
void DemoJuceFilter::prepareToPlay (double sampleRate, int samplesPerBlock)
{
       highlifeInstance.prepareToPlay(sampleRate, samplesPerBlock);
       highlifeInstance.setPlayConfigDetails(0, 1, sampleRate, samplesPerBlock);
}

void DemoJuceFilter::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void DemoJuceFilter::processBlock (AudioSampleBuffer& buffer,
                                   MidiBuffer& midiMessages)
{
   highlifeInstance.setPlayHead(getPlayHead());
   highlifeInstance.processBlock(buffer, midiMessages);
}

//==============================================================================
AudioProcessorEditor* DemoJuceFilter::createEditor()
{
    return new LowlifeEditorComponent (this);
}

//==============================================================================

/*// magic number to identify memory blocks that we've stored as XML
const uint32 magicXmlNumber = 0x21324356;

int ruaGetXmlToBinaryLen (const XmlElement& xml)
{
    const String xmlString (xml.createDocument (String::empty, true, false));
    const int stringLength = xmlString.length();
   return stringLength + 8;
}

void ruaCopyXmlToBinary (const XmlElement& xml,
                                      MemoryBlock& destData)
{
    const String xmlString (xml.createDocument (String::empty, true, false));
    const int stringLength = xmlString.length();

    destData.setSize (stringLength + 10);

    char* const d = (char*) destData.getData();
    *(uint32*) d = swapIfBigEndian ((const uint32) magicXmlNumber);
    *(uint32*) (d + 4) = swapIfBigEndian ((const uint32) stringLength);

    xmlString.copyToBuffer (d + 8, stringLength);
}

XmlElement* ruaGetXmlFromBinary (const void* data,
                                              const int sizeInBytes)
{
    if (sizeInBytes > 8
         && littleEndianInt ((const char*) data) == magicXmlNumber)
    {
        const uint32 stringLength = littleEndianInt (((const char*) data) + 4);

        if (stringLength > 0)
        {
            XmlDocument doc (String (((const char*) data) + 8,
                                     jmin ((sizeInBytes - 8), stringLength)));

            return doc.getDocumentElement();
        }
    }

    return 0;
}
*/
void DemoJuceFilter::getStateInformation (MemoryBlock& highLifeState)
{
    // you can store your parameters as binary data if you want to or if you've got
    // a load of binary to put in there, but if you're not doing anything too heavy,
    // XML is a much cleaner way of doing it - here's an example of how to store your
    // params as XML..

//    // create an outer XML element..
//    XmlElement xmlState (T("MYPLUGINSETTINGS"));
//
//    // add some attributes to it..
//    xmlState.setAttribute (T("pluginVersion"), 1);
//    xmlState.setAttribute (T("gainLevel"), gain);
//    xmlState.setAttribute (T("uiWidth"), lastUIWidth);
//    xmlState.setAttribute (T("uiHeight"), lastUIHeight);
//    
//
   
      // for now, we are not saving our real params, but we are saving the highlife state _instead_
      // this is because I am too lazy _right_now_ to deal with xml + binary blob issues!
      // really what is important now is tweaking the save/load of the highlife state so it doesn't save sample data 
      // (just saves file path)
      // when this is crankin, we can sort out saving our own params (if we have any)..
      // ..or only save the parameters from highlife that we want to save, via xml
      // MemoryBlock::toBase64Encoding would do it ;)
      
    // you could also add as many child elements as you need to here..
//    MemoryBlock highLifeState;
   //for (int i=0;i<getMaxZoneslots(); i++)
    highlifeInstance.getStateInformation(highLifeState);

    // then use this helper function to stuff it into the binary blob and return it..
//    copyXmlToBinary (xmlState, highLifeState);
}

void DemoJuceFilter::setStateInformation (const void* data, int sizeInBytes)
{
   setNumZoneslots(0);
   //for (int i=0;i<getMaxZoneslots(); i++)
    highlifeInstance.setStateInformation(data, sizeInBytes);

 //   // use this helper function to get the XML from this binary blob..
//    XmlElement* const xmlState = getXmlFromBinary (data, sizeInBytes);
//
//    if (xmlState != 0)
//    {
//        // check that it's the right type of xml..
//        if (xmlState->hasTagName (T("MYPLUGINSETTINGS")))
//        {
//            // ok, now pull out our parameters..
//            gain = (float) xmlState->getDoubleAttribute (T("gainLevel"), gain);
//
//            lastUIWidth = xmlState->getIntAttribute (T("uiWidth"), lastUIWidth);
//            lastUIHeight = xmlState->getIntAttribute (T("uiHeight"), lastUIHeight);
//
//            sendChangeMessage (this);
//        }
//
//        delete xmlState;
//    }
   
   // count the zoneslots in the loaded file
//   jassert(zoneslotCount == 0);
//   for (zoneslotCount=0; zoneslotCount<NUM_PROGRAMS; zoneslotCount++)
//      if (getHProgramRef(zoneslotCount).num_zones <= 0)
//         break;

   sendChangeMessage(this);
}