#ifndef CHANGEWATCHER_H
#define CHANGEWATCHER_H

/**
 *  Шаблонный класс, который используется для обработки
 * событий контейнера, такие как удаление объекта,
 * добавление, изменение.
 * Параметр шаблона определяется типом объекта, который хранится в конейнере
 */
template <typename C>
class ChangeWatcher {
 public:
  using element = C;

 public:
  virtual ~ChangeWatcher() = default;

 public:
  /**
   * @brief Вызывается, когда в контейнер был добавлен элемент
   * @param obj ссылка на добавленный элемент
   */
  virtual void added(const element& obj) = 0;

  /**
   * @brief Вызывается, когда в конейнере был изменен элемент
   * @param obj ссылка на измененный объект
   */
  virtual void changed(const element& obj) = 0;

  /**
   * @brief вызывается, когда из контейнера был удален элемент
   * @param obj ссылка на удаленный элемент
   */
  virtual void removed(const element& obj) = 0;
};

#endif  // CHANGEWATCHER_H
