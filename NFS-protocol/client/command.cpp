#include "command.h"
#include <iostream>
#include <sstream>
#include <string>
#include "include/linenoise/linenoise/linenoise.h"




Command get_command(){
    
    char *line;
    line = linenoise(">>> ");
    if(line == nullptr){
        Command c;
        c.type = Command::Exit;
        return c;    
    }
    linenoiseHistoryAdd(line);
    std::string s(line);

    std::string command_type;

    std::stringstream ss(s);
    ss>>command_type;

    Command c;
    c.command_string = s;

    if(command_type.find(std::string("help")) != std::string::npos){
        c.type = Command::Help;
    }else if(command_type.find(std::string("get")) != std::string::npos){
        c.type = Command::Get;
    }else if(command_type.find(std::string("exit")) != std::string::npos){
        c.type = Command::Exit;
    }else if(command_type.find(std::string("put")) != std::string::npos){
        c.type = Command::Put;
    }else if(command_type.find(std::string("ls")) != std::string::npos){
        c.type = Command::Get_dir;
    }else if(command_type.find(std::string("record")) != std::string::npos){
        c.type = Command::Record;
    }else if(command_type == ""){
        c.type = Command::Empty;
    }else{
        c.type = Command::Invalid_command;    
    }
    
    
    return c;
};

std::string get_command_type(const std::string& s){
    std::string new_s;
    bool c_begun = false;
    for(char c: s){
        if(c == ' '){
            if(c_begun){
                // konie komendy (nastepna spacja )
                break;
            }
        }else{
            if(!c_begun){
                c_begun = true;
            }
            new_s += c; 
        }
    }
    return new_s;
}