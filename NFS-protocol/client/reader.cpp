#include "utils.h"
#include "reader.h"
#include <iostream>
#include <fstream>
#include <string.h>



ssize_t Mock_reader::read(char* buffer, uint len){
    if(len == 0){
        return true;
    }
    stream_.read(buffer, len);
    return stream_.gcount();
}  

bool Mock_reader::open(std::string file_name){
    if(stream_.is_open()){
        return false;
    }
    stream_.open(file_name, std::ios::in);

    return stream_.good();
}

void Mock_reader::close(){
    if(stream_.is_open()){
        stream_.close();
    }
}

Mock_reader::~Mock_reader(){
    stream_.close();
}