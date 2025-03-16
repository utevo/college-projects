#include <iostream>
#include <sstream>
#include <cassert>

#include "./utils.hpp"
#include "../libraries/core/serializers.hpp"


int main() {
  std::vector<u_int8_t> data = std::vector<u_int8_t>{
    (u_int8_t) MessageType::CLOSE_REQUEST,
    0x00, 0x00, 0x00, 5,
    0x69,
    0x12, 0x34, 0x56, 0x78,
    0x99, 0x99, 0x99, 0x99,
    0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF,
    0x00, 0x00, 0x00, 5, 1, 2, 3, 4, 5,
    0x00, 0x00, 0x00, 5, 'a', 'b', 'c', 'd', 'e'
  };

  MessageParser parser = MessageParser(data);
  assert(parser.readMessageType() == MessageType::CLOSE_REQUEST);
  assert(parser.readSize() == 5);
  
  assert(parser.readUInt8T() == 0x69);
  assert(parser.readInt32T() == 0x12345678);
  assert(parser.readUInt32T() == 0x99999999);
  assert(parser.readUInt64T() == 0x1234567890ABCDEF);

  std::vector<u_int8_t> expected_bytes = std::vector<u_int8_t>{1, 2, 3, 4, 5};
  assert(parser.readBytes() == expected_bytes);

  std::string expected_string = "abcde";
  assert(parser.readString() == expected_string);
}