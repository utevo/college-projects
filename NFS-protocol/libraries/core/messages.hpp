#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <cstdint>
#include <sys/types.h>
#include <vector>
#include <string>
#include <dirent.h>

enum struct MessageType : u_int8_t {
  OPEN_REQUEST,
  OPEN_RESPONSE,

  READ_REQUEST,
  READ_RESPONSE,

  WRITE_REQUEST,
  WRITE_RESPONSE,

  LSEEK_REQUEST,
  LSEEK_RESPONSE,

  CLOSE_REQUEST,
  CLOSE_RESPONSE,

  UNLINK_REQUEST,
  UNLINK_RESPONSE,

  OPENDIR_REQUEST,
  OPENDIR_RESPONSE,

  READDIR_REQUEST,
  READDIR_RESPONSE,

  CLOSEDIR_REQUEST,
  CLOSEDIR_RESPONSE,

  AUTHENTICATE_REQUEST,
  AUTHENTICATE_RESPONSE
};


struct OpenRequest {
  std::string path;
  int oflag;
  int mode;
};

struct OpenResponse {
  int result;
  int error;
};

struct ReadRequest {
  int fd;
  ssize_t count;
};

struct ReadResponse {
  int result;
  std::vector<u_int8_t> buf;
  int error;
};

struct WriteRequest {
  int fd;
  std::vector<u_int8_t> buf;
};

struct WriteResponse {
  ssize_t result;
  int error;
};

struct LseekRequest {
  int fd;
  off_t offset;
  int whence;
};

struct LseekResponse {
  off_t result;
  int error;
};

struct CloseRequest {
  int fd;
};

struct CloseResponse {
  int result;
  int error;
};

struct UnlinkRequest {
  std::string pathname;
};

struct UnlinkResponse {
  int result;
  int error;
};

struct OpendirRequest {
  std::string name;
};

struct OpendirResponse {
  int result;
  int error;
};

struct ReaddirRequest {
  int dirfd;
};

struct ReaddirResponse {
  int isDirentNull;
  dirent result;
  int error;
};

struct ClosedirRequest {
  int dirfd;
};

struct ClosedirResponse {
  int result;
  int error;
};
struct AuthenticateRequest{
    std::string username;
    std::string password;
};

struct AuthenticateResponse{
    int result; // 0 - authenticated, 1 - wrong password, 2 -username doesnt exist
    int error;
};

extern void sendMessage(int fd, std::vector<u_int8_t> request);
extern std::vector<u_int8_t> receiveMessage(int fd);

#endif
