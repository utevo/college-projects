#include "../libraries/client/lib.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <numeric>
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

    for(int i = repeats; i >0; --i){
        auto beg = std::chrono::high_resolution_clock::now();

        auto fd = client.open(argv[4], O_RDONLY, 0);
        client.read(fd, buff, 1024);
        client.close(fd);

        auto end = std::chrono::high_resolution_clock::now();
        double time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - beg).count();
        times.push_back(time_taken / (double)1e9);
    }

    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    double mean = sum / times.size();

    double var = 0.0;
    for(auto t: times){
        var += (t-mean)*(t-mean);
    }

    std::cout<<
    std::string("times:")+
    "\nmean: "+std::to_string(mean)+
    "\nstddev: "+std::to_string(std::sqrt(var))+
    "\nmin time: "+std::to_string(*std::min_element(times.begin(), times.end()))+
    "\nmax time: "+std::to_string(*std::max_element(times.begin(), times.end()))
    <<std::endl;

    return 0;
}