#include <string>
#include <stack>

#include "MasterNode.h"

int main(int argc, char* argv[])
{
    std::vector<std::string> filenames;
    MasterNode masterNode(std::string ("0.0.0.0:50031"));

    for(int loop = 1; loop < argc; loop++)
    {
        filenames.emplace_back(std::string(argv[loop]));
    }
    std::vector<std::string> startPos(filenames.size(), "0");
    masterNode.AddJob(filenames, startPos, 2);
    masterNode.StartMasterNode();

    return 0;
}