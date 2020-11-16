#ifndef SACRED_AUDIOPLAYER
#define SACRED_AUDIOPLAYER

/**
 * @file AudioPlayer.h
 * @brief Declares the AudioPlayer class
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include <al.h>

namespace sacred
{
    namespace sounds
    {
        /**
         * @class AudioPlayer
         * @brief The class that play audio
         **/
        class AudioPlayer
        {
            public:
                AudioPlayer();
                virtual ~AudioPlayer();

                /**
                 * @fn void play(ALuint buffer)
                 * @brief Plays audio given the OpenAL buffer ID
                 * @param buffer : The OpenAL buffer ID
                 **/
                void play(ALuint buffer);
        };
    }
}

#endif
