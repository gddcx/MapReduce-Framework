#include <iostream>
#include <string>
#include "WorkerNode.h"

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "Node name of Worker is need" << std::endl;
        return -1;
    }

    std::string target = "127.0.0.1:50031";
    WorkerNode workerNode;

    workerNode.SetRpcServer(target);
    workerNode.SetNodeName(std::string(argv[1]));
    std::string libFile = "/home/dengchangxing/projects/MapReduceFramework/Test/scripts/MapReduce.so";
    workerNode.StartWorkerNode(libFile);

    return 0;
}