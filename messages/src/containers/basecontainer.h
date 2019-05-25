#ifndef BASECONTAINER_H
#define BASECONTAINER_H

#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <memory>
#include <mutex>
#include <vector>
#include "transparentcontainerelementwrapper.h"

/*
 * Needed realisation of functions:
 *  std::string get_id(ConstElement element );
 *
 */

template <typename C>
class ChangeWatcher;

/**
 * Шаблонный класс, который служит для хранения различных объектов: диалогов,
 * контактов и пр. Обеспечивает добавление, удаление элементов. Поиск по
 * идентификатору, поиск по предикату и пр. Так же обеспечивается возможность
 * добавить обработчики событий о добавлении/удалении/изменении элементов.
 *
 * Обеспечивает способ получения класса-обертки элемента. При создании обертки
 * хранимое значение элемента копируется, так что изменения выполненные через
 * прокси-класс не затрагивают сам контейнер. Обертка так же реализует
 * прозрачный доступ к элементам к элементам оборачиваемого объекта.
 * Оборачиваемое значение сохраняется только при вызове метода обертки.
 *
 * Необходимость создания такого прокси-класса - транзакционность изменений
 * объектов, для сохранения изменений нет надобности хранить изменяемое значение
 * и класс-контейнер
 *
 * @param Element - тип хранимого элемента
 * @param ConstElement - константный тип хранимого элемента (особенно актуально
 * при применении указателей) для передачи, например, в наблюдатели при
 * наступлении события изменения элемента.
 *
 * Для хранимых элементов необходимо реализовать свободную функцию
 *        std::string get_id(ConstElement element );
 * эта функция должна возвращать идентификатор объекта element.
 */
template <typename Element, typename ConstElement>
class BaseContainer : public std::enable_shared_from_this<
                          BaseContainer<Element, ConstElement>> {
 public:
  using element = Element;
  using const_element = ConstElement;
  using wrapper_type =
      TransparentContainerElementWrapper<BaseContainer<Element, ConstElement>>;

 private:
  typedef void (ChangeWatcher<const_element>::*WatherFunc)(
      const const_element&);

 public:
  /**
   * @brief Добавляется элемент
   * @param elem элемент, который необходимо добавить
   */
  void add(const element& elem);

  /**
   * @brief удаляет элемент по его идентификатору.
   * @param id идентификатор объекта, который необходимо удалить
   * @return true - если удаление прошло успешно, в противном случае - false.
   */
  bool remove(const std::string& id);

  /**
   * @brief Удаляет элемент по его индексу
   * @param pos индекс элемента для удаления
   * @return true при успешном удалении
   * @throws std::out_of_range если индекс превышает количество элементов
   */
  bool remove(int pos);

  /**
   * @brief Удаляет указанный элемент
   * @param elem - элемент, который необходимо удалить
   * @return  true - если удаление прошло успешно, в противном случае - false.
   */
  bool remove(const element& elem);

  /**
   * @brief Выполняет только обновление элемента путем перезаписи
   * Для успешного выполнения функции у объекта не должен изменяться
   * идентификатор.
   * @param elem - элемент, который необходимо обновить
   * @return true, если элемент находится в контейнере и обновлен, false - в
   * противном случае.
   */
  bool strictUpdate(const element& elem);

  /**
   * @brief Выполняет обновление, если элемент присутствует в хранилище, в
   * противном случае элемент добавляется.
   * @param elem элемент, который нужно обновить/добавить
   */
  void update(const element& elem);

  /**
   * @brief Добавляет слушатель событий удаления/добавления/изменения элементов
   * массива
   * @param watcher слушатель события, который нужно добавить.
   */
  void registerWatcher(
      const std::shared_ptr<ChangeWatcher<const_element>>& watcher);

  /**
   * @brief Удаляет указанный слушатель события
   * @param watcher слушатель, который необходимо удалить.
   */
  void removeWatcher(
      const std::shared_ptr<ChangeWatcher<const_element>>& watcher);

  /**
   * @brief Регистрирует слушателя.
   * В этом методе регистрируется слушатель, управление жизненным циклом
   * которого осуществляется внешним кодом. Т.е. контейнер хранит слабый
   * указатель на такой наблюдатель, если объект был удален, то удаляется и сам
   * указатель. Для такого указателя не вызывается метода контейнера для
   * удаления из контейнера.
   * @param watcher слушатель, который необходимо добавить.
   */
  void registerTemporaryWatcher(
      const std::shared_ptr<ChangeWatcher<const_element>>& watcher);

  /**
   * @brief Возвращает класс обертку для указанного идентификатора.
   * Обертка инкапсулирует внутри контейнер и копию изменяемого объекта.
   * Для сохранения изменений достаточно выполнить метод обертки.
   * @param id идентификатор объекта, для которого необходимо создать обертку.
   * @return класс-обертку.
   * @throws std::out_of_range если указанный объект не найден
   */
  wrapper_type wrapper(const std::string& id);

  /**
   * @brief Создает класс обертку для объекта, который имеет индекс pos.
   * @param pos позиция элемента, для которого необходимо создать обертку.
   * @return возвращает класс-обертку
   * @throws std::out_of_range если параметр выходит за допустимые пределы
   */
  wrapper_type wrapper(int pos);

 public:
  element get(const std::string& id);
  const_element get(const std::string& id) const;
  element at(int pos);
  const_element at(int pos) const;
  bool has(const std::string& id) const;
  std::vector<const_element> getAllElements() const;
  int size() const noexcept;
  int posOf(const std::string& id) const;

  std::vector<const_element> get_if(
      std::function<bool(const const_element&)> p) const;

 protected:
  const_element find(std::function<bool(const element&)> is) const;

 private:
  typename std::vector<element>::const_iterator find(
      const std::string& id) const;
  void notify(const const_element& elem, WatherFunc func);

 protected:
  std::vector<element> mElements;
  mutable std::recursive_mutex mMutex;

 private:
  std::vector<std::shared_ptr<ChangeWatcher<const_element>>> mWatchers;
  std::list<std::weak_ptr<ChangeWatcher<const_element>>> mTemporaryWatchers;
};

/*-----------------------------------------------------------------------------------------------------*/

template <typename Element, typename ConstElement>
void BaseContainer<Element, ConstElement>::add(
    const BaseContainer::element& elem) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  mElements.push_back(elem);
  notify(elem, &ChangeWatcher<const_element>::added);
}

template <typename Element, typename ConstElement>
bool BaseContainer<Element, ConstElement>::remove(const std::string& id) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto it = find(id);
  if (it != mElements.cend()) {
    auto elem = *it;
    mElements.erase(it);
    notify(elem, &ChangeWatcher<const_element>::removed);
    return true;
  }
  return false;
}

template <typename Element, typename ConstElement>
bool BaseContainer<Element, ConstElement>::remove(int pos) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto elem = mElements.at(pos);
  mElements.erase(mElements.cbegin() + pos);
  notify(elem, &ChangeWatcher<const_element>::removed);
  return true;
}

template <typename Element, typename ConstElement>
bool BaseContainer<Element, ConstElement>::remove(
    const BaseContainer::element& elem) {
  return remove(get_id(elem));
}

template <typename Element, typename ConstElement>
bool BaseContainer<Element, ConstElement>::strictUpdate(
    const BaseContainer::element& elem) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto it = find(get_id(elem));
  if (it != mElements.cend()) {
    mElements[std::distance(mElements.cbegin(), it)] = elem;
    notify(elem, &ChangeWatcher<const_element>::changed);
    return true;
  }

  return false;
}

template <typename Element, typename ConstElement>
void BaseContainer<Element, ConstElement>::update(
    const BaseContainer::element& elem) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto it = find(get_id(elem));
  if (it != mElements.cend()) {
    mElements[std::distance(mElements.cbegin(), it)] = elem;
  } else {
    mElements.push_back(elem);
  }
  notify(elem, &ChangeWatcher<const_element>::changed);
}

template <typename Element, typename ConstElement>
void BaseContainer<Element, ConstElement>::registerWatcher(
    const std::shared_ptr<ChangeWatcher<BaseContainer::const_element>>&
        watcher) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  mWatchers.push_back(watcher);
}

template <typename Element, typename ConstElement>
void BaseContainer<Element, ConstElement>::removeWatcher(
    const std::shared_ptr<ChangeWatcher<BaseContainer::const_element>>&
        watcher) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto it = std::find(mWatchers.cbegin(), mWatchers.cend(), watcher);
  if (it != mWatchers.cend()) {
    mWatchers.erase(it);
  }
}

template <typename Element, typename ConstElement>
void BaseContainer<Element, ConstElement>::registerTemporaryWatcher(
    const std::shared_ptr<ChangeWatcher<BaseContainer::const_element>>&
        watcher) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  mTemporaryWatchers.push_back(watcher);
}

template <typename Element, typename ConstElement>
typename BaseContainer<Element, ConstElement>::wrapper_type
BaseContainer<Element, ConstElement>::wrapper(const std::string& id) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  return wrapper_type(this->shared_from_this(), get(id));
}

template <typename Element, typename ConstElement>
typename BaseContainer<Element, ConstElement>::wrapper_type
BaseContainer<Element, ConstElement>::wrapper(int pos) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  return wrapper_type(this->shared_from_this(), at(pos));
}

template <typename Element, typename ConstElement>
typename BaseContainer<Element, ConstElement>::element
BaseContainer<Element, ConstElement>::get(const std::string& id) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto pos = posOf(id);
  return at(pos);
}

template <typename Element, typename ConstElement>
typename BaseContainer<Element, ConstElement>::const_element
BaseContainer<Element, ConstElement>::get(const std::string& id) const {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto pos = posOf(id);
  return at(pos);
}

template <typename Element, typename ConstElement>
typename BaseContainer<Element, ConstElement>::element
BaseContainer<Element, ConstElement>::at(int pos) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  return mElements.at(pos);
}

template <typename Element, typename ConstElement>
typename BaseContainer<Element, ConstElement>::const_element
BaseContainer<Element, ConstElement>::at(int pos) const {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  return mElements.at(pos);
}

template <typename Element, typename ConstElement>
bool BaseContainer<Element, ConstElement>::has(const std::string& id) const {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto it = find(id);
  return mElements.cend() != it;
}

template <typename Element, typename ConstElement>
std::vector<typename BaseContainer<Element, ConstElement>::const_element>
BaseContainer<Element, ConstElement>::getAllElements() const {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  std::vector<const_element> res;
  std::copy(mElements.cbegin(), mElements.cend(), std::back_inserter(res));
  return res;
}

template <typename Element, typename ConstElement>
int BaseContainer<Element, ConstElement>::size() const noexcept {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  return mElements.size();
}

template <typename Element, typename ConstElement>
int BaseContainer<Element, ConstElement>::posOf(const std::string& id) const {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto it = find(id);
  if (mElements.cend() == it) {
    throw std::out_of_range("Not found element for " + id);
  }
  return std::distance(mElements.cbegin(), it);
}

template <typename Element, typename ConstElement>
typename BaseContainer<Element, ConstElement>::const_element
BaseContainer<Element, ConstElement>::find(
    std::function<bool(const BaseContainer::element&)> is) const {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto it = std::find_if(mElements.cbegin(), mElements.cend(), is);

  if (mElements.cend() != it) {
    return *it;
  }

  throw std::range_error("not found element");
}

template <typename Element, typename ConstElement>
typename std::vector<
    typename BaseContainer<Element, ConstElement>::element>::const_iterator
BaseContainer<Element, ConstElement>::find(const std::string& id) const {
  auto find_cond =
      [&id](const BaseContainer<Element, ConstElement>::element& el) {
        return id == get_id(el);
      };
  return std::find_if(mElements.cbegin(), mElements.cend(), find_cond);
}

template <typename Element, typename ConstElement>
void BaseContainer<Element, ConstElement>::notify(
    const BaseContainer::const_element& elem,
    BaseContainer::WatherFunc func) {
  for (auto watcher : mWatchers) {
    ((*watcher).*func)(elem);
  }

  for (auto it = mTemporaryWatchers.cbegin();
       it != mTemporaryWatchers.cend();) {
    if (auto watcher = (*it).lock()) {
      ((*watcher).*func)(elem);
      ++it;
    } else {
      it = mTemporaryWatchers.erase(it);
    }
  }
}

template <typename Element, typename ConstElement>
std::vector<typename BaseContainer<Element, ConstElement>::const_element>
BaseContainer<Element, ConstElement>::get_if(
    std::function<bool(const const_element&)> p) const {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  std::vector<typename BaseContainer<Element, ConstElement>::const_element> res;
  std::copy_if(std::cbegin(mElements), std::cend(mElements),
               std::back_inserter(res), p);
  return res;
}

#endif  // BASECONTAINER_H
