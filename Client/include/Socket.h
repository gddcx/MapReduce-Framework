#pragma once

#include <string>
using namespace std;

class Socket
{
private:
    int fd_ = 0;
    int BuildConnection();
    void Read(string filename);
    void Send();
public:
    void UploadFile(string filename);
};