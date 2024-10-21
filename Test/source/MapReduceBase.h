#pragma once

#include <string>

class MapReduceBase
{
public:
    virtual std::vector<std::pair<std::string, int>> Map(std::string& fileName, std::string& offset) = 0;
    virtual void Reduce(std::vector<std::string>& keys, std::vector<int>& values) = 0;
};