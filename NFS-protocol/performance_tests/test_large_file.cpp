#include "../libraries/client/lib.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <math.h>
#include <numeric>
#include <sys/stat.h>
#include <fcntl.h>
#include <algorithm>

int main(int argc, char* argv[]){
    /**
     * arg [1] = user
     * arg [2] = address
     * arg [3] = passwd
     * arg [4] = file_name.txt
     */
    if(argc != 5){
        std::cout<<"Usage: "+std::string(argv[0])+" <user> <address> <passwd> <file_name>\n";
        return -1;
    }

    const int repeats = 1024;

    std::vector<double> times;
    times.reserve(repeats);

    char buff[1024];

    NFSClient client;
    client.connect4(argv[2], 9000, argv[1], argv[3]);

    auto beg = std::chrono::high_resolution_clock::now();
    auto fd = client.open(argv[4], O_RDONLY, 0);

    unsigned int num_frames = 0;

    long len=0;
    do{
        len = client.read(fd, buff, 1024);
        ++num_frames;
    }while(len!=0);

    client.close(fd);
    auto end = std::chrono::high_resolution_clock::now();
    double time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - beg).count(); 
        

    std::cout<<"time: "+std::to_string(time_taken/1e9)<<std::endl;
    std::cout<<"num_frames: "<<num_frames<<std::endl;

    return 0;
}