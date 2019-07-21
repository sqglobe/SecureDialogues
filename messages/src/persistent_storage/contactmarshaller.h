#ifndef CONTACTMARSHALLER_H
#define CONTACTMARSHALLER_H

#include <memory>
#include "primitives/contact.h"

class ContactMarshaller {
 public:
  static void restore(Contact& elem, const void* src);
  static size_t size(const Contact& element);
  static void store(void* dest, const Contact& elem);
};

#endif  // CONTACTMARSHALLER_H
