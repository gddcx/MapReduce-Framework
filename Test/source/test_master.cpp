#include <string>
#include <stack>

#include "MasterNode.h"

int main(int argc, char* argv[])
{
    std::string startPos = "0";
    std::string filename;
    MasterNode masterNode(std::string ("0.0.0.0:50031"));

    for(int loop = 1; loop < argc; loop++)
    {
        filename = std::string(argv[loop]);
        masterNode.AddJob(filename, startPos);
    }
    masterNode.StartMasterNode();

    return 0;
}