#include "store_primitives.h"
#include <cstring>

void* save_str(const std::string& str, void* dest) {
  auto size = str.length();
  memcpy(dest, &size, sizeof(size));
  char* charDest = static_cast<char*>(dest) + sizeof(size);
  memcpy(charDest, str.data(), str.length());
  return charDest + str.length();
}

const void* restore_str(std::string& str, const void* src) {
  std::string::size_type size;
  memcpy(&size, src, sizeof(size));
  str.reserve(size);
  const char* strSrc = static_cast<const char*>(src) + sizeof(size);
  str.insert(0, strSrc, size);
  return strSrc + size;
}
