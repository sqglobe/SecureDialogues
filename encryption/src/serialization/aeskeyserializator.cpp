#include "aeskeyserializator.h"
#include <sstream>
AesKeySerializator::AesKeySerializator(
    const std::shared_ptr<const Encryptor>& encryptor,
    std::string& ss) :
    mKeyEncryptor(encryptor),
    mOut(ss) {}

void AesKeySerializator::serialize(const AesKeyStruct& object) noexcept(false) {
  std::string key, iv;
  key.resize(object.key.size());
  std::memcpy(&key[0], &object.key[0], key.size());

  iv.resize(object.iv.size());
  std::memcpy(&iv[0], &object.iv[0], iv.size());

  std::stringstream ss;
  ss << key.size() << ":" << key << iv.size() << ":" << iv;
  auto tmp = ss.str();
  mOut = mKeyEncryptor->encrypt(ss.str());
}
