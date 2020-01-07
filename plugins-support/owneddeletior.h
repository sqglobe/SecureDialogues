#ifndef OWNEDDELETIOR_H
#define OWNEDDELETIOR_H
#include <memory>

#include "export/plaginfacade.h"

namespace plugin_support {

class PluginInterface;

class owned_deletor {
  friend PluginInterface;

 private:
  owned_deletor(std::shared_ptr<const PluginInterface> interface,
                PlaginFacade* facade) :
      mInterface(std::move(interface)),
      mFacade(facade) {}

 public:
  owned_deletor(owned_deletor&& obj) :
      mInterface(std::move(obj.mInterface)), mFacade(std::move(obj.mFacade)) {}
  owned_deletor& operator=(owned_deletor&& obj) {
    mInterface = std::move(obj.mInterface);
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
  std::shared_ptr<const PluginInterface> mInterface;
  PlaginFacade* mFacade;
};

}  // namespace plugin_support
#endif  // OWNEDDELETIOR_H
