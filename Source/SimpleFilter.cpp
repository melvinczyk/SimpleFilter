//
//  SimpleFilter.cpp
//  SimpleFilter - VST3
//
//  Created by Nicholas Burczyk 1 on 2/7/24.
//  Copyright © 2024 melvinczyk. All rights reserved.
//

#include "SimpleFilter.hpp"

void SimpleFilter::setHighPass(bool highPass)
{
    this->highPass = highPass;
}

void SimpleFilter::setCuttoff(<#float cuttoffFreq#>)
{
    this->cuttoffFreq = cuttoffFreq;
}

void SimpleFilter::setSampleRate(float sampleRate)
{
    this->sampleRate = sampleRate;
}

void SimpleFilter::processBlock(juce::AudioBuffer<float> & buffer, juce::MidiBuffer &)
{
    dnBuffer.resize(buffer.getNumChannels(), 0.f);
    const auto sign = highPass ? -1.f : 1.f;
    
    const auto tan = std::tan(PI * cuttoffFreq / sampleRate);
    
    const auto a1 = (tan - 1.f) / (tan + 1.f);
    
    for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto channelSamples = buffer.getWritePointer(channel);
        for (auto i = 0; i < buffer.getNumSamples(); ++i)
        {
            const auto inputSample = channelSamples[i];
            
            // allpass
            const auto allpassFilteredSample = a1 * inputSample + dnBuffer[channel];
            
            // TODO: channel assigning and output level
        }
    }
}
