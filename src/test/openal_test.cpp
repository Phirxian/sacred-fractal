#include "../sounds/openAL/AudioDeviceManager.h"
#include "../sounds/openAL/AudioLoader.h"
#include "../sounds/openAL/AudioPlayer.h"

#include <al.h>
#include <alc.h>

#include <iostream>
#include <string>
#include <thread>


void printMono(sacred::sounds::LoadedData loadedData){
    for (int n=0; n<loadedData.samplesNb; ++n){
        std::cout << "\t\t\t\t\t\t" << loadedData.samples_left[n] << "\t\t\t\t\t\t" << std::endl;
    }
}

void printStereo(sacred::sounds::LoadedData loadedData){
    for (int n=0; n<loadedData.samplesNb; ++n ){
        std::cout << loadedData.samples_left[n] << "\t\t\t\t\t\t" << loadedData.samples_right[n] << std::endl;
    }
}

void printLoadedData(sacred::sounds::LoadedData loadedData){
    switch (loadedData.fileInfos.channels){
        case 1 : {
            std::cout << "MONO" << std::endl << std::endl;
            printMono(loadedData);
            break;
        }
        case 2 : {
            std::cout << "STEREO" << std::endl << std::endl; 
            std::cout << "LEFT"<< "\t\t\t\t\t\t"<< "RIGHT" << std::endl << std::endl;
            printStereo(loadedData);
            break;
        }
    }
}


int main(int argc, char* argv[]){
    
    if (argc != 2){
        std::cerr << "Usage : $ ./program audiofilepath" << std::endl;
    }
    
    std::string audioFilePath(argv[1]);
    
    using namespace sacred::sounds;
    
    AudioDeviceManager manager;
    AudioLoader loader;
    AudioPlayer player;
    
    LoadedData loadedData;
    
    if (loader.loadSound(audioFilePath, loadedData)){
        printLoadedData(loadedData);
        player.play(loadedData.buffer);
    }
    
    return 0;
    
}


