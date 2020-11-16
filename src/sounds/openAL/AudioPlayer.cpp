/**
 * @file AudioPlayer.cpp
 * @brief Defines the AudioPlayer class methods
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "AudioPlayer.h"

#include <al.h>
#include <alc.h>

namespace sacred
{
    namespace sounds
    {
        AudioPlayer::AudioPlayer()
        {
        }

        AudioPlayer::~AudioPlayer()
        {
        }

        void AudioPlayer::play(ALuint buffer)
        {
            // Création d'une source
            ALuint source;
            alGenSources(1, &source);

            // On attache le tampon contenant les échantillons audio à la source
            alSourcei(source, AL_BUFFER, buffer);

            // Lecture du son
            alSourcePlay(source);

            ALint status;
            do
            {
                // Récupération de l'état du son
                alGetSourcei(source, AL_SOURCE_STATE, &status);
            }
            while(status == AL_PLAYING);

            // Destruction du tampon
            alDeleteBuffers(1, &buffer);

            // Destruction de la source
            alSourcei(source, AL_BUFFER, 0);
            alDeleteSources(1, &source);
        }
    }
}
