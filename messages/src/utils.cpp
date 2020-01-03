#include "utils.h"
#include <chrono>
#include "crossguid/Guid.hpp"
#include "oauth-agents/utils/base64.h"

std::string make_uiid() {
  std::string result = xg::newGuid().str();
  return std::string(base64_encode(
      reinterpret_cast<const unsigned char*>(result.c_str()), result.size()));
}

long get_timestamp() {
  auto now = std::chrono::system_clock::now();
  auto cnt = now.time_since_epoch();
  return std::chrono::duration_cast<std::chrono::seconds>(cnt).count();
}
