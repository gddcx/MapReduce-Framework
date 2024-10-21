#pragma once
#include <vector>
#include <utility>
#include <string>
#include "MapReduceBase.h"

class MapReduce: public MapReduceBase
{
public:
    std::vector<std::pair<std::string, int>> Map(std::string& fileName, std::string& offset) override;
    void Reduce(std::vector<std::string>& keys, std::vector<int>& values) override;
};