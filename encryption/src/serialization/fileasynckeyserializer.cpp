#include "fileasynckeyserializer.h"

#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>

#include <fstream>
#include <sstream>
#include "asymetrical/asymetricalkeystore.h"
#include "interfaces/symetricalcipher.h"

template <typename Key>
struct KeyTraits;

template <>
struct KeyTraits<CryptoPP::RSA::PublicKey> {
  static const std::string FILE_EXTENSION;
  static void decode(CryptoPP::RSA::PublicKey& key,
                     CryptoPP::BufferedTransformation& bt) {
    key.BERDecodePublicKey(bt, false /*optParams*/, bt.MaxRetrievable());
  }
  static void encode(const CryptoPP::RSA::PublicKey& key,
                     CryptoPP::BufferedTransformation& bt) {
    key.DEREncodePublicKey(bt);
  }
};

const std::string KeyTraits<CryptoPP::RSA::PublicKey>::FILE_EXTENSION = "PUB";

template <>
struct KeyTraits<CryptoPP::RSA::PrivateKey> {
  static const std::string FILE_EXTENSION;
  static void decode(CryptoPP::RSA::PrivateKey& key,
                     CryptoPP::BufferedTransformation& bt) {
    key.BERDecodePrivateKey(bt, false /*optParams*/, bt.MaxRetrievable());
  }
  static void encode(const CryptoPP::RSA::PrivateKey& key,
                     CryptoPP::BufferedTransformation& bt) {
    key.DEREncodePrivateKey(bt);
  }
};

const std::string KeyTraits<CryptoPP::RSA::PrivateKey>::FILE_EXTENSION = "PRV";

template <typename Key>
class KeyLoader {
 public:
  void saveKey(const std::string& fileTemplate,
               const Key& key,
               const std::shared_ptr<const SymetricalCipher>& cipher) const
      noexcept(false);
  Key loadKey(const std::string& fileTemplate,
              const std::shared_ptr<const SymetricalCipher>& cipher) const
      noexcept(false);
};

template <typename Key>
void KeyLoader<Key>::saveKey(
    const std::string& fileTemplate,
    const Key& key,
    const std::shared_ptr<const SymetricalCipher>& cipher) const
    noexcept(false) {
  auto filename = fileTemplate + "." + KeyTraits<Key>::FILE_EXTENSION;
  std::stringstream ss;

  CryptoPP::ByteQueue queue;
  KeyTraits<Key>::encode(key, queue);

  CryptoPP::FileSink file(ss);
  queue.CopyTo(file);
  auto encrypted = cipher->encrypt(ss.str());
  std::fstream outFile(filename, std::ios_base::out);
  outFile << encrypted << std::flush;
}

template <typename Key>
Key KeyLoader<Key>::loadKey(
    const std::string& fileTemplate,
    const std::shared_ptr<const SymetricalCipher>& cipher) const {
  Key key;
  std::string filename = fileTemplate + "." + KeyTraits<Key>::FILE_EXTENSION;

  std::fstream outFile(filename, std::ios_base::in);

  std::string str((std::istreambuf_iterator<char>(outFile)),
                  std::istreambuf_iterator<char>());
  std::stringstream ss;

  ss << cipher->decrypt(str);

  CryptoPP::ByteQueue queue;
  CryptoPP::FileSource file(ss, true /*pumpAll*/);
  file.TransferTo(queue);
  queue.MessageEnd();

  KeyTraits<Key>::decode(key, queue);
  return key;
}

FileAsyncKeySerializer::FileAsyncKeySerializer(
    const std::string& fileTemplate,
    const std::shared_ptr<const SymetricalCipher>& cipher) :
    mFileTemplate(fileTemplate),
    mCipher(cipher) {}

void FileAsyncKeySerializer::serialize(
    const AsymetricalKeyStore& store) noexcept(false) {
  auto pub = store.getPublicKey();
  auto pubKeyLoader = KeyLoader<CryptoPP::RSA::PublicKey>();
  pubKeyLoader.saveKey(mFileTemplate, pub, mCipher);

  auto priv = store.getPrivateKey();
  auto privKeyLoader = KeyLoader<CryptoPP::RSA::PrivateKey>();

  privKeyLoader.saveKey(mFileTemplate, priv, mCipher);
}

void FileAsyncKeySerializer::deserialize(AsymetricalKeyStore& store) noexcept(
    false) {
  auto privKeyLoader = KeyLoader<CryptoPP::RSA::PrivateKey>();
  auto pubKeyLoader = KeyLoader<CryptoPP::RSA::PublicKey>();

  auto priv = privKeyLoader.loadKey(mFileTemplate, mCipher);
  auto pub = pubKeyLoader.loadKey(mFileTemplate, mCipher);

  store.setKeys(pub, priv);
}
