#ifndef DISCONECTEDEXCEPTION_H
#define DISCONECTEDEXCEPTION_H

#include <stdexcept>

class DisconectedException : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

#endif  // DISCONECTEDEXCEPTION_H
