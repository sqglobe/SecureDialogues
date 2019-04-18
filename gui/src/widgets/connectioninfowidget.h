#ifndef CONNECTIONINFOWIDGET_H
#define CONNECTIONINFOWIDGET_H

#include <QWidget>
#include <memory>
class ConnectionHolder;
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
  ~ConnectionInfoWidget();

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
  std::unique_ptr<ConnectionHolder> mInfo;
};

#endif  // CONNECTIONINFOWIDGET_H
