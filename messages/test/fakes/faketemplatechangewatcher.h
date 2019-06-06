#ifndef FAKETEMPLATECHANGEWATCHER_H
#define FAKETEMPLATECHANGEWATCHER_H

#include <memory>
#include <string>
#include "interfaces/changewatcher.h"

template <typename T>
class FakeTemplateChangeWatcher : public ChangeWatcher<T> {
 public:
  void added(const T& obj) override;
  void changed(const T& obj) override;
  void removed(const T& obj) override;

 public:
  std::string mMethod;
  std::unique_ptr<T> mVal;
};
template <typename T>
void FakeTemplateChangeWatcher<T>::added(const T& obj) {
  mMethod = "added";
  mVal.reset(new T(obj));
}

template <typename T>
void FakeTemplateChangeWatcher<T>::changed(const T& obj) {
  mMethod = "changed";
  mVal.reset(new T(obj));
}

template <typename T>
void FakeTemplateChangeWatcher<T>::removed(const T& obj) {
  mMethod = "removed";
  mVal.reset(new T(obj));
}
#endif  // FAKETEMPLATECHANGEWATCHER_H
