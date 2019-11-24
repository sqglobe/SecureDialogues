#include "aescipher.h"
#include <cryptopp/aes.h>
#include <cryptopp/base64.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>

AesCipher::AesCipher(const CryptoPP::SecByteBlock& key,
                     const CryptoPP::SecByteBlock& iv) :
    mKey(key),
    mIv(iv) {}

std::string AesCipher::encrypt(const std::string& source) const {
  using namespace CryptoPP;

  CFB_Mode<AES>::Encryption e(mKey, mKey.size(), mIv);
  std::string cipher;
  StringSource(source, true,
               new CryptoPP::StreamTransformationFilter(
                   e,
                   new Base64Encoder(
                       new StringSink(cipher)))  // StreamTransformationFilter
  );                                             // StringSource

  return cipher;
}

std::string AesCipher::encrypt(std::string_view source) const {
  using namespace CryptoPP;

  CFB_Mode<AES>::Encryption e(mKey, mKey.size(), mIv);
  std::string cipher;
  StringSource(reinterpret_cast<const byte*>(source.data()), source.size(),
               true,
               new CryptoPP::StreamTransformationFilter(
                   e,
                   new Base64Encoder(
                       new StringSink(cipher)))  // StreamTransformationFilter
  );                                             // StringSource

  return cipher;
}

std::string AesCipher::decrypt(const std::string& data) const {
  using namespace CryptoPP;

  CFB_Mode<AES>::Decryption d(mKey, mKey.size(), mIv);
  std::string res;

  // The StreamTransformationFilter removes
  //  padding as required.
  StringSource s(data, true,
                 new Base64Decoder(new CryptoPP::StreamTransformationFilter(
                     d,
                     new StringSink(res))  // StreamTransformationFilter
                                   ));     // StringSource
  return res;
}

std::string AesCipher::decrypt(std::string_view data) const {
  using namespace CryptoPP;

  CFB_Mode<AES>::Decryption d(mKey, mKey.size(), mIv);
  std::string res;

  // The StreamTransformationFilter removes
  //  padding as required.
  StringSource s(reinterpret_cast<const byte*>(data.data()), data.size(), true,
                 new Base64Decoder(new CryptoPP::StreamTransformationFilter(
                     d,
                     new StringSink(res))  // StreamTransformationFilter
                                   ));     // StringSource
  return res;
}
