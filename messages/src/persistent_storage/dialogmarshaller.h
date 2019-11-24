#ifndef DIALOGMARSHALLER_H
#define DIALOGMARSHALLER_H

#include "primitives/dialog.h"

class DialogMarshaller {
 public:
  static void restore(Dialog& elem, const void* src);
  static u_int32_t size(const Dialog& element);
  static void store(void* dest, const Dialog& elem);
};

#endif  // DIALOGMARSHALLER_H
