#ifndef CONNECTIONINFOWIDGET_H
#define CONNECTIONINFOWIDGET_H

#include <QWidget>
#include <optional>
#include "communication/channel.h"
#include "primitives/connectionholder.h"

namespace Ui {
class ConnectionInfoWidget;
}

/**
 * @brief Widget содержит поля, через которые пользователь может изменить
 * конкретное подключение Используется в диалоге управления подключениями
 */
class ConnectionInfoWidget : public QWidget {
  Q_OBJECT

 public:
  explicit ConnectionInfoWidget(QWidget* parent = nullptr);
  ~ConnectionInfoWidget() override;

 public:
  ConnectionInfoWidget(const ConnectionInfoWidget&) = delete;
  ConnectionInfoWidget& operator=(const ConnectionInfoWidget&) = delete;

 public:
  /**
   * @brief Передает виджету подключение, которое необходимо отобразить
   * @param info подключение для отображения
   */
  void setElement(const ConnectionHolder& info);

  /**
   * @brief Получает отредактированное пользователем подключение
   * @return отредактированное подключение
   */
  ConnectionHolder getElement() noexcept(false);

 public:
  void updateChannelStatus(Channel::ChannelStatus status,
                           const std::string& channelName,
                           const std::string& message);
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

  /**
   * @brief Обработчик события выбора типа подключения
   * @param index тип подключения
   */
  void activatedConnectionType(int index);

 signals:
  void cleareVal();
  void changeEnabled(bool isEnabled);

 protected:
  Ui::ConnectionInfoWidget* ui;

 private:
  struct Element {
    Channel::ChannelStatus status;
    QString message;
  };

  Element getChannelInfo(const std::string& channelName) const;
  void displayStatus(const Element& element);

 private:
  std::optional<ConnectionHolder> mInfo;
  std::map<std::string, Element> mCachedElements;
};

#endif  // CONNECTIONINFOWIDGET_H
