#ifndef CHANGELISTENER_H
#define CHANGELISTENER_H

template <typename Element>
class ChangeListener {
 public:
  using element = Element;

 public:
  virtual ~ChangeListener() = default;

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

#endif  // CHANGELISTENER_H
