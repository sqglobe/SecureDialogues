#ifndef ASYMETRICALKEYSERIALIZER_H
#define ASYMETRICALKEYSERIALIZER_H
#include "utils/interfaces/objectdeserializer.h"
#include "utils/interfaces/objectserializer.h"
class AsymetricalKeyStore;

class AsymetricalKeySerializer : public ObjectDeserializer<AsymetricalKeyStore>,
                                 public ObjectSerializer<AsymetricalKeyStore> {
};

#endif  // ASYMETRICALKEYSERIALIZER_H
