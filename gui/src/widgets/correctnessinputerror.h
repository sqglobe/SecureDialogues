#ifndef CORRECTNESSINPUTERROR_H
#define CORRECTNESSINPUTERROR_H
#include <stdexcept>
class CorrectnessInputError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};
#endif  // CORRECTNESSINPUTERROR_H
