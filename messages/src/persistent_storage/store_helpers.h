#ifndef STORE_HELPERS_H
#define STORE_HELPERS_H
#include <cstring>
#include <string_view>

template <typename T>
const void* restore_simple_type(T& t, const void* src) {
  memcpy(&t, src, sizeof(t));
  return static_cast<const char*>(src) + sizeof(t);
}

template <typename T>
void* store_simple_type(T t, void* src) {
  memcpy(src, &t, sizeof(t));
  return static_cast<char*>(src) + sizeof(t);
}

inline void* save_str(const std::string_view& str, void* dest) {
  auto size = str.length();
  memcpy(dest, &size, sizeof(size));
  char* charDest = static_cast<char*>(dest) + sizeof(size);
  memcpy(charDest, str.data(), str.length());
  return charDest + str.length();
}

#endif  // STORE_HELPERS_H
