#ifndef FAKEUSERASK_H
#define FAKEUSERASK_H
#include <string>
#include "interfaces/abstractuserask.h"
class FakeUserAsk : public AbstractUserAsk {
 public:
  FakeUserAsk(bool retVal);
  virtual bool ask(const std::string& question) const override;

 private:
  bool mRetVal;
};

#endif  // FAKEUSERASK_H
