#ifndef EVENTWATCHERS_H
#define EVENTWATCHERS_H

#include <list>
#include <memory>
#include <mutex>

#include "interfaces/changelistener.h"

template <typename Element>
class EventListeners {
 private:
  using WatherFunc = void (ChangeListener<Element>::*)(const Element&);

 public:
  void appendListener(std::weak_ptr<ChangeListener<Element>> listener);
  void appendPermanentListener(
      std::shared_ptr<ChangeListener<Element>> listener);

  void removeListener(std::shared_ptr<ChangeListener<Element>> listener);

 protected:
  void elementAdded(const Element& element);
  void elementRemoved(const Element& element);
  void elementUpdated(const Element& element);

 private:
  void notify(const Element& elem, WatherFunc func);

 private:
  std::mutex mMutex;
  std::list<std::weak_ptr<ChangeListener<Element>>> mListeners;
  std::list<std::shared_ptr<ChangeListener<Element>>> mPermanentListeners;
};

template <typename Element>
void EventListeners<Element>::appendListener(
    std::weak_ptr<ChangeListener<Element>> listener) {
  [[maybe_unused]] std::lock_guard<std::mutex> lock(mMutex);
  mListeners.emplace_back(std::move(listener));
}

template <typename Element>
void EventListeners<Element>::appendPermanentListener(
    std::shared_ptr<ChangeListener<Element>> listener) {
  [[maybe_unused]] std::lock_guard<std::mutex> lock(mMutex);
  mPermanentListeners.emplace_back(std::move(listener));
}

template <typename Element>
void EventListeners<Element>::removeListener(
    std::shared_ptr<ChangeListener<Element>> listener) {
  [[maybe_unused]] std::lock_guard<std::mutex> lock(mMutex);
  mPermanentListeners.remove(listener);
}

template <typename Element>
void EventListeners<Element>::elementAdded(const Element& element) {
  notify(element, &ChangeListener<Element>::added);
}

template <typename Element>
void EventListeners<Element>::elementRemoved(const Element& element) {
  notify(element, &ChangeListener<Element>::removed);
}

template <typename Element>
void EventListeners<Element>::elementUpdated(const Element& element) {
  notify(element, &ChangeListener<Element>::changed);
}

template <typename Element>
void EventListeners<Element>::notify(const Element& elem,
                                     EventListeners::WatherFunc func) {
  for (const auto& listener : mPermanentListeners) {
    ((*listener).*func)(elem);
  }

  for (auto it = mListeners.cbegin(); it != mListeners.cend();) {
    if (auto watcher = (*it).lock()) {
      ((*watcher).*func)(elem);
      ++it;
    } else {
      it = mListeners.erase(it);
    }
  }
}

#endif  // EVENTWATCHERS_H
