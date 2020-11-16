#ifndef SACRED_AUDIOLOADER
#define SACRED_AUDIOLOADER

/**
 * @file AudioLoader.h
 * @brief Declares the AudioLoader class and objects closely related
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include <al.h>
#include <string>
#include <sndfile.h>

#include "../SignalSource.h"

namespace sacred
{
    namespace sounds
    {
        /**
         * @struct LoadedData
         * @brief The structure containing all data loaded by AudioLoader using OpenAL
         **/
        struct LoadedData
        {
            /** The audio file samples for the left channel **/
            SampleType *samples_left;
            
            /** The audio file samples for the right channel **/
            SampleType *samples_right;
            
            /** The number of samples in the audio file **/
            std::size_t samplesNb;
            
            /** The OpenAL buffer ID **/
            ALuint buffer;
            
            /** The information concerning the audio file **/
            SF_INFO fileInfos;
        };

        /**
         * @class AudioLoader
         * @brief The class that loads all the data from an audio file
         **/
        class AudioLoader
        {
            public:
                AudioLoader();
                virtual ~AudioLoader();

                /**
                 * @fn bool loadSound(const std::string& filename, LoadedData& loadedData)
                 * @brief Loads the data into the LoadedData struct
                 * @param filename : Name of the audio file
                 * @param loadedData : The struct to put data in
                 * @return success or not
                 * 
                 * works only for 16 PCM wav files
                 **/
                bool loadSound(const std::string& filename, LoadedData& loadedData);

                
                /**
                 * @fn void extractMonoSamples(LoadedData& loadedData, ALshort* samples)
                 * @brief   Fills the LoadedData struct with the samples returned by OpenAL
                 *          Mono version
                 * @param loadedData : The structure to fill with samples
                 * @param samples : The samples returned by OpenAL
                 **/
                void extractMonoSamples(LoadedData& loadedData, ALshort* samples);
                
                /**
                 * @fn void extractMonoSamples(LoadedData& loadedData, ALshort* samples)
                 * @brief   Fills the LoadedData struct with the samples returned by OpenAL
                 *          Stereo version
                 * @param loadedData : The structure to fill with samples
                 * @param samples : The samples returned by OpenAL
                 **/
                void extractStereoSamples(LoadedData& loadedData, ALshort* samples);
        };
    }
}

#endif
