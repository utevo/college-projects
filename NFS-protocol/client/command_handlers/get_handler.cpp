#include "handlers.h"
#include <sstream>
#include <limits>
#include <sys/stat.h>
#include <fcntl.h>

bool get_handler(Command c, Saver* saver, Connector* connector){
    if(c.type != Command::Get){
        throw -1;
    }
    std::stringstream ss(c.command_string);
    std::string source;
    std::string dst;
    ss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
    ss >> source;
    ss >> dst;

    if(saver->open(dst) == false){
        return false;
    }

    char buf[4096];

    FileDescriptor fd;

    try{
        fd = connector->open(source, O_RDONLY, 0);
    }
    catch(std::ios_base::failure e){
        return false;
    }

    if(fd < 0){
        return false;
    }

    

    ssize_t len;
    do{
        len = connector->read(fd, buf, 4096);
        if(len == 0){
            break;
        }
        saver->save(buf, len);
    }while(len != 0);
    
    connector->close(fd);
    
    saver->close();
    
    return true;
};

