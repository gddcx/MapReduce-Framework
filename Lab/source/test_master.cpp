#include <string>
#include <stack>

#include "MasterNode.h"

int main(int argc, char* argv[])
{
    std::string filename;
    std::string rpcListen("0.0.0.0:50031");
    MasterNode masterNode;

    for(int loop = 1; loop < argc; loop++)
    {
        filename = std::string(argv[loop]);
        masterNode.AddJob(filename);
    }
    masterNode.StartMasterNode(rpcListen);

    return 0;
}