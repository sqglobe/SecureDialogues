#include "connectioninfogasket.h"
#include "interfaces/changewatcher.h"

ConnectionInfoGasket::ConnectionInfoGasket(
    std::unique_ptr<DialogWidgetGasket<ConnectionInfoContainer,
                                       ConnectionInfoWidget>>&& gasket,
    QObject* parent) :
    QObject(parent),
    mGasket(std::move(gasket)) {}

void ConnectionInfoGasket::onActionViewAt(int pos) {
  mGasket->viewAt(pos);
}

void ConnectionInfoGasket::onActionRemoveAt(int pos) {
  mGasket->removeAt(pos);
}

void ConnectionInfoGasket::onActionUpdateAt(int) {
  mGasket->update();
}

void ConnectionInfoGasket::onActionAdd() {
  mGasket->add();
}
