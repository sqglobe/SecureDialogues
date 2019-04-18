#ifndef SENDEXCEPTION_H
#define SENDEXCEPTION_H

#include <stdexcept>

class SendException : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};
#endif  // SENDEXCEPTION_H
