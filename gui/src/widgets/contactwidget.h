#ifndef CONTACTWIDGET_H
#define CONTACTWIDGET_H

#include <QWidget>
#include <memory>
#include "pluginscontainer.h"
#include "primitives/contact.h"

class ChannelsListModel;

namespace Ui {
class ContactWidget;
}

/**
 * @brief Widget содержит поля, через которые пользователь может изменить
 * конкретный контакт Используется в диалоге управления контактами
 */
class ContactWidget : public QWidget {
  Q_OBJECT

 public:
  explicit ContactWidget(std::shared_ptr<ChannelsListModel> model,
                         QWidget* parent = nullptr);
  ~ContactWidget() override;

 public:
  ContactWidget(const ContactWidget&) = delete;
  ContactWidget operator=(const ContactWidget&) = delete;

 public:
  /**
   * @brief Передает виджету контакт, который необходимо отобразить
   * @param info контакт для отображения
   */
  void setElement(const Contact& info);

  /**
   * @brief Получает отредактированный пользователем контакт
   * @return отредактированный контакт
   */
  Contact getElement() noexcept(false);

 public slots:
  /**
   * @brief Выполняет очистку полей виджета
   */
  void actionCleare();

  /**
   * @brief Поля виджета становятся доступными для редактирования
   */
  void actionEnable();

  /**
   * @brief Поля виджета становятся не доступными для редактирования
   */
  void actionDisable();

 public slots:
  void connectionNameUpdated(int);

 signals:
  void cleareVal();
  void changeEnabled(bool isEnabled);

 protected:
  Ui::ContactWidget* ui;

 private:
  std::shared_ptr<ChannelsListModel> mModel;
  std::optional<Contact> mContact;
};

#endif  // CONTACTWIDGET_H
