#include "oauth-exceptions.h"

std::string getErrorCodeMessage(HttpCode code) {
  switch (code) {
    case HttpCode::INTERNAL_SERVER_ERROR:
      return "Internal Server Error";
    case HttpCode::UNAUTHORIZED:
      return "Unauthorized";
  }
  return "Undefined HTTP error " + std::to_string(static_cast<int>(code));
}

HttpError::HttpError(HttpCode code) :
    mCode(code), mMessage(getErrorCodeMessage(code)) {}

const char* HttpError::what() const noexcept {
  return mMessage.c_str();
}

HttpCode HttpError::code() const noexcept {
  return mCode;
}
