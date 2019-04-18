#ifndef CONTACTCONTAINER_H
#define CONTACTCONTAINER_H

#include "basecontainer.h"
#include "interfaces/changewatcher.h"
#include "primitives/contact.h"

/**
 * @brief Контейнер для контактов
 */
class ContactContainer : public BaseContainer<std::shared_ptr<Contact>,
                                              std::shared_ptr<const Contact>> {
 public:
  ContactContainer::const_element getContactByAdress(
      const std::string& adress) const noexcept(false);
};

std::string get_id(const ContactContainer::const_element& elem);
std::string get_id(const ContactContainer::element& elem);

#endif  // CONTACTCONTAINER_H
