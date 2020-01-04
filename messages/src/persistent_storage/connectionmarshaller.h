#ifndef CONNECTIONMARSHALLER_H
#define CONNECTIONMARSHALLER_H

#include "primitives/connectionholder.h"

class ConnectionMarshaller {
 public:
  static void restore(ConnectionHolder& elem, const void* src);
  static uint32_t size(const ConnectionHolder& element);
  static void store(void* dest, const ConnectionHolder& elem);
};

#endif  // CONNECTIONMARSHALLER_H
