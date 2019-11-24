#include "connectioninfogasket.h"

ConnectionInfoGasket::ConnectionInfoGasket(
    std::unique_ptr<
        DialogWidgetGasket<ConnectionStorage, ConnectionInfoWidget>>&& gasket,
    QObject* parent) :
    QObject(parent),
    mGasket(std::move(gasket)) {}

void ConnectionInfoGasket::onActionViewAt(std::string id) {
  mGasket->viewAt(id);
}

void ConnectionInfoGasket::onActionRemoveAt(std::string id) {
  mGasket->removeAt(id);
}

void ConnectionInfoGasket::onActionUpdateAt(std::string) {
  mGasket->update();
}

void ConnectionInfoGasket::onActionAdd() {
  mGasket->add();
}
