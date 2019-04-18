#ifndef RECEIVEEXCEPTION_H
#define RECEIVEEXCEPTION_H

#include <stdexcept>

class ReceiveException : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};
#endif  // RECEIVEEXCEPTION_H
