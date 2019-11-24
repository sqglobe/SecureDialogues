#ifndef CONTACTGASKET_H
#define CONTACTGASKET_H

#include <QObject>
#include <memory>

#include "containers/storages.h"
#include "dialogwidgetgasket.h"
#include "widgets/contactwidget.h"

/**
 * @brief Класс-прокладка между QObject и полной специализацией шаблонного
 * класса DialogWidgetGasket Используется для того, чтобы можно было применить
 * систему сигналов-слотов.
 */
class ContactGasket : public QObject {
  Q_OBJECT

 public:
  ContactGasket(std::unique_ptr<
                    DialogWidgetGasket<ContactStorage, ContactWidget>>&& gasket,
                QObject* parent = nullptr);

 public slots:
  void onActionViewAt(std::string id);
  void onActionRemoveAt(std::string id);
  void onActionUpdateAt(std::string);
  void onActionAdd();

 private:
  std::unique_ptr<DialogWidgetGasket<ContactStorage, ContactWidget>> mGasket;
};

#endif  // CONTACTGASKET_H
