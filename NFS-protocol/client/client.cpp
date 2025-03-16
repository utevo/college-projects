#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "../libraries/client/lib.hpp"

#include "include/lib.hpp"
#include "utils.h"
#include "loop.h"
#include "connector.h"
#include "include/cxxopts.hpp"
#include "linenoise_setup.h"


bool uNFS_EXIT = false;

void signal_callback_handler(int signum) {
   std::cout << "\n\nSpadam stad z powodu: " << signum << std::endl;
   // Terminate program
   uNFS_EXIT = true;
}


int main(int argc, char* argv[]){
    signal(SIGINT, signal_callback_handler);

    cxxopts::Options options("test", "A brief description");

    options.add_options()
        ("u,user", "User name", cxxopts::value<std::string>())
        ("a,address", "Host address", cxxopts::value<std::string>())
        ("p,port", "Define port", cxxopts::value<int>()->default_value("9000"))
        ("h,help", "Print usage")
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help") || !result.count("address") || !result.count("user"))
    {
      std::cout << options.help() << std::endl;
      exit(0);
    }

    std::string host = result["address"].as<std::string>();
    std::string user = result["user"].as<std::string>();
    int port = result["port"].as<int>();

    // prompting for password
    std::string passwd(getpass("Password: "));

    Connector* connector = new NFSConnector(); 

    try{
      auto succ = connector->connect(host.c_str(), port, user.c_str(), passwd.c_str());
      if(succ != 0){
        std::cout<<"Nie udało się połączyć z serwerem\n";
        exit(-1);  
      }
    }
    catch(std::domain_error e){
      std::cout<<"Nie udało się połączyć z serwerem\n";
      exit(-1);
    }

    display_welcome_message();


    custom_linenoise_setup();
    run_loop(connector, uNFS_EXIT);


    delete connector;
    linenoiseHistorySave(".uNSFcommandhistory.txt");
    
}
