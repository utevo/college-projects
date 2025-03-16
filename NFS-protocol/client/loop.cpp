#include "loop.h"
#include "command.h"
#include "reader.h"
#include "utils.h"
#include "command_handlers/handlers.h"
#include <iostream>

int run_loop(Connector* connector, bool& exit){
    bool _run = true;
    bool succ = true;
    while(_run && (!exit)){
        
        Command command = get_command();


        switch(command.type){
            case Command::Invalid_command:{
                display_error_message();
                display_help_prompt();
                break;
            }
            case Command::Empty:{
                continue;
            }
            case Command::Help:{
                display_help();
                break;
            }
            case Command::Get:{
                FSSaver s;
                succ = get_handler(command, &s, connector);
                break;
            }
            case Command::Put:{
                Mock_reader r;
                succ = put_handler(command, &r, connector);
                break;
            }
            case Command::Get_dir:{
                succ = get_dir_handler(command, connector);
                break;
            }
            case Command::Exit:{
                std::cout<<'\n';
                disp_prompt();
                std::cout<<"exit\n";
                _run = false;
                break;
            }
            case Command::Record:{
                succ = record_handler(command, connector);
                break;
            }
            case Command::Not_implemented:{
                show_not_implemented_info();
                break;
            }
            default:{
                show_not_implemented_info();
                break;
            }
        }
        if(succ == false){
            show_command_failed_info();
        }
    }
    return 0;
}