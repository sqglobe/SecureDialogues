#ifndef AESKEYSTRUCT_H
#define AESKEYSTRUCT_H

#include <cryptopp/secblock.h>

struct AesKeyStruct {
  CryptoPP::SecByteBlock key;
  CryptoPP::SecByteBlock iv;
};

#endif  // AESKEYSTRUCT_H
