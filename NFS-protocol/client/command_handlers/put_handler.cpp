#include "handlers.h"
#include <limits>
#include <sstream>
#include <sys/stat.h>
#include <fcntl.h>

bool put_handler(Command c, Reader* reader, Connector* connector){
    if(c.type != Command::Put){
        throw -1;
    }
    std::stringstream ss(c.command_string);
    std::string source;
    std::string dst;
    ss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
    ss >> source;
    ss >> dst;

    char buf[4096];

    if(reader->open(source) == false){
        return false;
    }
    FileDescriptor fd = connector->open(dst, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    if(fd <= 0){
        return false;
    }

    ssize_t len;
    do{
        len = reader->read(buf, 4096);
        if(len == 0){
            break;
        }
        connector->write(fd, buf, len);
    }while(len != 0);
    
    connector->close(fd);
    
    reader->close();
    
    return true;
};