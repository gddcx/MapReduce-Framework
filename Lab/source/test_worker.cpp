#include <string>
#include "WorkerNode.h"
#include "public.h"

int main()
{
    std::string target = "127.0.0.1:50031";
    WorkerNode workerNode;

    workerNode.CreateRpcClient(target);
    workerNode.StartWorkerNode();

    return 0;
}