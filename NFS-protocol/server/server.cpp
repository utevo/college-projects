#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "worker.hpp"
#include "../libraries/core/messages.hpp"
#include "../libraries/core/serializers.hpp"
#include "handler.h"

const u_int PORT = 9000;
const u_int HOW_MANY_CONNECTION = 5;

void work(int);

int main() {
  int server_fd;
  unsigned int length;
  struct sockaddr_in server_addr;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    std::cerr << "opening stream socket";
    return 1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof server_addr) == -1) {
    std::cerr << "binding stream socket";
    return 1;
  }

  length = sizeof server_addr;
  if (getsockname(server_fd, (struct sockaddr *)&server_addr, &length) == -1) {
    std::cerr << "getting socket name";
    return 2;
  }
  printf("Socket port #%d\n", ntohs(server_addr.sin_port));
  listen(server_fd, 5);
  std::cout << "Server_fd: " << server_fd << std::endl;

  auto workers = std::vector<std::thread>();
  auto fds = std::vector<int>();

  try {
      while (true) {
          int socket_fd = accept(server_fd, (struct sockaddr *)0, (unsigned int *)0);
          if (socket_fd == -1)
              std::cerr << "Error during tcp accept";
          else{
              fds.push_back(socket_fd);
              workers.push_back(Worker(socket_fd).spawn());
          }
      }
  }
  catch (std::exception& exception) {
      std::cerr << exception.what();
      std::for_each(workers.begin(), workers.end(), [] (std::thread& worker) { worker.join(); });
      std::for_each(fds.begin(), fds.end(), close);
  }
  return 0;
}


