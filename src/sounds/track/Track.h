#ifndef __SACRED_TRACK__
#define __SACRED_TRACK__

/**
 * @file Track.h
 * @brief Declares the Track class
 * 
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "../SignalSource.h"
#include "../openAL/AudioDeviceManager.h"
#include "../openAL/AudioLoader.h"
#include "../openAL/AudioPlayer.h"

#include "al.h"

#include <string>
#include <iostream>
#include <cstdlib>

namespace sacred
{
    namespace sounds
    {
        /**
         * @class Track
         * @brief The SignalSource class that reads an audio track and creates the corresponding SampleFrames
         **/
        class Track : public SignalSource
        {
            public:
                Track();
                virtual ~Track();

                /**
                 * @fn void setFilePath(std::string filePath)
                 * @brief Sets the path of the audio file to read
                 * @param filePath : Path of the file
                 **/
                void setFilePath(std::string filePath);

                /**
                 * @fn void setMuted(bool muted)
                 * @brief Mutes the track or not
                 * @param muted : enable muting
                 **/
                void setMuted(bool muted);

                /**
                 * @return Track is finished or not
                 **/
                bool isFinished() const override;
                
                /**
                 * @fn bool isMuted() const
                 * @brief Tells if the track is muted or not
                 * @return Track is muted or not
                 **/
                bool isMuted() const;

                void process() noexcept override;

            protected:
                // OpenAL tools
                // NB : manager and player should be singleton ?
                
                /** Device manager for playing audio **/
                AudioDeviceManager m_audioDeviceManager;
                
                /** Loader **/
                AudioLoader m_audioLoader;
                
                /** Player **/
                AudioPlayer m_audioPlayer;

                /** Fully loaded file samples for the left/right channel ( = all samples if MONO ) **/
                SampleType *m_fullSamples[2];
                
                /** Remaining samples in the file for the left/right channel **/
                SampleType *m_remainingSamples[2];

                /** Number of remaining samples **/
                std::size_t m_remainingSamplesCount;
                
                /** Path of the audio file **/
                std::string m_filePath;

                /** OpenAL buffer ID **/
                ALuint m_buffer;

                bool m_muted;
        };
    }
}


#endif
