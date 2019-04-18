#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <string>

template <typename Out>
void split(const std::string& s, char delim, Out result) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
}

std::string make_uiid();

long get_timestamp();

#endif  // UTILS_H
