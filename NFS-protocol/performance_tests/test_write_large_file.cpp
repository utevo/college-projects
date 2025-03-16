#include "../libraries/client/lib.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <math.h>
#include <numeric>
#include <algorithm>
#include <sys/stat.h>
#include <fcntl.h>

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

    const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    char buff[2 * 1024 * 1024];
    for (int i = 0; i < 2 * 1024 * 1024; ++i) {
        buff[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    NFSClient client;
    client.connect4(argv[2], 9000, argv[1], argv[3]);

    auto beg = std::chrono::high_resolution_clock::now();
    auto fd = client.open(argv[4], O_WRONLY, 0);


        client.write(fd, buff , 2 * 1024 * 1024);


    client.close(fd);
    auto end = std::chrono::high_resolution_clock::now();
    double time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - beg).count();


    std::cout<<"time: "+std::to_string(time_taken/1e9)<<std::endl;
    return 0;
}
