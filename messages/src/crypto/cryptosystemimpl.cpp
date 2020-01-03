#include "cryptosystemimpl.h"

#include "asymetrical/localpeeroperations.h"
#include "asymetrical/remotepeeroperations.h"
#include "interfaces/abstractmessagemarshaller.h"
#include "interfaces/symetricalcipher.h"
#include "primitives/contact.h"
#include "primitives/dialogmessage.h"

#include "interfaces/symetricalcipher.h"
#include "symetrical/symetricalsystemfactories.h"

#include "asymetrical/asymetricalkeygenerator.h"
#include "asymetrical/asymetricalkeystore.h"
#include "serialization/serializations.h"

#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>

#include <sstream>

#include <libintl.h>
#include "fmt/core.h"

CryptoSystemImpl::CryptoSystemImpl(
    std::shared_ptr<AsymetricalKeyStore> keys,
    std::shared_ptr<AbstractMessageMarshaller> marshaller) :
    mAsymKeys(std::move(keys)),
    mMarshaller(std::move(marshaller)),
    mLocal(std::make_shared<LocalPeerOperations>(mAsymKeys->getPrivateKey())) {}

bool CryptoSystemImpl::isSignatureOk(const DialogMessage& message) const
    noexcept(false) {
  auto strRepr = mMarshaller->marshallExceptSignature(message);

  auto elem = getRemoteByAdress(message.adress());
  return elem->isValid(strRepr, message.sign());
}

std::string CryptoSystemImpl::createSignature(
    const DialogMessage& message) const {
  auto strRepr = mMarshaller->marshall(message);

  return mLocal->createSign(strRepr);
}

std::string CryptoSystemImpl::generateAndExportKey(
    std::string_view dialogId,
    std::string_view addres) noexcept(false) {
  std::string messBody;

  CryptoPP::SecByteBlock genBlock(100);
  std::string resStr;

  CryptoPP::OS_GenerateRandomBlock(true, genBlock, genBlock.size());

  CryptoPP::HexEncoder hex(new CryptoPP::StringSink(resStr));
  hex.Put(genBlock, genBlock.size());
  hex.MessageEnd();

  auto elem = getRemoteByAdress(addres);

  {
    std::unique_lock<std::shared_mutex> guard(mMutex);

    mCiphers.emplace(
        dialogId,
        DialogElement{generateSymmetricalCipherTo(messBody, elem), resStr});
  }
  auto encrCheck = elem->encrypt(resStr);

  return messBody + "," + encrCheck;
}

std::string CryptoSystemImpl::importKey(
    std::string_view dialogId,
    std::string_view message) noexcept(false) {
  auto pos = message.find(',');
  {
    std::string_view key(message.substr(0, pos)),
        verification(message.substr(pos + 1));

    std::unique_lock<std::shared_mutex> guard(mMutex);
    mCiphers.emplace(
        dialogId, DialogElement{createSymmetricalCipherFrom(key, mLocal), ""});
    auto decr = mLocal->decrypt(verification);
    if (auto iter = mCiphers.find(dialogId); iter != mCiphers.end()) {
      return iter->second.cipher->encrypt(decr);
    } else {
      throw std::out_of_range(
          "out of range for the CryptoSystemImpl::importKey");
    }
  }
}

bool CryptoSystemImpl::checkVerificationString(std::string_view dialogId,
                                               std::string_view message) {
  std::shared_lock<std::shared_mutex> guard(mMutex);
  if (auto iter = mCiphers.find(dialogId); iter != mCiphers.end()) {
    auto str = iter->second.myString;
    auto decr = iter->second.cipher->decrypt(message);

    return decr == str;
  }
  return false;
}

std::string CryptoSystemImpl::encryptMessageBody(std::string_view dialogId,
                                                 std::string_view message) const
    noexcept(false) {
  std::shared_lock<std::shared_mutex> guard(mMutex);
  if (auto iter = mCiphers.find(dialogId); iter != mCiphers.end()) {
    return iter->second.cipher->encrypt(message);
  }
  throw std::runtime_error(
      fmt::format(dgettext("messages", "Cant find dialog {}"), dialogId));
}

std::string CryptoSystemImpl::decryptMessageBody(std::string_view dialogId,
                                                 std::string_view message) const
    noexcept(false) {
  std::shared_lock<std::shared_mutex> guard(mMutex);
  if (auto iter = mCiphers.find(dialogId); iter != mCiphers.end()) {
    return iter->second.cipher->decrypt(message);
  }
  throw std::runtime_error(
      fmt::format(dgettext("messages", "Cant find dialog {}"), dialogId));
}

std::string CryptoSystemImpl::exportPublicKey() const noexcept(false) {
  return serialize(mAsymKeys->getPublicKey());
}

bool CryptoSystemImpl::generateAsymetricKeys() noexcept(false) {
  if (AsymetricalKeyGenerator(mAsymKeys).generate()) {
    mLocal->updateKey(mAsymKeys->getPrivateKey());

    return true;
  }
  return false;
}

void CryptoSystemImpl::updateContact(const Contact& contact) noexcept(false) {
  std::unique_lock<std::shared_mutex> guard(mMutex);

  CryptoPP::RSA::PublicKey pub;
  deserialize(contact.publicKey(), pub);
  mRemote[std::string(contact.id())] = {
      std::string(contact.adress()),
      std::make_shared<RemotePeerOperations>(pub)};
}

void CryptoSystemImpl::removeContact(const Contact& contact) noexcept(false) {
  std::unique_lock<std::shared_mutex> guard(mMutex);
  mRemote.erase(std::string(contact.id()));
}

void CryptoSystemImpl::dialogRemoved(std::string_view dialogId) noexcept(
    false) {
  std::unique_lock<std::shared_mutex> guard(mMutex);

  mCiphers.erase(std::string(dialogId));
}

std::shared_ptr<RemotePeerOperations> CryptoSystemImpl::getRemoteByAdress(
    std::string_view adress) const noexcept(false) {
  std::shared_lock<std::shared_mutex> guard(mMutex);
  auto it = std::find_if(
      mRemote.cbegin(), mRemote.cend(),
      [&adress](const std::pair<std::string, RemoteElement>& elem) {
        return elem.second.addres == adress;
      });

  if (mRemote.cend() != it) {
    return it->second.mOperation;
  }

  throw std::range_error(fmt::format(
      dgettext("messages", "not found contact with adress {}"), adress));
}
