#ifndef CURLEXCEPTIONS_H
#define CURLEXCEPTIONS_H

#include <stdexcept>

class CurlEscapeError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class CurlInitError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class CurlHttpSendError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class CurlHttpHeaderError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};
#endif  // CURLEXCEPTIONS_H
