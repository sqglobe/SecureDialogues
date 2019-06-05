#ifndef FAKENOTIFIER_H
#define FAKENOTIFIER_H
#include "interfaces/abstractusernotifier.h"

#include <iostream>

class FakeNotifier : public AbstractUserNotifier {
 public:
  virtual void notify(Severity sev, const std::string& message) override {
    mSev = sev;
    mMessage = message;
    std::cout << "FakeNotifier:" << static_cast<int>(sev) << ": " << message
              << std::endl;
  }

 public:
  Severity mSev;
  std::string mMessage;
};

#endif  // FAKENOTIFIER_H
