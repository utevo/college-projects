#include "handlers.h"



unsigned int parse_options(std::string options){
    return 0;
}

bool get_dir_handler(Command c, Connector* connector){
    if(c.type != Command::Get_dir){
        return false;
    }
    std::stringstream ss(c.command_string);
    std::string dir;
    std::string options;
    ss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
    ss >> dir;
    ss >> options;

    auto parsed_options = parse_options(options);
    std::vector<FileInfo> dirs;
    connector->ls(dir, parsed_options, dirs);
    
    if(dirs.size() > 0){
        std::cout<<"Your files:\n";
        for(auto d: dirs){
            std::cout<<d<<'\n';
    }}
    return true;
}