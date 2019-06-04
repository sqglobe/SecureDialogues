#include "abstractchanneladapter.h"
#include "primitives/connectionholder.h"

AbstractChannelAdapter::AbstractChannelAdapter(
    const std::shared_ptr<AbstractUserNotifier>& notifier,
    const ConnectionHolder& conn) :
    mNotifier(notifier),
    mConn(conn) {}

void AbstractChannelAdapter::notify(AbstractUserNotifier::Severity sev,
                                    const std::string& message) {
  mNotifier->notify(sev, message);
}

ConnectionHolder AbstractChannelAdapter::getHolder() const {
  return mConn;
}
