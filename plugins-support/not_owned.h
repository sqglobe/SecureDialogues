#ifndef NOT_OWNED_H
#define NOT_OWNED_H

#include <memory>

namespace plugin_support {

class PluginInterface;

template <typename T>
class not_owned {
  friend PluginInterface;

 private:
  not_owned(std::shared_ptr<const PluginInterface> interface, T* obj) :
      mInterface(std::move(interface)), mObject(obj) {}

 public:
  not_owned(not_owned&& obj) :
      mInterface(std::move(obj.mInterface)), mObject(std::move(obj.mObject)) {}
  not_owned& operator=(not_owned&& obj) {
    this->mInterface = std::move(obj.mInterface);
    this->mObject = obj.mObject;
    obj.mObject = nullptr;
    return &this;
  }

  not_owned(const not_owned&) = delete;
  not_owned& operator=(const not_owned&) = delete;

 public:
  T* get() const noexcept { return mObject; }
  T* operator->() const noexcept { return mObject; }

 private:
  std::shared_ptr<const PluginInterface> mInterface;
  T* mObject;
};

}  // namespace plugin_support

#endif  // NOT_OWNED_H
