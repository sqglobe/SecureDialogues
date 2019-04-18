#include "contactcontainer.h"
#include <algorithm>
#include "primitives/contact.h"

std::string get_id(const ContactContainer::const_element& elem) {
  return elem->id();
}

std::string get_id(const ContactContainer::element& elem) {
  return elem->id();
}

ContactContainer::const_element ContactContainer::getContactByAdress(
    const std::string& adress) const noexcept(false) {
  return find([&adress](const ContactContainer::element& elem) {
    return elem->adress() == adress;
  });
}
