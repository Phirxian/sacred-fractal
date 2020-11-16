#include "../common/print.h"
#include "../common/json.hpp"

#include "../../sounds/generator/SquareGenerator.h"
#include "../../sounds/generator/SineGenerator.h"
#include "../../sounds/operators/SignalsSubstractor.h"

using namespace sacred::sounds;
using namespace test;

int main(int argc, char* argv[])
{
    static const unsigned int MAXIFRAME = {2};
    static const char *strings[2] = { "false", "true" };
    
    auto src1 = std::make_shared<SineGenerator>(100.0,1.0,0.0, false);
    auto src2 = std::make_shared<SquareGenerator>(100.0,1.0,0.0, false);
    
    SignalsSubstractor gen;
    gen.addInput(src1);
    gen.addInput(src2);
    
    std::cout << std::fixed;
    
    std::cout << "{" << std::endl;
    std::cout << " \"stereo\" : " << strings[gen.isStereo()] << ", " << std::endl;
    std::cout << " \"count\" : " << MAXIFRAME << ", " << std::endl;
    std::cout << " \"size\" : " << FRAME_SIZE << ", " << std::endl;
    std::cout << " \"type\" : \"scatter\", " << std::endl;
    std::cout << " \"frames\" : [" << std::endl;
                     
    std::cout << gen.getProcessedFrame(1);
    for(unsigned int i = 1; i<MAXIFRAME; ++i)
        std::cout << ", " << std::endl << gen.getProcessedFrame(i+1);
    
    std::cout << std::endl << " ]" << std::endl << "}" << std::endl;
    
    return 0;
}
