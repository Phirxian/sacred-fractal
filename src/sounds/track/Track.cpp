/**
 * @file Track.cp
 * @brief Defines the Track class methods
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "Track.h"
#include "../SignalSource.h"

#include <cstring>
#include <iostream>

namespace sacred
{
    namespace sounds
    {
        Track::Track()
            : SignalSource(false, 0), m_muted(false)
        {
        }

        Track::~Track()
        {
        }

        void Track::setMuted(bool muted)
        {
            m_muted = muted;
        }

        bool Track::isMuted() const
        {
            return m_muted;
        }

        void Track::process() noexcept
        {
            unsigned short delta = std::min(m_remainingSamplesCount, FRAME_SIZE);

            #pragma omp parallel for
            for(int i = 0; i<=m_stereo; ++i)
            {
                std::memcpy(m_frame->samples[i], m_remainingSamples[i], sizeof(SampleType)*delta);
                if(delta != FRAME_SIZE)
                    std::memset(m_frame->samples[i]+delta, 0.0, sizeof(SampleType)*(FRAME_SIZE-delta));
                m_remainingSamples[i] += delta;
            }

            m_remainingSamplesCount -= delta;
        }

        void Track::setFilePath(std::string filePath)
        {
            m_filePath = filePath;
            LoadedData loadedData;

            if(m_audioLoader.loadSound(filePath, loadedData))
            {
                m_remainingSamplesCount = loadedData.samplesNb;
                m_buffer = loadedData.buffer;

                m_fullSamples[0] = loadedData.samples_left;
                m_remainingSamples[0] = loadedData.samples_left;

                if(loadedData.fileInfos.channels == 2)
                {
                    m_fullSamples[1] = loadedData.samples_right;
                    m_remainingSamples[1] = loadedData.samples_right;
                    setStereo(true);
                }

                setStereo(loadedData.fileInfos.channels == 2);
            }
            else
                std::cerr << "Problem while loading the file " << filePath << std::endl;
        }

        bool Track::isFinished() const
        {
            return m_remainingSamplesCount == 0;
        }
    }
}
