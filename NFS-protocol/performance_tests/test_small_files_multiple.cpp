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

int func(std::string user, std::string address, std::string file_name, std::vector<double>& times){
    

    const int repeats = 1024;

    times.reserve(repeats);

    char buff[1024];

    NFSClient client;
    client.connect4((char*)address.c_str(), 9000, (char*)user.c_str(), "password");

    for(int i = repeats; i >0; --i){
        auto beg = std::chrono::high_resolution_clock::now();

        auto fd = client.open((char*) file_name.c_str(), O_RDONLY, 0);
        client.read(fd, buff, 1024);
        client.close(fd);

        auto end = std::chrono::high_resolution_clock::now();
        double time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - beg).count(); 
        times.push_back(time_taken / (double)1e9);
    }

    

    return 0;
}

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

    const unsigned int num_workers = 10;

    std::vector<std::vector<double> > t;

    for(auto i = num_workers; i>0; --i){
        std::vector<double>* times = new std::vector<double>;
        std::string usrname = std::string("user")+std::to_string(i);
        func(usrname, std::string(argv[2]), std::string(argv[4]),*times);
        t.push_back(*times);
    }


    std::vector<double> times;
    for(auto t_: t){
        times.insert(times.end(), t_.begin(), t_.end());
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
    +"\nnumber measurements: "+std::to_string(times.size())
    <<std::endl;
}