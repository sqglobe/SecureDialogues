#include "aeskeygenerator.h"

#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>

AesKeyGenerator::Value AesKeyGenerator::generate() noexcept(false) {
  using namespace CryptoPP;
  AutoSeededRandomPool rnd;
  SecByteBlock key(nullptr, AES::MAX_KEYLENGTH);
  rnd.GenerateBlock(key, key.size());

  // Generate a random IV
  SecByteBlock iv(AES::BLOCKSIZE);
  rnd.GenerateBlock(iv, iv.size());

  return AesKeyGenerator::Value{key, iv};
}
