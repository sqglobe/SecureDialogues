#ifndef STORE_PRIMITIVES_H
#define STORE_PRIMITIVES_H

#include <string>

const void* restore_str(std::string& str, const void* src);
void* save_str(const std::string& str, void* dest);

#endif  // STORE_PRIMITIVES_H
