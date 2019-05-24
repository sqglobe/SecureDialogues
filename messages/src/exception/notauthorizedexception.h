#ifndef NOTAUTHORIZEDEXCEPTION_H
#define NOTAUTHORIZEDEXCEPTION_H

#include <stdexcept>

class NotAuthorizedException : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

#endif  // NOTAUTHORIZEDEXCEPTION_H
