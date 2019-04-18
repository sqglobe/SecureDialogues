#ifndef CONNECTIONINFOGASKET_H
#define CONNECTIONINFOGASKET_H

#include <QObject>
#include <memory>

#include "containers/connectioninfocontainer.h"
#include "dialogwidgetgasket.h"
#include "widgets/connectioninfowidget.h"

/**
 * @brief Класс-прокладка между QObject и полной специализацией шаблонного
 * класса DialogWidgetGasket Используется для того, чтобы можно было применить
 * систему сигналов-слотов.
 */
class ConnectionInfoGasket : public QObject {
  Q_OBJECT

 public:
  ConnectionInfoGasket(
      std::unique_ptr<DialogWidgetGasket<ConnectionInfoContainer,
                                         ConnectionInfoWidget>>&& gasket,
      QObject* parent = nullptr);
  ~ConnectionInfoGasket() = default;

 public slots:
  void onActionViewAt(int pos);
  void onActionRemoveAt(int pos);
  void onActionUpdateAt(int);
  void onActionAdd();

 private:
  std::unique_ptr<
      DialogWidgetGasket<ConnectionInfoContainer, ConnectionInfoWidget>>
      mGasket;
};

#endif  // CONNECTIONINFOGASKET_H
