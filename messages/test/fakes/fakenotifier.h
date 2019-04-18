#ifndef FAKENOTIFIER_H
#define FAKENOTIFIER_H
#include "interfaces/abstractusernotifier.h"

class FakeNotifier : public AbstractUserNotifier {
 public:
  virtual void notify(Severity sev, const std::string& message) override {
    mSev = sev;
    mMessage = message;
  }

 public:
  Severity mSev;
  std::string mMessage;
};

#endif  // FAKENOTIFIER_H
