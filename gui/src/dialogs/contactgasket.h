#ifndef CONTACTGASKET_H
#define CONTACTGASKET_H

#include <QObject>
#include <memory>

#include "containers/contactcontainer.h"
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
  ContactGasket(
      std::unique_ptr<DialogWidgetGasket<ContactContainer, ContactWidget>>&&
          gasket,
      QObject* parent = nullptr);
  ~ContactGasket() = default;

 public slots:
  void onActionViewAt(int pos);
  void onActionRemoveAt(int pos);
  void onActionUpdateAt(int);
  void onActionAdd();

 private:
  std::unique_ptr<DialogWidgetGasket<ContactContainer, ContactWidget>> mGasket;
};

#endif  // CONTACTGASKET_H
