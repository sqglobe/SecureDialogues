#ifndef CONTACTMARSHALLER_H
#define CONTACTMARSHALLER_H

#include <memory>
#include "primitives/contact.h"

class ContactMarshaller {
 public:
  static void restore(std::shared_ptr<Contact>& elem, const void* src);
  static size_t size(const std::shared_ptr<Contact>& element);
  static void store(void* dest, const std::shared_ptr<Contact>& elem);
};

#endif  // CONTACTMARSHALLER_H
