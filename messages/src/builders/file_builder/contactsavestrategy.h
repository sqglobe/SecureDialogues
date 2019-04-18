#ifndef CONTACTSAVESTRATEGY_H
#define CONTACTSAVESTRATEGY_H

#include <vector>
#include "containers/contactcontainer.h"

class ContactSaveStrategy {
 public:
  using container = ContactContainer;

 public:
  static std::vector<typename container::const_element> getContainerData(
      const container& container);
  static std::string to_str(const typename container::const_element& obj);
  static typename container::element from_str(const std::string& row);
  static void add(container& container, const typename container::element& obj);
};

#endif  // CONTACTSAVESTRATEGY_H
