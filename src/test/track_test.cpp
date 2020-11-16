
#include "../sounds/SignalSource.h"
#include "../sounds/track/Track.h"

#include <iostream>


using namespace sacred::sounds;

Track track;

void printSampleFrameMono(SampleFrame_ptr frame){

    if (!frame->stereo){
        for (int n=0; n<FRAME_SIZE; ++n){
            std::cout << "\t\t\t\t\t\t" << frame->samples[0][n] << "\t\t\t\t\t\t" << std::endl;
        }
    }else{
        std::cerr << "Expected MONO frame, got STEREO (printing)" << std::endl;
    }
}

void printSampleFrameStereo(SampleFrame_ptr frame){

    if (frame->stereo){
        for (int n=0; n<FRAME_SIZE; ++n){
            std::cout << frame->samples[0][n] << "\t\t\t\t\t\t" << frame->samples[1][n] << std::endl;
        }
    }else{
        std::cerr << "Expected STEREO frame, got MONO (printing)" << std::endl;
    }
}

int main(int argc, char* argv[]){

    if (argc != 2){
        std::cerr << "Usage : $ ./program audiofilepath" << std::endl;
    }

    std::string audioFilePath(argv[1]);

    track.setFilePath(audioFilePath);

    int frameNb = 0;
    SampleFrame_ptr frame = track.getProcessedFrame(++frameNb);
    bool stereo = track.isStereo();

    if (stereo){
        std::cout << "STEREO" << std::endl << std::endl;
        std::cout << "LEFT"<< "\t\t\t\t\t\t"<< "RIGHT" << std::endl << std::endl;
        while (!track.isFinished()){
            printSampleFrameStereo(frame);
            track.getProcessedFrame(++frameNb);
        }
        std::cout << "<3 ~~~~~~ <3 Finished <3 ~~~~~~ <3" << std::endl;
    }
    else{
        std::cout << "MONO" << std::endl << std::endl;
        while (!track.isFinished()){
            printSampleFrameMono(frame);
            track.getProcessedFrame(++frameNb);
        }
        std::cout << "<3 ~~~~~~ <3 Finished <3 ~~~~~~ <3" << std::endl;
    }


    return 0;
}
