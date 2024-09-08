#pragma once

#include <string>
#include "public.h"

class MapReduceBase
{
public:
    virtual std::vector<std::pair<std::string, int>> Map(std::string& fileName, std::string& offset) = 0;
    virtual bool Reduce(std::string& key, std::vector<std::pair<std::string, int>>& value) = 0;
    virtual void MapCombiner() { return; }
};