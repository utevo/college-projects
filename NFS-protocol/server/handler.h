#ifndef NFS_HANDLER_H
#define NFS_HANDLER_H

#include "DescriptorsMapper.h"
#include "../libraries/core/messages.hpp"

class Handler {
public:
    AuthenticateResponse authenticate_handler(std::vector<u_int8_t> byte_request);
    std::vector<u_int8_t> open_handler(std::vector<u_int8_t> byte_request);
    std::vector<u_int8_t> read_handler(std::vector<u_int8_t> byte_request);
    std::vector<u_int8_t> write_handler(std::vector<u_int8_t> byte_request);
    std::vector<u_int8_t> lseek_handler(std::vector<u_int8_t> byte_request);
    std::vector<u_int8_t> close_handler(std::vector<u_int8_t> byte_request);
    std::vector<u_int8_t> unlink_handler(std::vector<u_int8_t> byte_request);
    std::vector<u_int8_t> opendir_handler(std::vector<u_int8_t> byte_request);
    std::vector<u_int8_t> readdir_handler(std::vector<u_int8_t> byte_request);
    std::vector<u_int8_t> closedir_handler(std::vector<u_int8_t> byte_request);
    std::vector<u_int8_t> make_response(std::vector<u_int8_t> byte_request);

private:
    int authenticate(std::string username, std::size_t passwordHash);

    DescriptorsMapper<int> fileMapper;
    DescriptorsMapper<DIR*> dirMapper;
    const static std::string shadowFilePath;
};



#endif //NFS_HANDLER_H
