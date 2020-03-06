#ifndef SUPPORTFUNCTIONS_H
#define SUPPORTFUNCTIONS_H

#include <string>
namespace plugin_support {

inline std::string make_string(const char* str) {
  return str == nullptr ? "" : str;
}

}  // namespace plugin_support
#endif  // SUPPORTFUNCTIONS_H
