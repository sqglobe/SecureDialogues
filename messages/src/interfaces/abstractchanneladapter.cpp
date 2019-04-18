#include "abstractchanneladapter.h"
#include "primitives/connectionholder.h"

AbstractChannelAdapter::AbstractChannelAdapter(
    const std::shared_ptr<AbstractUserNotifier>& notifier) :
    mNotifier(notifier) {}

void AbstractChannelAdapter::notify(AbstractUserNotifier::Severity sev,
                                    const std::string& message) {
  mNotifier->notify(sev, message);
}
