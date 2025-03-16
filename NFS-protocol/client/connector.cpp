#include "connector.h"
#include <iostream>

#include <experimental/filesystem>

#include <unistd.h>
#include <sys/types.h>

/**
 * LocalFSConnector
 * 
 */

int LocalFSConnector::connect(std::string host, uint port, std::string user, std::string passwd){
    return 1;
}

FileDescriptor LocalFSConnector::open(std::string path, int oflag, int mode){
    std::fstream *s = new std::fstream(path, std::ios::in);
    FileDescriptor fd;
    if(open_files.empty()){
        fd = 1;
    }else{
        fd = open_files.rbegin()->first + 1;
    }
    
    open_files[fd] = s;
    return fd;
}


ssize_t LocalFSConnector::read(FileDescriptor fd, char *buf, size_t count){
    // TODO: sprawdzić czy plik istnieje
    if(open_files.find(fd) != open_files.end()){
        std::fstream* s = open_files[fd];
        s->get(buf, count);
        return s->gcount();
    }
    return 0;
}

ssize_t LocalFSConnector::write(FileDescriptor fd, const char* buf, size_t count){
    if(open_files.find(fd) != open_files.end()){
        std::string s(buf, count);
        std::cout<<s<<std::endl;
    }
    return 0;
}

int LocalFSConnector::close(FileDescriptor fd){
    if(open_files.find(fd) != open_files.end()){
        open_files[fd]->close();
        delete open_files[fd];
        open_files.erase(fd);
    }
}

LocalFSConnector::~LocalFSConnector(){
    for (auto iter = open_files.rbegin(); iter != open_files.rend(); ++iter) {
        close(iter->first);
    }
}

bool LocalFSConnector::ls(std::string path, unsigned int options, std::vector<FileInfo>& dirs){
    if(std::experimental::filesystem::exists(path)){
        for (const auto & entry : std::experimental::filesystem::directory_iterator(path))
            dirs.push_back(entry.path()); 
        
        return true;
    }else{
        return false;
    }
}

off_t LocalFSConnector::lseek(int fd, off_t offset, int whence){
    if(open_files.find(fd) != open_files.end()){
        std::fstream* s = open_files[fd];
        s->seekg(offset, (std::ios_base::seekdir) whence);
        return s->tellg();
    }
    return 0;
}

/**
 * Real connector
 * 
 */

NFSConnector::NFSConnector(){
    client = new NFSClient();
}

int NFSConnector::connect(std::string host, uint port, std::string user, std::string passwd){
    return client->connect4((char*)host.c_str(), port, (char*)user.c_str(), (char*)passwd.c_str());
}

FileDescriptor NFSConnector::open(std::string path, int oflag, int mode){
    return client->open((char*)path.c_str(), oflag, mode);
}

ssize_t NFSConnector::read(FileDescriptor fd, char *buf, size_t count){
    return client->read(fd, buf, count);
}

ssize_t NFSConnector::write(FileDescriptor fd, const char* buf, size_t count){
    return client->write(fd, buf, count);
}

int NFSConnector::close(FileDescriptor fd){
    return client->close(fd);
}

off_t NFSConnector::lseek(int fd, off_t offset, int whence){
    return client->lseek(fd, offset, whence);
}

bool NFSConnector::ls(std::string path, unsigned int options, std::vector<FileInfo>& dirs){
    auto dd = client->opendir(path.c_str());
    bool is_null = false;
    do{
        dirent dirend_ptr = client->readdir(dd, is_null);
        if(error != 0 || is_null){
            break;
        }
        dirs.push_back(std::string(dirend_ptr.d_name));
    }while(true);

    client->closedir(dd);
    return error == 0;
}
