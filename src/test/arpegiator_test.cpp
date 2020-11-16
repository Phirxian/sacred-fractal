



#include "../sounds/SignalSource.h"
#include "../sounds/generator/TriangleGenerator.h"
#include "../sounds/extractors/Arpegiator.h"

#include <iostream>

unsigned int IFRAME{0};
unsigned int MAXIFRAME{100};

using namespace sacred::sounds;

void printSampleFrameMono(SignalSource::SampleFrame_ptr frame){
    using namespace sacred::sounds;
    if (!frame->stereo){
        for (int n=0; n<SignalSource::FRAME_SIZE; ++n){
            std::cout << "\t\t\t\t\t\t" << frame->samples_left.get()[n] << "\t\t\t\t\t\t" << std::endl;
        }
    }else{
        std::cerr << "Expected MONO frame, got STEREO (printing)" << std::endl;
    }
}

void printSampleFrameStereo(SignalSource::SampleFrame_ptr frame){
    using namespace sacred::sounds;
    if (frame->stereo){
        for (int n=0; n<SignalSource::FRAME_SIZE; ++n){
            std::cout << frame->samples_left.get()[n] << /*"\t\t\t\t\t\t" << frame->samples_right.get()[n] << */std::endl;
        }
    }else{
        std::cerr << "Expected STEREO frame, got MONO (printing)" << std::endl;
    }
}

int main(int argc, char* argv[]){
    using namespace sacred::sounds;
    
    TriangleGenerator triGen(20.0,1.0,0.0, true);
    Arpegiator arpegiator(true, 0.9, Arpegiator_mode::random, -1., 1., 12);
    
    arpegiator.addInput(triGen);
    
    SignalSource::SampleFrame_ptr frame;
    bool stereo = arpegiator.isStereo();
    int frameNb = 0;
    
    if (stereo){
        //std::cout << "STEREO" << std::endl << std::endl; 
        //std::cout << "LEFT"<< "\t\t\t\t\t\t"<< "RIGHT" << std::endl << std::endl;
        while (IFRAME++ < MAXIFRAME){
            printSampleFrameStereo(arpegiator.getProcessedFrame(++frameNb));
        }
        //std::cout << "<3 ~~~~~~ <3 Finished <3 ~~~~~~ <3" << std::endl;
    }
    else{
        //std::cout << "MONO" << std::endl << std::endl;
        while (IFRAME++ < MAXIFRAME){
            printSampleFrameMono(arpegiator.getProcessedFrame(++frameNb));
        }
        //std::cout << "<3 ~~~~~~ <3 Finished <3 ~~~~~~ <3" << std::endl;
    }
    
    
    
    return 0;
}
