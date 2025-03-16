#ifndef UTILS_HPP
#define UTILS_HPP

#include <sstream>
#include <string>
#include <vector>

std::string bytes_to_string(std::vector<u_int8_t> bytes) {
  std::stringstream sstream;
  for (std::vector<u_int8_t>::const_iterator i = bytes.begin(); i != bytes.end(); ++i)
    sstream << int(*i) << ' ';

  std::string result;
  getline(sstream, result);
  return result;
}

std::string bytes_to_string(char *bytes, int len) {
  std::stringstream sstream;
  for (int i = 0; i < len; ++i)
    sstream << int(*(bytes+i)) << ' ';

  std::string result;
  getline(sstream, result);
  return result;
}

#endif