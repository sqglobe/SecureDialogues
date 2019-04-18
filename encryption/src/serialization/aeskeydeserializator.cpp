#include "aeskeydeserializator.h"

#include <iomanip>
#include <sstream>
#include <string>

AesKeyDeserializator::AesKeyDeserializator(
    const std::shared_ptr<const Decryptor>& decryptor,
    const std::string& message) :
    mKeyDecryptor(decryptor),
    mMessage(message) {}

CryptoPP::SecByteBlock readBlock(std::stringstream& ss) {
  std::string size, field;
  std::getline(ss, size, ':');
  auto isize = std::stoi(size);
  if (isize <= 0) {
    throw BadFormatException("message have a bad format");
  }

  field.resize(isize);
  ss.read(&field[0], field.size());

  return CryptoPP::SecByteBlock(
      reinterpret_cast<const unsigned char*>(&field[0]), field.size());
}

void AesKeyDeserializator::deserialize(AesKeyStruct& object) noexcept(false) {
  std::stringstream ss(mKeyDecryptor->decrypt(mMessage));
  object.key = readBlock(ss);
  object.iv = readBlock(ss);
}
