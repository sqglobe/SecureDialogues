#include "consistentwatchers.h"
#include "containers/contactcontainer.h"
#include "containers/dialogmanager.h"
#include "containers/messagecontainer.h"
#include "primitives/dialog.h"

ContactConsistentWatcher::ContactConsistentWatcher(
    const std::shared_ptr<ContactContainer>& consistentConteiner) :
    mConsistentConteiner(consistentConteiner) {}

void ContactConsistentWatcher::removed(const element& obj) {
  auto removed = mConsistentConteiner->get_if(
      [&obj](const std::shared_ptr<const Contact>& cont) -> bool {
        return cont->channelMoniker() == obj.getConnectionName();
      });

  for (const auto& element : removed) {
    mConsistentConteiner->remove(element->id());
  }
}

DialogConsistentWatcher::DialogConsistentWatcher(
    const std::shared_ptr<DialogManager>& consistentConteiner) :
    mConsistentConteiner(consistentConteiner) {}

void DialogConsistentWatcher::removed(const ChangeWatcher::element& obj) {
  auto removed = mConsistentConteiner->get_if(
      [&obj](const std::shared_ptr<const Dialog>& dial) -> bool {
        return dial->getContactId() == obj->id();
      });

  for (const auto& element : removed) {
    mConsistentConteiner->remove(element->getDialogId());
  }
}

MessagesConsistentWatcher::MessagesConsistentWatcher(
    const std::shared_ptr<MessageContainer>& consistentConteiner) :
    mConsistentConteiner(consistentConteiner) {}

void MessagesConsistentWatcher::removed(const ChangeWatcher::element& obj) {
  mConsistentConteiner->removeDialog(obj->getDialogId());
}
