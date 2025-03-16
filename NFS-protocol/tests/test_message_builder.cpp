#include <iostream>
#include <sstream>
#include <cassert>

#include "./utils.hpp"

#include "../libraries/core/serializers.hpp"


int main() {
  MessageBuilder builder;
  builder.writeMessageType(MessageType::CLOSE_REQUEST);
  builder.write((u_int8_t)2);
  builder.write((int32_t)0x12345678);
  builder.write((u_int32_t)0x99999999);
  builder.write((u_int64_t)0x1234567890ABCDEF);
  char buffer[] = {1, 2, 3, 4, 5};
  builder.write(buffer, sizeof(buffer));
  std::string str = "abcde";
  builder.write(str);
  std::vector<u_int8_t> result = builder.build();

  std::vector<u_int8_t> expected = std::vector<u_int8_t>{
    (u_int8_t) MessageType::CLOSE_REQUEST,
    0x00, 0x00, 0x00, 35,
    2,
    0x12, 0x34, 0x56, 0x78,
    0x99, 0x99, 0x99, 0x99,
    0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF,
    0x00, 0x00, 0x00, 5, 1, 2, 3, 4, 5,
    0x00, 0x00, 0x00, 5, 'a', 'b', 'c', 'd', 'e'
  };

  assert(result == expected);
}