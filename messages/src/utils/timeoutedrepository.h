#ifndef TIMEOUTEDREPOSITORY_H
#define TIMEOUTEDREPOSITORY_H
#include <chrono>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>

/**
 * Для использования данного класса
 * необходимо определить функции:
 *  removed(const T &) - вызывается при удалении элемента из
 *                   хранилища (когда таймаут еще не вышел)
 *  timeouted(const T&) - вызывается, когда вышел таймаут хранения
 */
template <typename T, typename K>
class TimeoutedRrepository {
 private:
  struct StoredElement {
    T element;
    std::chrono::system_clock::time_point validTo;
    template <typename R>
    StoredElement(R&& elem,
                  const std::chrono::system_clock::time_point& valid) :
        element(std::forward<R>(elem)),
        validTo(valid) {}
  };

 public:
  static inline std::shared_ptr<TimeoutedRrepository<T, K>> make(
      const std::chrono::seconds& timeout = std::chrono::seconds(5),
      const std::chrono::milliseconds& check = std::chrono::milliseconds(30));

 private:
  static inline void thread_execute(
      std::weak_ptr<TimeoutedRrepository<T, K>>&& shared);

  TimeoutedRrepository(const std::chrono::seconds& timeout,
                       const std::chrono::milliseconds& check);

 public:
  template <typename R>
  inline bool store(R&& obj, K&& key);
  template <typename R>
  inline bool store(R&& obj, K&& key, const std::chrono::seconds& timeout);
  inline bool remove(const K& k);

 private:
  inline void checkInThread();

 private:
  const std::chrono::seconds mDefaultTimeout;
  const std::chrono::milliseconds mCheckPeriod;
  std::map<K, StoredElement> mValues;
  std::mutex mMutex;
};

template <typename T, typename K>
std::shared_ptr<TimeoutedRrepository<T, K>> TimeoutedRrepository<T, K>::make(
    const std::chrono::seconds& timeout,
    const std::chrono::milliseconds& check) {
  auto res = std::shared_ptr<TimeoutedRrepository<T, K>>(
      new TimeoutedRrepository(timeout, check));
  std::thread th(&TimeoutedRrepository<T, K>::thread_execute,
                 std::weak_ptr<TimeoutedRrepository<T, K>>(res));
  th.detach();
  return res;
}

template <typename T, typename K>
void TimeoutedRrepository<T, K>::thread_execute(
    std::weak_ptr<TimeoutedRrepository<T, K>>&& shared) {
  std::weak_ptr<TimeoutedRrepository<T, K>> weak(shared);
  while (auto lk = weak.lock()) {
    std::this_thread::sleep_for(lk->mCheckPeriod);
    lk->checkInThread();
  }
}

template <typename T, typename K>
TimeoutedRrepository<T, K>::TimeoutedRrepository(
    const std::chrono::seconds& timeout,
    const std::chrono::milliseconds& check) :
    mDefaultTimeout(timeout),
    mCheckPeriod(check) {}

template <typename T, typename K>
template <typename R>
bool TimeoutedRrepository<T, K>::store(R&& obj, K&& key) {
  return store(std::forward<R>(obj), std::forward<K>(key), mDefaultTimeout);
}

template <typename T, typename K>
template <typename R>
bool TimeoutedRrepository<T, K>::store(R&& obj,
                                       K&& key,
                                       const std::chrono::seconds& timeout) {
  std::lock_guard<std::mutex> lock(mMutex);
  if (mValues.count(key) > 0)
    return false;

  auto end = std::chrono::system_clock::now() + timeout;

  mValues.emplace(std::piecewise_construct, std::forward_as_tuple(key),
                  std::forward_as_tuple(std::forward<R>(obj), end));
  return true;
}

template <typename T, typename K>
bool TimeoutedRrepository<T, K>::remove(const K& k) {
  std::lock_guard<std::mutex> lock(mMutex);
  auto it = mValues.find(k);

  if (mValues.end() == it)
    return false;

  removed(it->second.element);

  mValues.erase(it);
  return true;
}

template <typename T, typename K>
void TimeoutedRrepository<T, K>::checkInThread() {
  auto now = std::chrono::system_clock::now();

  std::lock_guard<std::mutex> lock(mMutex);
  for (auto it = mValues.begin(); it != mValues.end();) {
    if (it->second.validTo > now) {
      ++it;
    } else {
      auto rm = it;
      ++it;
      timeouted(rm->second.element);
      mValues.erase(rm);
    }
  }
}

#endif  // TIMEOUTEDREPOSITORY_H
