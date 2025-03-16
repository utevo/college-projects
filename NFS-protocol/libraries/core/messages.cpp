#include <algorithm>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


#include "./messages.hpp"
#include "./serializers.hpp"


extern void sendMessage(int fd, std::vector<u_int8_t> request) {
  send(fd, static_cast<void *>(request.data()), request.size(), 0);
};

extern std::vector<u_int8_t> receiveMessage(int fd) {
  std::vector<u_int8_t> result;

  int buffer_size = 4096;
  char buffer[buffer_size];

  int how_many_need_to_read = 5;
  int how_many_read = 0;
  while (how_many_read != how_many_need_to_read) {
    int last_read = read(fd, buffer, how_many_need_to_read - how_many_read);
    if (last_read <= 0) {
      throw std::ios_base::failure("End connection with client");
    }
    how_many_read += last_read;
    result.insert(result.end(), buffer, buffer + last_read);
  }

  MessageParser parser = MessageParser(result);
  how_many_need_to_read = parser.readSize();
  how_many_read = 0;
  while (how_many_read != how_many_need_to_read) {
    int last_read =
        read(fd, buffer,
             std::min(buffer_size, how_many_need_to_read - how_many_read));
    if (last_read <= 0) {
        throw std::ios_base::failure("End connection with client");
    }
    how_many_read += last_read;
    result.insert(result.end(), buffer, buffer + last_read);
  }

  return result;
}
