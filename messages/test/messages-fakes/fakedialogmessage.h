#ifndef FAKEDIALOGMESSAGE_H
#define FAKEDIALOGMESSAGE_H
#include "primitives/dialogmessage.h"
// add necessary includes here

class FakeDialogMessage : public DialogMessage {
 public:
  static bool splitKeyValTest(const std::string& source,
                              std::string& key,
                              std::string& val,
                              char ch = ':') {
    return splitKeyVal(source, key, val, ch);
  }
};
#endif  // FAKEDIALOGMESSAGE_H
