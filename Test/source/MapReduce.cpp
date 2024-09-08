#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <map>
#include "MapReduce.h"

std::vector<std::pair<std::string, int>> MapReduce::Map(std::string& key, std::string& value)
{
    std::ifstream fd;
    std::string context;
    std::stringstream buffer;
    fd.open(key, std::ios::in|std::ios::binary);
    buffer << fd.rdbuf();
    context = buffer.str();

    std::string word;
    std::map<std::string, int> mapRes;
    std::string::const_iterator start = context.begin();
    std::string::const_iterator end = context.begin();
    while(end != context.end() && !std::isalpha(*end)) end++;
    start = end;

    while(end != context.end())
    {
        while(end != context.end() && std::isalpha(*end)) end++;
        word = std::string(start, end);
        if(mapRes.find(word) != mapRes.end())
        {
            mapRes[word]+=1;
        }
        else
        {
            mapRes.emplace(word, 1);
        }
        while(end != context.end() && !std::isalpha(*end)) end++;
        start = end;
    }

    return std::vector<std::pair<std::string, int>>(mapRes.begin(), mapRes.end());
}

bool MapReduce::Reduce(std::string& key, std::vector<std::pair<std::string, int>>& value)
{
    
}
