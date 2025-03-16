#ifndef TEST_HELPER_H
#define TEST_HELPER_H


#include "gtest/gtest.h"
#include "gmock/gmock.h"


#include "../connector.h"

#include "../saver.h"
#include "../reader.h"

class mock_connector: public Connector{
public:

    MOCK_METHOD(int, connect, (std::string host, uint port, std::string user, std::string passwd));
    MOCK_METHOD(int, open, (std::string path, int oflag, int mode));
    MOCK_METHOD(ssize_t, read, (FileDescriptor fd, char *buf, size_t count));
    MOCK_METHOD(ssize_t, write, (FileDescriptor fd, const char* buf, size_t count));
    MOCK_METHOD(bool, ls, (std::string path, unsigned int options, std::vector<FileInfo>& dirs));
    MOCK_METHOD(int, close, (FileDescriptor fd));
    MOCK_METHOD(off_t, lseek, (int fd, off_t offset, int whence));

};

class mock_reader_: public Reader{
public:
    MOCK_METHOD(ssize_t, read, (char* buffer, uint len));
    MOCK_METHOD(bool, open, (std::string name));
    MOCK_METHOD(void, close, ());
};


class mock_saver_: public Saver{
    public:
    
    MOCK_METHOD(bool, save, (char* buffer, uint len));
    MOCK_METHOD(bool, open, (std::string name));
    MOCK_METHOD(void, close, ());
};

#endif

