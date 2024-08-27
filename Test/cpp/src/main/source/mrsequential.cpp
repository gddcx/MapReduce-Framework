#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <algorithm>
#include <iostream>

#include "wc.h"

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::pair;

bool cmp(const pair<string, char>& a, const pair<string, char>& b)
{
    return a.first < b.first;
}


int main(int argc, char *argv[])
{
    int fileIdx;
    int fileNum = argc;
    std::stringstream buffer;
    string content = "";
    vector<pair<string, char>> intermediate;
    vector<pair<string, char>> mapRes;
    ifstream inputFile;

    for(fileIdx = 2; fileIdx < fileNum; fileIdx++)
    {
        inputFile.open(argv[fileIdx]);
        buffer << inputFile.rdbuf();
        content = buffer.str();

        mapRes = Map(string(argv[fileIdx]), content);
        intermediate.insert(intermediate.end(), mapRes.begin(), mapRes.end());

        vector<pair<string, char>>().swap(mapRes);
        buffer.str("");
        inputFile.close();
    }

    std::sort(intermediate.begin(), intermediate.end(), cmp);

    int i = 0;
    int j = 0;
    int k = 0;
    vector<char> values;
    ofstream output(string(argv[1]) + "/mr-out-0");
    string reduceRes = "";

    while(i < intermediate.size())
    {
        j = i + 1;
        while(j < intermediate.size() && (intermediate[j].first == intermediate[i].first)) j++;
        for(k = i; k < j; k++)
        {
            values.push_back(intermediate[k].second);
        }
        reduceRes = Reduce(intermediate[i].first, values);
        vector<char>().swap(values);

        output << intermediate[i].first << " " << reduceRes << std::endl;
        reduceRes = "";
        i = j;
    }

    output.close();

    return 0;
}