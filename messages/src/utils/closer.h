#ifndef CLOSER_H
#define CLOSER_H

#include <type_traits>

template <typename T, typename F>
struct check_close_prototype {
  static void detect_close(...);
  template <typename U,
            typename = decltype(std::declval<F>()(std::declval<U>()))>
  static int detect_close(U);

  const static bool value =
      std::is_same<int, decltype(detect_close(std::declval<T>()))>::value;
};

template <typename V, typename C>
class Closer {
 public:
  Closer(V param, C* close);
  ~Closer();

 public:
  operator V() const;

 private:
  V mParam;
  C* mClose;
};

template <typename V, typename C>
inline Closer<V, C>::Closer(V param, C* close) : mParam(param), mClose(close) {
  static_assert(check_close_prototype<V, C>::value,
                "needed to define close funtion(C) with parameter V");
}

template <typename V, typename C>
inline Closer<V, C>::~Closer() {
  mClose(mParam);
}

template <typename V, typename C>
inline Closer<V, C>::operator V() const {
  return mParam;
}

#endif  // CLOSER_H
