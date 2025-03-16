#ifndef WORKER_HPP
#define WORKER_HPP
#include <vector>
#include <thread>
#include <unordered_map>

#include <sys/types.h>
#include "DescriptorsMapper.h"
#include "handler.h"


class Worker {
public:
  Worker(int socket_fd);
  void run();
  std::thread spawn();

private:
  bool authenticateUser();

  int socket_fd;
  Handler handler;
};

#endif