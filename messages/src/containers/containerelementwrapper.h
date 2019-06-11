#ifndef CONTAINERELEMENTWRAPPER_H
#define CONTAINERELEMENTWRAPPER_H
#include <cassert>
#include <memory>
#include <mutex>

/**
 * Шаблонный класс-обертка для контейнеров. Не обеспечивает прозрачного доступа
 * к методам оборачиваемого элемента. Оборачиваемый элемент должен иметь
 * конструктор копирования.
 */
template <typename C>
class ContainerElementWrapper {
 private:
  struct nested_class {
    int empty_field;
  };

 public:
  using element = typename C::element;

 public:
  /**
   * @brief Конструктор класса
   * @param container контейнер, в который будет выполняться сохранение элемента
   * @param elem оборачиваемый элемент
   */
  ContainerElementWrapper(std::shared_ptr<C> container, const element& elem);

  /**
   * @brief Конструктор перемещения
   * @param elem обертка, для которой выполняется перемещение
   */
  ContainerElementWrapper(ContainerElementWrapper<C>&& elem) noexcept;

 public:
  /**
   * @brief Сохраняет внесенные изменения в объект
   * @return true, если изменения прошли успешно, в противном случае - false
   */
  bool save();

  /**
   * @brief удаляет оборачиваемый элемент из контейнера
   * @return  true, если изменения прошли успешно, в противном случае - false
   */
  bool remove();

  /**
   * @brief загружает последние изменения элемента из контейнера
   */
  void reload();

  /**
   * @brief Возвращает ссылку на оборачиваемый элемент, для получения доступа к
   * его методам.
   * @return  ссылку на элемент
   */
  element& get();

  operator const int nested_class::*() const {
    return mStateOk ? &nested_class::empty_field : nullptr;
  }

 public:
  ContainerElementWrapper(ContainerElementWrapper&) = delete;
  ContainerElementWrapper& operator=(const ContainerElementWrapper&) = delete;
  ContainerElementWrapper& operator=(const ContainerElementWrapper&&) = delete;

 protected:
  element mElement;

 private:
  bool mStateOk{true};
  std::shared_ptr<C> mContainer;
};

template <typename C>
ContainerElementWrapper<C>::ContainerElementWrapper(
    std::shared_ptr<C> container,
    const ContainerElementWrapper::element& elem) :
    mElement(make_element_copy(elem)),
    mContainer(std::move(container)) {}

template <typename C>
ContainerElementWrapper<C>::ContainerElementWrapper(
    ContainerElementWrapper<C>&& elem) noexcept :
    mElement(std::move(elem.mElement)),
    mContainer(std::move(elem.mContainer)) {}

template <typename C>
bool ContainerElementWrapper<C>::save() {
  try {
    return mContainer->strictUpdate(mElement);
  } catch (const std::exception&) {
    mStateOk = false;
  }
  return false;
}

template <typename C>
bool ContainerElementWrapper<C>::remove() {
  try {
    return mContainer->remove(mElement);
  } catch (const std::exception&) {
    mStateOk = false;
  }
  return false;
}

template <typename C>
void ContainerElementWrapper<C>::reload() {
  try {
    mElement = mContainer->get(get_id(mElement));
  } catch (const std::exception&) {
    mStateOk = false;
  }
}

template <typename C>
typename ContainerElementWrapper<C>::element&
ContainerElementWrapper<C>::get() {
  return mElement;
}

template <typename T>
T make_element_copy(const T& element) {
  return T(element);
}

template <typename T>
std::shared_ptr<T> make_element_copy(const std::shared_ptr<T>& element) {
  return std::make_shared<T>(*element);
}

template <typename T>
std::unique_ptr<T> make_element_copy(const std::unique_ptr<T>& element) {
  return std::make_unique<T>(*element);
}

template <typename T>
T* make_element_copy(T* element) {
  return new T(*element);
}

#endif  // CONTAINERELEMENTWRAPPER_H
