#include <sys/types.h>
#include <vector>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "handler.h"
#include "../libraries/core/serializers.hpp"

#include <stdio.h>
#include <string.h>

const std::string Handler::shadowFilePath = "../server/shadowfile.txt";

AuthenticateResponse Handler::authenticate_handler(std::vector<u_int8_t> byte_request){
    std::cout << "AUTHENTICATE" << std::endl;

    AuthenticateRequest request = DeserializeAuthenticateRequest(byte_request);
    std::cout << "Request:" << std::endl;
    std::cout << "  username: " << request.username << std::endl;

    std::size_t passwordHash = std::hash<std::string>{}(request.password);

    int result = authenticate(request.username, passwordHash);
    AuthenticateResponse response = {result, errno};

    std::cout << "Response:" << std::endl;
    std::cout << "  result: " << response.result << std::endl;
    std::cout << "  error: " << response.error << std::endl;
    std::cout << std::endl;

    return response;
}

int Handler::authenticate(std::string username, std::size_t passwordHash){
    std::ifstream infile(shadowFilePath);
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string name;
        std::size_t pass;
        if (!(iss >> name >> pass))
            break;
        if(username.compare(name) == 0) {
            if(pass == passwordHash)
                return 0;
            else
                return 1;
        }
    }
    return 2;
}

std::vector<u_int8_t> Handler::open_handler(std::vector<u_int8_t> byte_request) {
    std::cout << "OPEN" << std::endl;

    OpenRequest request = DeserializeToOpenRequest(byte_request);
    std::cout << "Request:" << std::endl;
    std::cout << "  path: " << request.path << std::endl;
    std::cout << "  oflag: " << request.oflag << std::endl;
    std::cout << "  mode: " << request.mode << std::endl;
    int fileFd = -1;
    int result;

    try {
        result = open(request.path.c_str(), request.oflag, request.mode);
        if(result >= 0)
            fileFd = fileMapper.addDescriptor(result);
        else {
            fileFd = -1;
            errno = 1;
        }
    }
    catch (std::invalid_argument&) {
        fileFd = -1;
        errno = 1;
    }

    OpenResponse response = {fileFd, errno};
    std::cout << "Response:" << std::endl;
    std::cout << "  client_fd: " << response.result << std::endl;
    std::cout << "  server_fd: " << result << std::endl;
    std::cout << "  error: " << response.error << std::endl;
    std::cout << std::endl;

    std::vector<u_int8_t> byte_response = SerializeOpenResponse(response);
    return byte_response;
}

std::vector<u_int8_t> Handler::read_handler(std::vector<u_int8_t> byte_request) {
    std::cout << "READ" << std::endl;

    ReadRequest request = DeserializeToReadRequest(byte_request);
    std::cout << "Request:" << std::endl;
    std::cout << "  fd: " << request.fd << std::endl;
    std::cout << "  count: " << request.count << std::endl;
    int result = -1;
    std::vector<u_int8_t> buf = std::vector<u_int8_t>(request.count);
    try {
        int server_fd = fileMapper[request.fd];
        result = read(server_fd, buf.data(), request.count);
        buf.resize(result);
    }
    catch (std::out_of_range&) {
        result = -1;
        errno = 1;
    }
    ReadResponse response = {result, buf, errno};
    std::cout << "Response:" << std::endl;
    std::cout << "  result: " << response.result << std::endl;
    std::cout << "  error: " << response.error << std::endl;
    std::cout << std::endl;

    std::vector<u_int8_t> byte_response = SerializeReadResponse(response);
    return byte_response;
}
std::vector<u_int8_t> Handler::write_handler(std::vector<u_int8_t> byte_request) {
    std::cout << "WRITE" << std::endl;

    WriteRequest request = DeserializeToWriteRequest(byte_request);
    std::cout << "Request:" << std::endl;
    std::cout << "  fd: " << request.fd << std::endl;
    std::cout << "  buf_size: " << request.buf.size() << std::endl;
    int result;
    try {
        int server_fd = fileMapper[request.fd];
        result = write(server_fd, request.buf.data(), request.buf.size());
    }
    catch (std::out_of_range&) {
        result = -1;
        errno = 1;
    }
    WriteResponse response = {result, errno};
    std::cout << "Response:" << std::endl;
    std::cout << "  result: " << response.result << std::endl;
    std::cout << "  error: " << response.error << std::endl;
    std::cout << std::endl;

    std::vector<u_int8_t> byte_response = SerializeWriteResponse(response);
    return byte_response;
};

//+
std::vector<u_int8_t> Handler::lseek_handler(std::vector<u_int8_t> byte_request) {
    std::cout << "LSEEK" << std::endl;

    LseekRequest request = DeserializeToLseekRequest(byte_request);
    std::cout << "Request:" << std::endl;
    std::cout << "  fd: " << request.fd << std::endl;
    std::cout << "  offset: " << request.offset << std::endl;
    std::cout << "  whence: " << request.whence << std::endl;

    off_t result;
    try {
        int server_fd = fileMapper[request.fd];
        result = lseek(server_fd, request.offset, request.whence);
    }
    catch (std::out_of_range&) {
        result = -1;
        errno = 1;
    }
    LseekResponse response = {result, errno};
    std::cout << "Response:" << std::endl;
    std::cout << "  result: " << response.result << std::endl;
    std::cout << "  error: " << response.error << std::endl;
    std::cout << std::endl;

    std::vector<u_int8_t> byte_response = SerializeLseekResponse(response);
    return byte_response;
};
//+
std::vector<u_int8_t> Handler::close_handler(std::vector<u_int8_t> byte_request) {
    std::cout << "CLOSE" << std::endl;

    CloseRequest request = DeserializeToCloseRequest(byte_request);
    std::cout << "Request:" << std::endl;
    std::cout << "  fd: " << request.fd << std::endl;

    int result;
    try {
        int server_fd = fileMapper[request.fd];
        result = close(server_fd);
        fileMapper.removeDescriptor(request.fd);
    }
    catch (std::out_of_range&) {
        result = -1;
        errno = 1;
    }
    CloseResponse response = {result, errno};
    std::cout << "Response:" << std::endl;
    std::cout << "  result: " << response.result << std::endl;
    std::cout << "  error: " << response.error << std::endl;
    std::cout << std::endl;

    std::vector<u_int8_t> byte_response = SerializeCloseResponse(response);
    return byte_response;
};
//+
std::vector<u_int8_t> Handler::unlink_handler(std::vector<u_int8_t> byte_request) {
    std::cout << "UNLINK" << std::endl;

    UnlinkRequest request = DeserializeToUnlinkRequest(byte_request);
    std::cout << "Request:" << std::endl;
    std::cout << " pathname: " << request.pathname << std::endl;

    int result = unlink(request.pathname.c_str());

    UnlinkResponse response = {result, errno};
    std::cout << "Response:" << std::endl;
    std::cout << "  result: " << response.result << std::endl;
    std::cout << "  error: " << response.error << std::endl;
    std::cout << std::endl;

    std::vector<u_int8_t> byte_response = SerializeUnlinkResponse(response);
    return byte_response;
};
//

std::vector<u_int8_t> Handler::opendir_handler(std::vector<u_int8_t> byte_request) {
    std::cout << "OPENDIR" << std::endl;

    OpendirRequest request = DeserializeToOpendirRequest(byte_request);
    std::cout << "Request:" << std::endl;
    std::cout << " name: " << request.name << std::endl;
    int result = -1;
    try {
        DIR* dirstream = opendir(request.name.c_str());
        if ( dirstream != nullptr) {
            result = dirMapper.addDescriptor(dirstream);
        }
    } catch (std::exception&) {
        result = -1;
        errno = 1;
    }
    OpendirResponse response = {result, errno};
    std::cout << "Response:" << std::endl;
    std::cout << "  result: " << response.result << std::endl;
    std::cout << "  error: " << response.error << std::endl;
    std::cout << std::endl;

    std::vector<u_int8_t> byte_response = SerializeOpendirResponse(response);
    return byte_response;
};

std::vector<u_int8_t> Handler::readdir_handler(std::vector<u_int8_t> byte_request) {
  std::cout << "READDIR" << std::endl;

  ReaddirRequest request = DeserializeToReaddirRequest(byte_request);
  std::cout << "Request:" << std::endl;
  std::cout << " dirfd: " << request.dirfd << std::endl;
  int isDirentNull = 0;
  dirent result;
  try {
      DIR *dirp = dirMapper[request.dirfd];
      if (dirp == nullptr)
          isDirentNull = 1;
      else {
          dirent *resultPtr = readdir(dirp);
          if (resultPtr == nullptr)
              isDirentNull = 1;
          else
              memcpy((void *) &result, resultPtr, sizeof(result));
      }
  }
  catch (std::exception&) {
    errno = 1;
  }
  ReaddirResponse response = {isDirentNull, result, errno};
  std::cout << "Response:" << std::endl;
  std::cout << "  result: " << response.result.d_name << std::endl;
  std::cout << "  error: " << response.error << std::endl;
  std::cout << std::endl;

  std::vector<u_int8_t> byte_response = SerializeReaddirResponse(response);
  return byte_response;
};

std::vector<u_int8_t> Handler::closedir_handler(std::vector<u_int8_t> byte_request) {
    std::cout << "CLOSEDIR" << std::endl;

    ClosedirRequest request = DeserializeToClosedirRequest(byte_request);
    std::cout << "Request:" << std::endl;
    std::cout << " dirfd: " << request.dirfd << std::endl;
    int result = -1;
    try {
        DIR *dirp = dirMapper[request.dirfd];
        if (dirp != nullptr)
            result =  closedir(dirp);
    } catch (std::exception&) {
        result = -1;
        errno = 1;
    }
    ClosedirResponse response = {result, errno};
    std::cout << "Response:" << std::endl;
    std::cout << "  result: " << response.result << std::endl;
    std::cout << "  error: " << response.error << std::endl;
    std::cout << std::endl;

    std::vector<u_int8_t> byte_response = SerializeClosedirResponse(response);
    return byte_response;
};


std::vector<u_int8_t> Handler::make_response(std::vector<u_int8_t> byte_request) {
    MessageParser parser = MessageParser(byte_request);
    switch (parser.readMessageType()) {
        case MessageType::OPEN_REQUEST:
            return open_handler(byte_request);
        case MessageType::READ_REQUEST:
            return read_handler(byte_request);
        case MessageType::WRITE_REQUEST:
            return write_handler(byte_request);
        case MessageType::LSEEK_REQUEST:
            return lseek_handler(byte_request);
        case MessageType::CLOSE_REQUEST:
            return close_handler(byte_request);
        case MessageType::UNLINK_REQUEST:
            return unlink_handler(byte_request);
        case MessageType::OPENDIR_REQUEST:
            return opendir_handler(byte_request);
        case MessageType::READDIR_REQUEST:
            return readdir_handler(byte_request); 
        case MessageType::CLOSEDIR_REQUEST:
            return closedir_handler(byte_request);
        default:
            return std::vector<u_int8_t>{0, 0, 0, 0, 0};
    }
}
