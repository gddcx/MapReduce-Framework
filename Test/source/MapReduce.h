#pragma once
#include <vector>
#include <utility>
#include <string>
#include "MapReduceBase.h"

class MapReduce: public MapReduceBase
{
public:
    std::vector<std::pair<std::string, int>> Map(std::string& fileName, std::string& offset) override;
    bool Reduce(std::string& key, std::vector<std::pair<std::string, int>>& value) override;
    void MapCombiner() override;
};