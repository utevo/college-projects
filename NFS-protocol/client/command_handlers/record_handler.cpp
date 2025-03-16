#include "handlers.h"
#include <sys/stat.h>
#include <fcntl.h>

bool record_handler(Command c, Connector* connector){
    if(c.type != Command::Record){
        return false;
    }
    std::stringstream ss(c.command_string);
    // record <remote file> <record size> <record number> 
    std::string file;
    unsigned int size;
    unsigned int number;
    ss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
    
    
    if (!(ss >> file >> size >> number))
    {
        ss.clear();
        return false;
    }
    if(size > 4096){
        std::cout<<"Sorry, record size must be lower than 4096\n";
        return false;
    }

    char buffer[size+2];

    auto fd = connector->open(file, O_RDONLY, 0);

    auto r = connector->lseek(fd, 0, SEEK_END);
    if(r!=-1){
        if(r >= size*(number + 1)){
            // to znaczy że nasz rekord jest w pliku
            connector->lseek(fd, size*number, SEEK_SET);
            connector->read(fd, buffer, size+1);
            buffer[size+1] = '\0';
            std::cout<<buffer<<'\n';
            connector->close(fd);
            return true;
        }
    }

    connector->close(fd);

    std::cout<<"\nCommand failed for some reason\n";
    return false;    

}