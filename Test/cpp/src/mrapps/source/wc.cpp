#include "wc.h"
#include <iostream>

using std::string;
using std::vector;
using std::pair;

vector<pair<string, char>> Map(const string& filename, const string& contents)
{
    vector<pair<string, char>> res;
    string::const_iterator end = contents.begin();
    while(!std::isalpha(*end)) end++;
    string::const_iterator start = end;

    while(end != contents.end())
    {
        while(end != contents.end() && std::isalpha(*end)) end++;
        res.emplace_back(string(start, end), '1');
        while(end != contents.end() && !std::isalpha(*end)) end++;
        start = end;
    }

    return res;
}

string Reduce(const string& key, const vector<char>& values)
{
    return std::to_string(values.size());
}