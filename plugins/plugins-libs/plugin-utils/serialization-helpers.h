#ifndef SERIALIZATIONHELPERS_H
#define SERIALIZATIONHELPERS_H

#include <cstring>
#include <string>

namespace serialization_helpers {

template <typename T>
inline const void* restore_simple_type(T& t, const void* src) {
  std::memcpy(&t, src, sizeof(t));
  return static_cast<const char*>(src) + sizeof(t);
}

template <typename T>
inline void* store_simple_type(T t, void* src) {
  std::memcpy(src, &t, sizeof(t));
  return static_cast<char*>(src) + sizeof(t);
}

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

}  // namespace serialization_helpers

#endif  // SERIALIZATIONHELPERS_H
