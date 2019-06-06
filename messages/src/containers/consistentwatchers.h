#ifndef CONSISTENTWATCHERS_H
#define CONSISTENTWATCHERS_H

#include "interfaces/changewatcher.h"
#include "primitives/connectionholder.h"

#include <memory>

class ContactContainer;
class DialogManager;
class Dialog;
class Contact;
class MessageContainer;

class ContactConsistentWatcher : public ChangeWatcher<ConnectionHolder> {
 public:
  ContactConsistentWatcher(
      std::shared_ptr<ContactContainer> consistentConteiner);

 public:
  void added(const element&) override {}
  void changed(const element&) override {}
  void removed(const element& obj) override;

 private:
  std::shared_ptr<ContactContainer> mConsistentConteiner;
};

class DialogConsistentWatcher
    : public ChangeWatcher<std::shared_ptr<const Contact>> {
 public:
  DialogConsistentWatcher(std::shared_ptr<DialogManager> consistentConteiner);

 public:
  void added(const element&) override {}
  void changed(const element&) override {}
  void removed(const element& obj) override;

 private:
  std::shared_ptr<DialogManager> mConsistentConteiner;
};

class MessagesConsistentWatcher
    : public ChangeWatcher<std::shared_ptr<const Dialog>> {
 public:
  MessagesConsistentWatcher(
      std::shared_ptr<MessageContainer> consistentConteiner);

 public:
  void added(const element&) override {}
  void changed(const element&) override {}
  void removed(const element& obj) override;

 private:
  std::shared_ptr<MessageContainer> mConsistentConteiner;
};

#endif  // CONSISTENTWATCHERS_H
