#include "serializations.h"

#include "interfaces/decryptor.h"
#include "interfaces/encryptor.h"
#include "symetrical/aeskeystruct.h"

#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>

#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <iomanip>

CryptoPP::SecByteBlock readBlock(std::stringstream& ss) {
  std::string size, field;
  std::getline(ss, size, ':');
  auto isize = std::stoi(size);
  if (isize <= 0) {
    throw BadFormatException("message have a bad format");
  }

  field.resize(static_cast<std::size_t>(isize));
  ss.read(&field[0], static_cast<int>(field.size()));

  return CryptoPP::SecByteBlock(
      reinterpret_cast<const unsigned char*>(&field[0]), field.size());
}

void deserialize(const std::shared_ptr<const Decryptor>& decryptor,
                 const std::string& message,
                 AesKeyStruct& object) {
  std::stringstream ss(decryptor->decrypt(message));
  object.key = readBlock(ss);
  object.iv = readBlock(ss);
}

std::string serialize(const std::shared_ptr<const Encryptor>& encryptor,
                      const AesKeyStruct& object) {
  std::string key, iv;
  key.resize(object.key.size());
  std::memcpy(&key[0], &object.key[0], key.size());

  iv.resize(object.iv.size());
  std::memcpy(&iv[0], &object.iv[0], iv.size());

  std::stringstream ss;
  ss << key.size() << ":" << key << iv.size() << ":" << iv;
  auto tmp = ss.str();
  return encryptor->encrypt(ss.str());
}

void deserialize(const std::string& message, CryptoPP::RSA::PublicKey& object) {
  CryptoPP::ByteQueue bt;
  CryptoPP::StringSource ss(message, true /*pumpAll*/,
                            new CryptoPP::Base64Decoder());
  ss.TransferTo(bt);
  bt.MessageEnd();
  object.Load(bt);
  CryptoPP::AutoSeededRandomPool rng;
  if (!object.Validate(rng, 3)) {
    throw std::runtime_error("RSA key not valid!");
  }
}

std::string serialize(const CryptoPP::RSA::PublicKey& object) {
  std::string res;
  CryptoPP::Base64Encoder encoder(new CryptoPP::StringSink(res), true, 100);
  CryptoPP::ByteQueue queue;
  object.Save(queue);
  queue.CopyTo(encoder);
  encoder.MessageEnd();
  return res;
}
