#ifndef OAUTHEXCEPTIONS_H
#define OAUTHEXCEPTIONS_H

#include <stdexcept>
#include <string>
#include "httpcode.h"

class AuthorizedFailed : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class ApiCommunicationFailed : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class ApiCallError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class HttpError : public std::exception {
 public:
  HttpError(HttpCode code);

 public:
  virtual const char* what() const noexcept override;
  HttpCode code() const noexcept;

 private:
  HttpCode mCode;
  std::string mMessage;
};

#endif  // OAUTHEXCEPTIONS_H
