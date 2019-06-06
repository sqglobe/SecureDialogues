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

CryptoSystemImpl::CryptoSystemImpl(
    std::shared_ptr<AsymetricalKeyStore> keys,
    std::shared_ptr<AbstractMessageMarshaller> marshaller) :
    mAsymKeys(std::move(keys)),
    mMarshaller(std::move(marshaller)),
    mLocal(std::make_shared<LocalPeerOperations>(mAsymKeys->getPrivateKey())) {}

bool CryptoSystemImpl::isSignatureOk(const DialogMessage& message) const
    noexcept(false) {
  DialogMessage mess(message.action(), message.content(), message.dialogId(),
                     message.adress(), message.sequential(), "",
                     message.timestamp());
  auto strRepr = mMarshaller->marshall(mess);

  auto elem = getRemoteByAdress(message.adress());
  return elem->isValid(strRepr, message.sign());
}

std::string CryptoSystemImpl::createSignature(
    const DialogMessage& message) const {
  auto strRepr = mMarshaller->marshall(message);

  return mLocal->createSign(strRepr);
}

std::string CryptoSystemImpl::generateAndExportKey(
    const std::string& dialogId,
    const std::string& addres) noexcept(false) {
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
    const std::string& dialogId,
    const std::string& message) noexcept(false) {
  std::string line;
  std::stringstream ss(message);
  std::getline(ss, line, ',');

  {
    std::unique_lock<std::shared_mutex> guard(mMutex);
    mCiphers.emplace(
        dialogId, DialogElement{createSymmetricalCipherFrom(line, mLocal), ""});
    std::getline(ss, line, ',');
    auto decr = mLocal->decrypt(line);
    return mCiphers.at(dialogId).cipher->encrypt(decr);
  }
}

bool CryptoSystemImpl::checkVerificationString(const std::string& dialogId,
                                               const std::string& message) {
  std::shared_lock<std::shared_mutex> guard(mMutex);
  auto str = mCiphers.at(dialogId).myString;
  auto decr = mCiphers.at(dialogId).cipher->decrypt(message);

  return decr == str;
}

std::string CryptoSystemImpl::encryptMessageBody(
    const std::string& dialogId,
    const std::string& message) const noexcept(false) {
  std::shared_lock<std::shared_mutex> guard(mMutex);

  return mCiphers.at(dialogId).cipher->encrypt(message);
}

std::string CryptoSystemImpl::decryptMessageBody(
    const std::string& dialogId,
    const std::string& message) const noexcept(false) {
  std::shared_lock<std::shared_mutex> guard(mMutex);

  return mCiphers.at(dialogId).cipher->decrypt(message);
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

void CryptoSystemImpl::updateContact(
    const std::shared_ptr<const Contact>& contact) noexcept(false) {
  std::unique_lock<std::shared_mutex> guard(mMutex);

  CryptoPP::RSA::PublicKey pub;
  deserialize(contact->publicKey(), pub);
  mRemote[contact->id()] = {contact->adress(),
                            std::make_shared<RemotePeerOperations>(pub)};
}

void CryptoSystemImpl::removeContact(
    const std::shared_ptr<const Contact>& contact) noexcept(false) {
  std::unique_lock<std::shared_mutex> guard(mMutex);
  mRemote.erase(contact->id());
}

void CryptoSystemImpl::dialogRemoved(const std::string& dialogId) noexcept(
    false) {
  std::unique_lock<std::shared_mutex> guard(mMutex);

  mCiphers.erase(dialogId);
}

std::shared_ptr<RemotePeerOperations> CryptoSystemImpl::getRemoteByAdress(
    const std::string& adress) const noexcept(false) {
  std::shared_lock<std::shared_mutex> guard(mMutex);
  auto it = std::find_if(
      mRemote.cbegin(), mRemote.cend(),
      [&adress](const std::pair<std::string, RemoteElement>& elem) {
        return elem.second.addres == adress;
      });

  if (mRemote.cend() != it) {
    return it->second.mOperation;
  }

  throw std::range_error("not found contact with adress " + adress);
}
