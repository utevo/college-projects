#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include "../libraries/core/messages.hpp"
#include "../libraries/core/serializers.hpp"

#include "../tests/utils.hpp"
//#include "worker.h"

#include "worker.hpp"
#include "handler.h"

Worker::Worker(int socket_fd) {
  this->socket_fd = socket_fd;
}
std::vector<u_int8_t> open_handler(std::vector<u_int8_t> byte_request) {
  std::cout << "OPEN" << std::endl;

  OpenRequest request = DeserializeToOpenRequest(byte_request);
  std::cout << "Request:" << std::endl;
  std::cout << "  path: " << request.path << std::endl;
  std::cout << "  oflag: " << request.oflag << std::endl;
  std::cout << "  mode: " << request.mode << std::endl;
  int result = open(request.path.c_str(), request.oflag, request.mode);

  OpenResponse response = {result, errno};
  std::cout << "Response:" << std::endl;
  std::cout << "  fd: " << response.result << std::endl;
  std::cout << "  error: " << response.error << std::endl;
  std::cout << std::endl;

  std::vector<u_int8_t> byte_response = SerializeOpenResponse(response);
  return byte_response;
}

std::vector<u_int8_t> read_handler(std::vector<u_int8_t> byte_request) {
  std::cout << "READ" << std::endl;

  ReadRequest request = DeserializeToReadRequest(byte_request);
  std::cout << "Request:" << std::endl;
  std::cout << "  fd: " << request.fd << std::endl;
  std::cout << "  count: " << request.count << std::endl;

  std::vector<u_int8_t> buf = std::vector<u_int8_t>(request.count);
  int result = read(request.fd, buf.data(), request.count);
  buf.resize(result);

  ReadResponse response = {result, buf, errno};
  std::cout << "Response:" << std::endl;
  std::cout << "  result: " << response.result << std::endl;
  std::cout << "  error: " << response.error << std::endl;
  std::cout << std::endl;

  std::vector<u_int8_t> byte_response = SerializeReadResponse(response);
  return byte_response;
}


std::vector<u_int8_t> write_handler(std::vector<u_int8_t> byte_request) {
  std::cout << "WRITE" << std::endl;

  WriteRequest request = DeserializeToWriteRequest(byte_request);
  std::cout << "Request:" << std::endl;
  std::cout << "  fd: " << request.fd << std::endl;
  std::cout << "  buf_size: " << request.buf.size() << std::endl;
  int result = write(request.fd, request.buf.data(), request.buf.size());

  WriteResponse response = {result, errno};
  std::cout << "Response:" << std::endl;
  std::cout << "  result: " << response.result << std::endl;
  std::cout << "  error: " << response.error << std::endl;
  std::cout << std::endl;

  std::vector<u_int8_t> byte_response = SerializeWriteResponse(response);
  return byte_response;
};

std::vector<u_int8_t> lseek_handler(std::vector<u_int8_t> byte_request) {
  std::cout << "LSEEK" << std::endl;

  LseekRequest request = DeserializeToLseekRequest(byte_request);
  std::cout << "Request:" << std::endl;
  std::cout << "  fd: " << request.fd << std::endl;
  std::cout << "  offset: " << request.offset << std::endl;
  std::cout << "  whence: " << request.whence << std::endl;

  off_t result =  lseek(request.fd, request.offset, request.whence);

  LseekResponse response = {result, errno};
  std::cout << "Response:" << std::endl;
  std::cout << "  result: " << response.result << std::endl;
  std::cout << "  error: " << response.error << std::endl;
  std::cout << std::endl;

  std::vector<u_int8_t> byte_response = SerializeLseekResponse(response);
  return byte_response;
};

std::vector<u_int8_t> close_handler(std::vector<u_int8_t> byte_request) {
  std::cout << "CLOSE" << std::endl;

  CloseRequest request = DeserializeToCloseRequest(byte_request);
  std::cout << "Request:" << std::endl;
  std::cout << "  fd: " << request.fd << std::endl;

  int result =  close(request.fd);

  CloseResponse response = {result, errno};
  std::cout << "Response:" << std::endl;
  std::cout << "  result: " << response.result << std::endl;
  std::cout << "  error: " << response.error << std::endl;
  std::cout << std::endl;

  std::vector<u_int8_t> byte_response = SerializeCloseResponse(response);
  return byte_response;
};

std::vector<u_int8_t> unlink_handler(std::vector<u_int8_t> byte_request) {
  std::cout << "UNLINK" << std::endl;

  UnlinkRequest request = DeserializeToUnlinkRequest(byte_request);
  std::cout << "Request:" << std::endl;
  std::cout << " pathname: " << request.pathname << std::endl;

  int result =  unlink(request.pathname.c_str());

  UnlinkResponse response = {result, errno};
  std::cout << "Response:" << std::endl;
  std::cout << "  result: " << response.result << std::endl;
  std::cout << "  error: " << response.error << std::endl;
  std::cout << std::endl;
  std::vector<u_int8_t> byte_response = SerializeUnlinkResponse(response);
  return byte_response;
};
//

std::vector<u_int8_t> opendir_handler(std::vector<u_int8_t> byte_request) {
  std::cout << "OPENDIR" << std::endl;

  OpendirRequest request = DeserializeToOpendirRequest(byte_request);
  std::cout << "Request:" << std::endl;
  std::cout << " name: " << request.name << std::endl;
  int result = -1;
  DIR* dirstream =  opendir(request.name.c_str());
     if (dirstream != nullptr)
      result = dirfd(dirstream);
  OpendirResponse response = {result, errno};
  std::cout << "Response:" << std::endl;
  std::cout << "  result: " << response.result << std::endl;
  std::cout << "  error: " << response.error << std::endl;
  std::cout << std::endl;

  std::vector<u_int8_t> byte_response = SerializeOpendirResponse(response);
  return byte_response;
};

std::vector<u_int8_t> readdir_handler(std::vector<u_int8_t> byte_request) {
  std::cout << "READDIR" << std::endl;

  ReaddirRequest request = DeserializeToReaddirRequest(byte_request);
  std::cout << "Request:" << std::endl;
  std::cout << " dirfd: " << request.dirfd << std::endl;
  int isDirentNull = 0;
  DIR *dirp = fdopendir(request.dirfd);
  dirent result;
  if (dirp == nullptr)
    isDirentNull = 1;
  else
  {
      dirent* resultPtr =  readdir(dirp);
        
      if (resultPtr == nullptr)
        isDirentNull = 1;
      else
        memcpy((void *)&result,resultPtr,sizeof(result));
  }

  ReaddirResponse response = {isDirentNull, result, errno};
  std::cout << "Response:" << std::endl;
  std::cout << "  result: " << response.result.d_name << std::endl;
  std::cout << "  error: " << response.error << std::endl;
  std::cout << std::endl;

  std::vector<u_int8_t> byte_response = SerializeReaddirResponse(response);
  return byte_response;
};

std::vector<u_int8_t> closedir_handler(std::vector<u_int8_t> byte_request) {
  std::cout << "CLOSEDIR" << std::endl;

  ClosedirRequest request = DeserializeToClosedirRequest(byte_request);
  std::cout << "Request:" << std::endl;
  std::cout << " dirfd: " << request.dirfd << std::endl;
  int result = -1;
    DIR *dirp = fdopendir(request.dirfd);
    if (dirp != nullptr)
      result =  closedir(dirp);

  ClosedirResponse response = {result, errno};
  std::cout << "Response:" << std::endl;
  std::cout << "  result: " << response.result << std::endl;
  std::cout << "  error: " << response.error << std::endl;
  std::cout << std::endl;

  std::vector<u_int8_t> byte_response = SerializeClosedirResponse(response);
  return byte_response;
};


std::vector<u_int8_t> make_response(std::vector<u_int8_t> byte_request) {
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
void Worker::run() {
    if(!authenticateUser()) {
        close(socket_fd);
        return;
    }

  std::vector<u_int8_t> byte_request;
  std::vector<u_int8_t> byte_response;
  while(true){
      try{
          byte_request = receiveMessage(socket_fd);
          byte_response = handler.make_response(byte_request);
          sendMessage(socket_fd, byte_response);
      }
      catch (std::ios_base::failure&) {
          std::cout << "Ending connection with client" << std::endl;
          close(socket_fd);
          break;
      }
  }
}
std::thread Worker::spawn() {
  return std::thread( [this] { this->run(); } );
}

bool Worker::authenticateUser() {
    std::vector<u_int8_t> byte_request = receiveMessage(socket_fd);

    MessageParser parser = MessageParser(byte_request);
    if(parser.readMessageType() != MessageType::AUTHENTICATE_REQUEST)
        return false;
    AuthenticateResponse authenticateResponse = handler.authenticate_handler(byte_request);
    bool isAuthenticated = authenticateResponse.result == 0;

    sendMessage(socket_fd, SerializeAuthenticateResponse(authenticateResponse));
    return isAuthenticated;
}

