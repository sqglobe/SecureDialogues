#include "fakeuserask.h"

FakeUserAsk::FakeUserAsk(bool retVal) : mRetVal(retVal) {}

bool FakeUserAsk::ask([[maybe_unused]] const std::string& question) const {
  return mRetVal;
}
