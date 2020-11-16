/**
 * @file AudioLoader.cpp
 * @brief Defines the AudioLoader class methods
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "AudioLoader.h"

#include <al.h>
#include <alc.h>
#include <sndfile.h>
#include <vector>
#include <string>
#include <cstdlib>

#include <iostream>

namespace sacred
{
    namespace sounds
    {
        AudioLoader::AudioLoader()
        {
        }

        AudioLoader::~AudioLoader()
        {
        }

        bool AudioLoader::loadSound(const std::string& filename, LoadedData& loadedData)
        {

            // Ouverture du fichier audio avec libsndfile
            SF_INFO &fileInfos = loadedData.fileInfos;
            SNDFILE* file = sf_open(filename.c_str(), SFM_READ, &fileInfos);
            if(!file)
            {
                std::cerr << "error during sf_open" << std::endl;
                return false;
            }

            // Lecture du nombre d'échantillons et du sample rate
            ALsizei samplesNb  = static_cast<ALsizei>(fileInfos.channels * fileInfos.frames);
            ALsizei sampleRate = static_cast<ALsizei>(fileInfos.samplerate);


            ALshort *shortSamples = (short*) calloc(samplesNb+2, sizeof(short));;

            // Lecture des échantillons audio au format entier 16 bits signé (le plus commun)
            if(sf_read_short(file, shortSamples, samplesNb) < samplesNb)
            {
                std::cerr << "error during sf_read_short" << std::endl;
                return false;
            }

            // Fermeture du fichier
            sf_close(file);

            // Détermination du format en fonction du nombre de canaux
            ALenum format;
            switch(fileInfos.channels)
            {
                case 1 :
                {
                    format = AL_FORMAT_MONO16;
                    loadedData.samplesNb = samplesNb;
                    extractMonoSamples(loadedData, shortSamples);
                }
                break;
                case 2 :
                {
                    format = AL_FORMAT_STEREO16;
                    loadedData.samplesNb = samplesNb/2;
                    extractStereoSamples(loadedData, shortSamples);
                }
                break;
                default :
                {
                    std::cerr << "error : wrong file format" << std::endl;
                    return false;
                }
                break;
            }

            // Création du tampon OpenAL
            ALuint buffer;
            alGenBuffers(1, &buffer);

            // Remplissage avec les échantillons lus
            alBufferData(buffer, format, shortSamples, samplesNb * sizeof(ALshort), sampleRate);

            // Vérification des erreurs
            int error_state = alGetError();
            std::string errmsg;
            if(error_state != AL_NO_ERROR)
            {
                switch(error_state)
                {
                    case AL_INVALID_NAME:
                        errmsg = "AL_INVALID_NAME";
                        break;
                    case AL_INVALID_ENUM:
                        errmsg = "AL_INVALID_ENUM";
                        break;
                    case AL_INVALID_VALUE:
                        errmsg = "AL_INVALID_VALUE";
                        break;
                    case AL_INVALID_OPERATION :
                        errmsg = "AL_INVALID_OPERATION";
                        break;
                    case AL_OUT_OF_MEMORY:
                        errmsg = "AL_OUT_OF_MEMORY";
                        break;
                    default:
                        errmsg = "NOT A STANDARD ERROR";
                        break;

                }
                std::cerr << "error from alGetError : "<< errmsg << std::endl;
                return false;
            }

            loadedData.buffer = buffer;

            return true;
        }

        void AudioLoader::extractMonoSamples(LoadedData& loadedData, ALshort* samples)
        {
            loadedData.samples_left = (SampleType*) calloc(loadedData.samplesNb+1, sizeof(SampleType));

            for(ALsizei n=0; n<loadedData.samplesNb; ++n)
                loadedData.samples_left[n] = SampleType(samples[n] * SampleType(1./32768));
        }

        void AudioLoader::extractStereoSamples(LoadedData& loadedData, ALshort* samples)
        {
            loadedData.samples_left = (SampleType*) calloc(loadedData.samplesNb+1, sizeof(SampleType));
            loadedData.samples_right = (SampleType*) calloc(loadedData.samplesNb+1, sizeof(SampleType));

            for(ALsizei n=0; n<loadedData.samplesNb; ++n)
            {
                loadedData.samples_left[n] = SampleType(samples[n*2] * SampleType(1./32768));
                loadedData.samples_right[n] = SampleType(samples[n*2 +1] * SampleType(1./32768));
            }
        }
    }
}
