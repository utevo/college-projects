#ifndef LIB_HPP
#define LIB_HPP

#include <dirent.h>
#include <sys/types.h>
#include <vector>

extern int test_libclient(int x);

extern int error;

class NFSClient {
public:
  NFSClient();
  int connect4(char *host, int port, char *user, char *password);

  int open(char *path, int oflag, int mode);
  ssize_t read(int fd, void *buf, size_t count);
  ssize_t write(int fd, const void *buf, size_t count);
  off_t lseek(int fd, off_t offset, int whence);
  int close(int fd);
  int unlink(const char *pathname);

  int opendir(const char *name);
  dirent readdir(int dirfd, bool& is_null);
  int closedir(int dirfd);

private:
  int socket_fd_ = -1;
  void sendRequest_(std::vector<u_int8_t> request);
  std::vector<u_int8_t> receiveResponse_();
  int authenticate(char *user, char *password);
};

#endif
