#ifndef TRANSPARENTCONTAINERELEMENTWRAPPER_H
#define TRANSPARENTCONTAINERELEMENTWRAPPER_H
#include <type_traits>
#include "containerelementwrapper.h"

/**
 * Добавляет в прокси-класс элементов контейнеров возможность прозрачного
 * доступа к методам класса через оператор ->
 */
template <typename C, typename = void>
class TransparentContainerElementWrapper : public ContainerElementWrapper<C> {
 public:
  using ContainerElementWrapper<C>::ContainerElementWrapper;

 public:
  typename C::element* operator->() { return &(this->mElement); }
  typename C::const_element const* operator->() const {
    return &(this->mElement);
  }
};

/**
 * Реализация для простых указателей
 */
template <typename C>
class TransparentContainerElementWrapper<
    C,
    typename std::enable_if<std::is_pointer<typename C::element>::value>::type>
    : public ContainerElementWrapper<C> {
 public:
  using ContainerElementWrapper<C>::ContainerElementWrapper;

 public:
  typename C::element operator->() { return this->mElement; }
  typename C::const_element operator->() const { return this->mElement; }
};

/**
 * Реализация для умных указателей
 */
template <typename C>
class TransparentContainerElementWrapper<
    C,
    typename std::enable_if<std::is_member_function_pointer<decltype(
        &C::element::operator->)>::value>::type>
    : public ContainerElementWrapper<C> {
 public:
  using ContainerElementWrapper<C>::ContainerElementWrapper;

 public:
  typename C::element& operator->() { return this->mElement; }
  typename C::const_element operator->() const { return this->mElement; }
};

#endif  // TRANSPARENTCONTAINERELEMENTWRAPPER_H
