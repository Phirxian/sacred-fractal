#ifndef SACRED_AUDIODEVICEMANAGER
#define SACRED_AUDIODEVICEMANAGER

/**
 * @file AudioDeviceManager.h
 * @brief Declares the AudioDeviceManager class
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include <vector>
#include <string>

namespace sacred
{
    namespace sounds
    {
        /**
         * @class AudioDeviceManager
         * @brief The class that manages audio devices to play audio using OpenAl
         **/
        class AudioDeviceManager
        {
            public:
                AudioDeviceManager();
                virtual ~AudioDeviceManager();
            private:
                void getDevices(std::vector<std::string>& devices) const;
        };
    }
}

#endif
