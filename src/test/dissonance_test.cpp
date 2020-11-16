
#include "../sounds/SignalSource.h"
#include "../sounds/track/Track.h"
#include "../sounds/extractors/Dissonance.h"
#include "../sounds/window/HammingWindow.h"

#include <iostream>

using namespace sacred::sounds;

auto track = std::make_shared<sacred::sounds::Track>();
sacred::sounds::Dissonance dissonance;
sacred::sounds::HammingWindow window;

void printSampleFrameMono(SampleFrame_ptr frame){
    using namespace sacred::sounds;
    if (!frame->stereo){
        /*for (int n=0; n<SignalSource::FRAME_SIZE; ++n){
            std::cout << "\t\t\t\t\t\t" << frame.samples_left[n] << "\t\t\t\t\t\t" << std::endl;
        }*/
        std::cout << "\t\t\t\t\t\t" << frame->samples[0][0] << "\t\t\t\t\t\t" << std::endl;
    }else{
        std::cerr << "Expected MONO frame, got STEREO (printing)" << std::endl;
    }
}

void printSampleFrameStereo(SampleFrame_ptr frame){
    using namespace sacred::sounds;
    if (frame->stereo){
        /*for (int n=0; n<SignalSource::FRAME_SIZE; ++n){
            std::cout << frame.samples_left[n] << "\t\t\t\t\t\t" << frame.samples_right[n] << std::endl;
        }*/
        std::cout << frame->samples[0][0] << "\t\t\t\t\t\t" << frame->samples[1][0] << std::endl;
    }else{
        std::cerr << "Expected STEREO frame, got MONO (printing)" << std::endl;
    }
}

int main(int argc, char* argv[]){
    using namespace sacred::sounds;

    if (argc != 2){
        std::cerr << "Usage : $ ./program audiofilepath" << std::endl;
    }

    std::string audioFilePath(argv[1]);

    track->setFilePath(audioFilePath);
    //dissonance.setWindow(window);

    dissonance.addInput(track);

    SampleFrame_ptr frame;
    bool stereo = track->isStereo();
    dissonance.setStereo(stereo);
    int frameNb = 0;

    if (stereo){
        std::cout << "STEREO" << std::endl << std::endl;
        std::cout << "LEFT"<< "\t\t\t\t\t\t"<< "RIGHT" << std::endl << std::endl;
        while (!dissonance.isFinished()){
            printSampleFrameStereo(dissonance.getProcessedFrame(++frameNb));
        }
        std::cout << "<3 ~~~~~~ <3 Finished <3 ~~~~~~ <3" << std::endl;
    }
    else{
        std::cout << "MONO" << std::endl << std::endl;
        while (!dissonance.isFinished()){
            printSampleFrameMono(dissonance.getProcessedFrame(++frameNb));
        }
        std::cout << "<3 ~~~~~~ <3 Finished <3 ~~~~~~ <3" << std::endl;
    }



    return 0;
}
