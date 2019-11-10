#ifndef CONNECTIONINFOGASKET_H
#define CONNECTIONINFOGASKET_H

#include <QObject>
#include <memory>

#include "containers/storages.h"
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
      std::unique_ptr<
          DialogWidgetGasket<ConnectionStorage, ConnectionInfoWidget>>&& gasket,
      QObject* parent = nullptr);

 public slots:
  void onActionViewAt(std::string id);
  void onActionRemoveAt(std::string id);
  void onActionUpdateAt(std::string);
  void onActionAdd();

 private:
  std::unique_ptr<DialogWidgetGasket<ConnectionStorage, ConnectionInfoWidget>>
      mGasket;
};

#endif  // CONNECTIONINFOGASKET_H
