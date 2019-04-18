#ifndef SYMETRICALSYSTEMFACTORIES_H
#define SYMETRICALSYSTEMFACTORIES_H

#include <memory>
#include <string>

#include "aeskeystruct.h"

class Encryptor;
class Decryptor;
class SymetricalCipher;

/**
 * @brief Создает симметричный шифратор/дешифратор на основе зашифрованного
 * ключа
 * @param message зашифрованный ключ для шифратора/дешифратора
 * @param decryptor объект, с помощью которого можно дешифровать ключ из
 * сообщения message
 * @return шифратор/дешифратор для симметричного алгоритма
 */
std::unique_ptr<SymetricalCipher> createSymmetricalCipherFrom(
    const std::string& message,
    const std::shared_ptr<const Decryptor>& decryptor);

/**
 * @brief Генерирует случайный ключ для симметричного шифратора/дешифратора,
 * шифрует его и записывает в строку, возвращает созданный шифратор/дешифратор.
 * @param message сообщение, в которое будет записан зашифрованный ключ
 * @param encryptor объект, с помощью которого будет зашифрован ключ
 * @return система шифрования по симметричному алгоритму
 */
std::unique_ptr<SymetricalCipher> generateSymmetricalCipherTo(
    std::string& message,
    const std::shared_ptr<const Encryptor>& encryptor);

/**
 * @brief Из тектового пароля создает ключ для системы симметричного шифрования
 * и инициализирует им такую систему
 * @param pass текстовый пароль, из которого будет создан ключ
 * @return симметричная система криптографии
 */
std::unique_ptr<SymetricalCipher> makeForStringPass(const std::string& pass);

#endif  // SYMETRICALSYSTEMFACTORIES_H
