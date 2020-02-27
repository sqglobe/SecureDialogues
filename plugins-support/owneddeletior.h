#ifndef OWNEDDELETIOR_H
#define OWNEDDELETIOR_H
#include <memory>

#include "export/pluginfacade.h"

namespace plugin_support {

class owned_deletor {
 public:
  owned_deletor(PluginFacade* facade) : mFacade(facade) {}
  owned_deletor(owned_deletor&& obj) : mFacade(std::move(obj.mFacade)) {}
  owned_deletor& operator=(owned_deletor&& obj) {
    mFacade = std::move(obj.mFacade);
    return *this;
  }

 public:
  owned_deletor(const owned_deletor&) = delete;
  owned_deletor& operator=(const owned_deletor&) = delete;

 public:
  template <typename T>
  void operator()(T* obj) noexcept {
    mFacade->release(obj);
  }

 private:
  PluginFacade* mFacade;
};

}  // namespace plugin_support
#endif  // OWNEDDELETIOR_H
