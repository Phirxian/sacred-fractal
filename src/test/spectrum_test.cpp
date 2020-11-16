
#include "../sounds/SignalSource.h"
#include "../sounds/window/Window.h"
#include "../sounds/window/HammingWindow.h"
#include "../sounds/track/Track.h"
#include "../sounds/transform/Spectrum.h"

#include <vector>
#include <iostream>

sacred::sounds::Track track;
sacred::sounds::Spectrum spectrum;
sacred::sounds::HammingWindow window;
std::vector<sacred::sounds::SampleType> frequencies;
std::vector<sacred::sounds::SampleType> magnitudes_left;
std::vector<sacred::sounds::SampleType> magnitudes_right;

void printSpectrum_stereo(){
    auto it_f = frequencies.begin();
    auto it_mag_left = magnitudes_left.begin();
    auto it_mag_right = magnitudes_right.begin();

    while (it_f != frequencies.end()){
        std::cout << *(it_f++) << "\t\t" << *(it_mag_left++) << "\t\t" << *(it_mag_right++) << std::endl;
    }
}

void printSpectrum_mono(){
    auto it_f = frequencies.begin();
    auto it_mag_left = magnitudes_left.begin();

    while (it_f != frequencies.end()){
        std::cout << *(it_f++) << "\t\t" << *(it_mag_left++) << std::endl;
    }
}

int main(int argc, char* argv[]){

    using namespace sacred::sounds;

    if (argc != 2){
        std::cerr << "Usage : $ ./program audiofilepath" << std::endl;
    }

    std::string audioFilePath(argv[1]);

    track.setFilePath(audioFilePath);

    //spectrum.setWindow(window);

    SampleFrame_ptr frame;
    int frameNb(1);

    if (track.isStereo()){
        std::cout << "STEREO" << std::endl << std::endl;
        while(!track.isFinished()){
            frame = track.getProcessedFrame(frameNb++);
            for(int i=0; i<FRAME_SIZE; ++i){
                spectrum.pushSample(frame->samples[0][i], false);
                spectrum.pushSample(frame->samples[1][i], true);
            }
            frequencies = spectrum.getFrequencies();
            magnitudes_left = spectrum.getMagnitudes(false);
            magnitudes_right = spectrum.getMagnitudes(true);

            printSpectrum_stereo();

            std::cout << std::endl << "------------ NEW SPECTRUM ------------" << std::endl;
        }
    }
    else{
        std::cout << "MONO" << std::endl << std::endl;
        while(!track.isFinished()){
            frame = track.getProcessedFrame(frameNb++);
            for(int i=0; i<FRAME_SIZE; ++i){
                spectrum.pushSample(frame->samples[0][i], false);
                spectrum.pushSample(frame->samples[1][i], true);
            }
            frequencies = spectrum.getFrequencies();
            magnitudes_left = spectrum.getMagnitudes(false);

            printSpectrum_mono();

            std::cout << std::endl << "------------ NEW SPECTRUM ------------" << std::endl;
        }

    }


    std::cout << "<3 ~~~~~~ <3 Finished <3 ~~~~~~ <3" << std::endl;

    return 0;
}
