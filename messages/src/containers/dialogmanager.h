#ifndef DIALOGMANAGER_H
#define DIALOGMANAGER_H

#include <memory>
#include "basecontainer.h"
#include "interfaces/changewatcher.h"
#include "primitives/contact.h"
#include "primitives/dialog.h"

/**
 * @brief Контейнер для диалогов
 */
class DialogManager : public BaseContainer<std::shared_ptr<Dialog>,
                                           std::shared_ptr<const Dialog>> {
 public:
  std::string make(const std::shared_ptr<const Contact>& contact);
};

std::string get_id(const DialogManager::const_element& elem);
std::string get_id(const DialogManager::element& elem);

#endif  // DIALOGMANAGER_H
