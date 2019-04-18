#include "contactgasket.h"

ContactGasket::ContactGasket(
    std::unique_ptr<DialogWidgetGasket<ContactContainer, ContactWidget>>&&
        gasket,
    QObject* parent) :
    QObject(parent),
    mGasket(std::move(gasket)) {}

void ContactGasket::onActionViewAt(int pos) {
  mGasket->viewAt(pos);
}

void ContactGasket::onActionRemoveAt(int pos) {
  mGasket->removeAt(pos);
}

void ContactGasket::onActionUpdateAt(int) {
  mGasket->update();
}

void ContactGasket::onActionAdd() {
  mGasket->add();
}
