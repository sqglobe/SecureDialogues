#include "abstractchanneladapter.h"
#include "primitives/connectionholder.h"

AbstractChannelAdapter::AbstractChannelAdapter(
    std::shared_ptr<AbstractUserNotifier> notifier,
    ConnectionHolder conn) :
    mNotifier(std::move(notifier)),
    mConn(std::move(conn)) {}

void AbstractChannelAdapter::notify(AbstractUserNotifier::Severity sev,
                                    const std::string& message) {
  mNotifier->notify(sev, message);
}

ConnectionHolder AbstractChannelAdapter::getHolder() const {
  return mConn;
}
