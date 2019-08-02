#ifndef CONNECTIONMARSHALLER_H
#define CONNECTIONMARSHALLER_H

#include "primitives/connectionholder.h"

class ConnectionMarshaller {
 public:
  static void restore(ConnectionHolder& elem, const void* src);
  static size_t size(const ConnectionHolder& element);
  static void store(void* dest, const ConnectionHolder& elem);
};

#endif  // CONNECTIONMARSHALLER_H
