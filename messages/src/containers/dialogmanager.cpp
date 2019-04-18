#include "dialogmanager.h"

std::string get_id(const DialogManager::const_element& elem) {
  return elem->getDialogId();
}
std::string get_id(const DialogManager::element& elem) {
  return elem->getDialogId();
}

std::string DialogManager::make(const std::shared_ptr<const Contact>& contact) {
  auto dialog = std::make_shared<Dialog>(contact);
  add(dialog);
  return dialog->getDialogId();
}
