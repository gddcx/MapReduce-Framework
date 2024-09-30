#pragma once

#include <string>
#include "public.h"

class MapReduceBase
{
public:
    virtual std::vector<std::pair<std::string, int>> Map(std::string& fileName, std::string& offset) = 0;
    virtual bool Reduce(std::vector<std::string>& keys, std::vector<int>& values) = 0;
    virtual void MapCombiner() { return; }
};