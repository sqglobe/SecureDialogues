#ifndef ASYMETRICALKEYSAVER_H
#define ASYMETRICALKEYSERIALIZER_H
#include "utils/interfaces/objectdeserializer.h"
#include "utils/interfaces/objectserializer.h"
class AsymetricalKeyStore;

class AsymetricalKeySerializer : public ObjectDeserializer<AsymetricalKeyStore>,
                                 public ObjectSerializer<AsymetricalKeyStore> {
 public:
  virtual ~AsymetricalKeySerializer() = default;
};

#endif  // ASYMETRICALKEYSAVER_H
