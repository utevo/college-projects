#include "lib.hpp"

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>

#include "../core/messages.hpp"
#include "../core/serializers.hpp"


#include <sstream>
#include <string>
#include <vector>


std::string bytes_to_string2(std::vector<u_int8_t> bytes) {
  std::stringstream sstream;
  for (std::vector<u_int8_t>::const_iterator i = bytes.begin(); i != bytes.end(); ++i)
    sstream << int(*i) << ' ';

  std::string result;
  getline(sstream, result);
  return result;
}

extern int test_libclient(int x) { return test_libcore(x) * 321; };

extern int error = -1;

NFSClient::NFSClient(){};

int NFSClient::connect4(char *host, int port, char *user, char *password) {
  struct sockaddr_in server;
  struct hostent *hp;
  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd_ == -1) {
    throw std::runtime_error("Error during opening stream socket");
  }
  server.sin_family = AF_INET;
  hp = gethostbyname(host);

  if (hp == (struct hostent *)0) {
    std::cerr << host << "%s: unknown host\n";
    throw std::runtime_error(std::string(host) + ": unknown host");
  }

  memcpy((char *)&server.sin_addr, (char *)hp->h_addr, hp->h_length);
  server.sin_port = htons(port);
  if (connect(socket_fd_, (struct sockaddr *)&server, sizeof server) == -1) {
    std::cerr << "connecting stream socket";
    throw std::runtime_error("Error during connecting stream socket");
  }
  return authenticate(user, password);
};

int NFSClient::authenticate(char *user, char *password) {
    AuthenticateRequest authenticate_request{user, password};
    std::vector<u_int8_t> byte_request = SerializeAuthenticateRequest(authenticate_request);

    sendRequest_(byte_request);
    std::vector<u_int8_t> byte_response = receiveResponse_();

    AuthenticateResponse authenticate_response = DeserializeAuthenticateResponse(byte_response);
    if (authenticate_response.result == 0)
        return authenticate_response.result;
    else if (authenticate_response.result == 1)
        throw std::domain_error("wrong password");
    else
        throw std::domain_error("user: " + std::string(user) + " does not exist");
}

int NFSClient::open(char *path, int oflag, int mode) {
  OpenRequest open_request{path, oflag, mode};
  std::vector<u_int8_t> byte_request = SerializeOpenRequest(open_request);

  sendRequest_(byte_request);
  std::vector<u_int8_t> byte_response = receiveResponse_();

  OpenResponse open_response = DeserializeToOpenResponse(byte_response);
  error = open_response.error;
  return open_response.result;
};

ssize_t NFSClient::read(int fd, void *buf, size_t count) {
  ReadRequest read_request{fd, static_cast<ssize_t>(count)};
  std::vector<u_int8_t> byte_request = SerializeReadRequest(read_request);

  sendRequest_(byte_request);
  std::vector<u_int8_t> byte_response = receiveResponse_();

  ReadResponse read_response = DeserializeToReadResponse(byte_response);
  error = read_response.error;
  memcpy(buf, read_response.buf.data(), read_response.result);
  return read_response.result;
};
//+
ssize_t NFSClient::write(int fd, const void *buf, size_t count) {

  const u_int8_t *charBuf = (u_int8_t *)buf;
  std::vector<u_int8_t>vectorBuff(charBuf,charBuf + count);
  std::cout << "vectorBuf_size(): " << vectorBuff.size() << std::endl;
  WriteRequest write_request{fd,vectorBuff};
  std::vector<u_int8_t> byte_request = SerializeWriteRequest(write_request); 
 
  sendRequest_(byte_request);
  std::vector<u_int8_t> byte_response = receiveResponse_();

  WriteResponse write_response = DeserializeToWriteResponse(byte_response);
  error = write_response.error;
  return write_response.result;

};
//+
off_t NFSClient::lseek(int fd, off_t offset, int whence) {
  LseekRequest lseek_request{fd, offset, whence};
  std::vector<u_int8_t> byte_request = SerializeLseekRequest(lseek_request); 
 
  sendRequest_(byte_request);
  std::vector<u_int8_t> byte_response = receiveResponse_();

  LseekResponse lseek_response = DeserializeToLseekResponse(byte_response);
  error = lseek_response.error;
  return lseek_response.result;
};
//
int NFSClient::close(int fd) {
  CloseRequest close_request{fd};
  std::vector<u_int8_t> byte_request = SerializeCloseRequest(close_request); 
 
  sendRequest_(byte_request);
  std::vector<u_int8_t> byte_response = receiveResponse_();

  CloseResponse close_response = DeserializeToCloseResponse(byte_response);
  error = close_response.error;
  return close_response.result;
};
//
int NFSClient::unlink(const char *pathname) {
  UnlinkRequest unlink_request{pathname};
  std::vector<u_int8_t> byte_request = SerializeUnlinkRequest(unlink_request); 

  sendRequest_(byte_request);
  std::vector<u_int8_t> byte_response = receiveResponse_();

  UnlinkResponse unlink_response = DeserializeToUnlinkResponse(byte_response);
  error = unlink_response.error;
  return unlink_response.result;
}

int NFSClient::opendir(const char *name) {
  OpendirRequest opendir_request{name};
  std::vector<u_int8_t> byte_request = SerializeOpendirRequest(opendir_request); 

  sendRequest_(byte_request);
  std::vector<u_int8_t> byte_response = receiveResponse_();

  OpendirResponse opendir_response = DeserializeToOpendirResponse(byte_response);
  error = opendir_response.error;
  return opendir_response.result;
};

dirent NFSClient::readdir(int dirfd, bool& is_null) {
  ReaddirRequest readdir_request{dirfd};
  std::vector<u_int8_t> byte_request = SerializeReaddirRequest(readdir_request); 
 
  sendRequest_(byte_request);
  std::vector<u_int8_t> byte_response = receiveResponse_();

  ReaddirResponse readdir_response = DeserializeToReaddirResponse(byte_response);
  error = readdir_response.error;
  is_null = readdir_response.isDirentNull;
  return readdir_response.result;
};

int NFSClient::closedir(int dirfd) {
  ClosedirRequest closedir_request{dirfd};
  std::vector<u_int8_t> byte_request = SerializeClosedirRequest(closedir_request); 
 
  sendRequest_(byte_request);
  std::vector<u_int8_t> byte_response = receiveResponse_();

  ClosedirResponse closedir_response = DeserializeToClosedirResponse(byte_response);
  error = closedir_response.error;
  return closedir_response.result;
};


void NFSClient::sendRequest_(std::vector<u_int8_t> request) {
  sendMessage(socket_fd_, request);
}; 

std::vector<u_int8_t> NFSClient::receiveResponse_() {
  return receiveMessage(socket_fd_);
};
