#ifndef FAKETEMPLATECHANGEWATCHER_H
#define FAKETEMPLATECHANGEWATCHER_H

#include <optional>
#include <string>
#include "interfaces/changelistener.h"

template <typename T>
class FakeTemplateChangeListener : public ChangeListener<T> {
 public:
  void added(const T& obj) override;
  void changed(const T& obj) override;
  void removed(const T& obj) override;

 public:
  std::string mMethod;
  std::optional<T> mVal;
};
template <typename T>
void FakeTemplateChangeListener<T>::added(const T& obj) {
  mMethod = "added";
  mVal = obj;
}

template <typename T>
void FakeTemplateChangeListener<T>::changed(const T& obj) {
  mMethod = "changed";
  mVal = obj;
}

template <typename T>
void FakeTemplateChangeListener<T>::removed(const T& obj) {
  mMethod = "removed";
  mVal = obj;
}
#endif  // FAKETEMPLATECHANGEWATCHER_H
