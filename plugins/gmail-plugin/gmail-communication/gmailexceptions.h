#ifndef GMAILEXCEPTIONS_H
#define GMAILEXCEPTIONS_H

#include <stdexcept>

class NotAuthorisedException : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

#endif  // GMAILEXCEPTIONS_H
