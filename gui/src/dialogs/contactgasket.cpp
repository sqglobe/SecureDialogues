#include "contactgasket.h"

ContactGasket::ContactGasket(
    std::unique_ptr<DialogWidgetGasket<ContactStorage, ContactWidget>>&& gasket,
    QObject* parent) :
    QObject(parent),
    mGasket(std::move(gasket)) {}

void ContactGasket::onActionViewAt(std::string id) {
  mGasket->viewAt(id);
}

void ContactGasket::onActionRemoveAt(std::string id) {
  mGasket->removeAt(id);
}

void ContactGasket::onActionUpdateAt(std::string) {
  mGasket->update();
}

void ContactGasket::onActionAdd() {
  mGasket->add();
}
