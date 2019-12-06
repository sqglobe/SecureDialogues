#ifndef DISCOVEREDCONTACTMARSHALLER_H
#define DISCOVEREDCONTACTMARSHALLER_H

#include <sys/types.h>

class DiscoveredContact;

class DiscoveredContactMarshaller {
 public:
  static void restore(DiscoveredContact& elem, const void* src);
  static u_int32_t size(const DiscoveredContact& element);
  static void store(void* dest, const DiscoveredContact& elem);
};

#endif  // DISCOVEREDCONTACTMARSHALLER_H
