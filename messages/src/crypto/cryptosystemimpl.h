#ifndef CRYPTOSYSTEMIMPL_H
#define CRYPTOSYSTEMIMPL_H

#include <map>
#include <memory>
#include <shared_mutex>
#include <string>
#include "interfaces/cryptosystem.h"

class DialogMessage;
class RemotePeerOperations;
class LocalPeerOperations;
class Contact;
class SymetricalCipher;
class AsymetricalKeyStore;
class AbstractMessageMarshaller;

/**
 * @brief Класс, который покрывает потребности системы в криптографии
 */
class CryptoSystemImpl : public CryptoSystem {
 private:
  struct RemoteElement {
    std::string addres;
    std::shared_ptr<RemotePeerOperations> mOperation;
  };

  struct DialogElement {
    std::unique_ptr<SymetricalCipher> cipher;
    std::string myString;
  };

 public:
  CryptoSystemImpl(
      const std::shared_ptr<AsymetricalKeyStore>& keys,
      const std::shared_ptr<AbstractMessageMarshaller>& marshaller);

 public:
  bool isSignatureOk(const DialogMessage& message) const
      noexcept(false) override;
  std::string createSignature(const DialogMessage& message) const
      noexcept(false) override;

  std::string encryptMessageBody(const std::string& dialogId,
                                 const std::string& message) const
      noexcept(false) override;
  std::string decryptMessageBody(const std::string& dialogId,
                                 const std::string& message) const
      noexcept(false) override;

 public:
  std::string generateAndExportKey(
      const std::string& dialogId,
      const std::string& addres) noexcept(false) override;
  std::string importKey(const std::string& dialogId,
                        const std::string& message) noexcept(false) override;
  bool checkVerificationString(
      const std::string& dialogId,
      const std::string& message) noexcept(false) override;

  /**
   * @brief Экспортирует текущее значение публичного ключа пользователя
   * @return строковое представление публичного ключа пользователя
   */
  std::string exportPublicKey() const noexcept(false);

  /**
   * @brief Выполняет генерацию открытого и закрытого ключей пользователя
   * @return true в случае успеха, false во всех остальных
   */
  bool generateAsymetricKeys() noexcept(false);

 public:
  /**
   * @brief Обновляет информацию об удаленном собеседнике.
   * Используется для обновления публичного ключа указанного собеседника
   * Используется так же для добавления публичного ключа, если данных о
   * собеседнике нет. Вызывается при добавлении/изменении контакта в контейнере.
   * @param contact собеседник, для которого произошло обновление
   */
  void updateContact(const std::shared_ptr<const Contact>& contact) noexcept(
      false);

  /**
   * @brief Удалает данные (публичный ключ), которые ассоциированы с указанным
   * контактом Вызывается, когда диалог удаляется из контейнера
   * @param contact контакт, для которого необходимо удалить данные
   */
  void removeContact(const std::shared_ptr<const Contact>& contact) noexcept(
      false);

  /**
   * @brief Используется для удалении информации (сеансового ключа) для
   * указанного диалога. Вызывается, когда диалог удаляется из контейнера
   * @param dialogId диалог, для которого необходимо произвести очистку.
   */
  void dialogRemoved(const std::string& dialogId) noexcept(false);

 private:
  std::shared_ptr<RemotePeerOperations> getRemoteByAdress(
      const std::string& adress) const noexcept(false);

 private:
  std::shared_ptr<AsymetricalKeyStore> mAsymKeys;
  std::shared_ptr<AbstractMessageMarshaller> mMarshaller;
  std::shared_ptr<LocalPeerOperations> mLocal;

  std::map<std::string, RemoteElement> mRemote;
  std::map<std::string, DialogElement> mCiphers;
  mutable std::shared_mutex mMutex;
};

#endif  // CRYPTOSYSTEMIMPL_H
