#ifndef READER_H
#define READER_H
#include <string>
#include <iostream>
#include <fstream>

class Reader{
    public:
    virtual ssize_t read(char* buffer, uint len)=0;
    virtual bool open(std::string name)=0;
    virtual void close()=0;
};

class Mock_reader: public Reader{
    public:
    Mock_reader(){};
    ~Mock_reader();
    ssize_t read(char* buffer, uint len);
    bool open(std::string name);
    void close();
    private:
    std::fstream stream_;
};

#endif