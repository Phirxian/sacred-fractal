/**
 * @file AudioDeviceManager.cpp
 * @brief Defines the AudioDeviceManager class methods
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "AudioDeviceManager.h"

#include <al.h>
#include <alc.h>
#include <cstring>
#include <stdexcept>

namespace sacred
{
    namespace sounds
    {
        AudioDeviceManager::AudioDeviceManager()
        {
            // Ouverture du device
            ALCdevice* device = alcOpenDevice(NULL);
            if(!device)
                throw std::domain_error("cannot open device");

            // Création du contexte
            ALCcontext* context = alcCreateContext(device, NULL);
            if(!context)
                throw std::domain_error("cannot create context");

            // Activation du contexte
            if(!alcMakeContextCurrent(context))
                throw std::domain_error("cannot make the context as the current");
        }

        AudioDeviceManager::~AudioDeviceManager()
        {
            ALCcontext* context = alcGetCurrentContext();
            ALCdevice*  device  = alcGetContextsDevice(context);

            alcMakeContextCurrent(nullptr);
            alcDestroyContext(context);
            alcCloseDevice(device);
        }

        void AudioDeviceManager::getDevices(std::vector<std::string>& devices) const
        {
            const ALCchar *lists = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
            devices.clear();

            while(lists)
            {
                devices.push_back(lists);
                lists += strlen(lists) + 1;
            }
        }
    }

}
