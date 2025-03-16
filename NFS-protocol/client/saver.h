#ifndef SAVER_H
#define SAVER_H
#include <string>
#include <iostream>
#include <fstream>

class Saver{
    public:
    virtual ~Saver(){};
    virtual bool save(char* buffer, uint len)=0;
    virtual bool open(std::string name)=0;
    virtual void close()=0;
};

class Mock_saver: public Saver{
    public:
    Mock_saver():name_(std::string("")){};
    bool save(char* buffer, uint len);
    bool open(std::string name);
    void close();
    private:
    std::string name_;
};

class FSSaver: public Saver{
public:
    FSSaver();
    bool save(char* buffer, uint len);
    bool open(std::string name);
    void close();

    ~FSSaver();
private:
    std::fstream* s;
};

#endif