


#include "../sounds/SignalSource.h"
#include "../sounds/generator/TriangleGenerator.h"
#include "../sounds/extractors/Dynamics.h"

#include <iostream>

unsigned int IFRAME{0};
unsigned int MAXIFRAME{100};

using namespace sacred::sounds;

void printSampleFrameMono(SampleFrame_ptr frame){
    using namespace sacred::sounds;
    if (!frame->stereo){
        for (int n=0; n<FRAME_SIZE; ++n){
            std::cout << "\t\t\t\t\t\t" << frame->samples[0][n] << "\t\t\t\t\t\t" << std::endl;
        }
    }else{
        std::cerr << "Expected MONO frame, got STEREO (printing)" << std::endl;
    }
}

void printSampleFrameStereo(SampleFrame_ptr frame){
    using namespace sacred::sounds;
    if (frame->stereo){
        for (int n=0; n<FRAME_SIZE; ++n){
            std::cout << frame->samples[0][n] << /*"\t\t\t\t\t\t" << frame->samples[1][n] << */std::endl;
        }
    }else{
        std::cerr << "Expected STEREO frame, got MONO (printing)" << std::endl;
    }
}

int main(int argc, char* argv[]){
    using namespace sacred::sounds;

    auto triGen = std::make_shared<TriangleGenerator>(20.0,1.0,0.0, true);
    Dynamics dynamics(true, 0.9, 0.1, 0.001, 0.005);

    dynamics.addInput(triGen);

    SampleFrame_ptr frame;
    bool stereo = dynamics.isStereo();
    int frameNb = 0;

    if (stereo){
        //std::cout << "STEREO" << std::endl << std::endl;
        //std::cout << "LEFT"<< "\t\t\t\t\t\t"<< "RIGHT" << std::endl << std::endl;
        while (IFRAME++ < MAXIFRAME){
            printSampleFrameStereo(dynamics.getProcessedFrame(++frameNb));
        }
        //std::cout << "<3 ~~~~~~ <3 Finished <3 ~~~~~~ <3" << std::endl;
    }
    else{
        //std::cout << "MONO" << std::endl << std::endl;
        while (IFRAME++ < MAXIFRAME){
            printSampleFrameMono(dynamics.getProcessedFrame(++frameNb));
        }
        //std::cout << "<3 ~~~~~~ <3 Finished <3 ~~~~~~ <3" << std::endl;
    }



    return 0;
}
