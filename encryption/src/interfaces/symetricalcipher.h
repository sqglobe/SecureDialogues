#ifndef SYMETRICALCIPHER_H
#define SYMETRICALCIPHER_H
#include "decryptor.h"
#include "encryptor.h"

/**
 * @brief Интерфейс для класса, который выполняет шифрование и дешифрование по
 * ассиметричному алгоритму
 */
class SymetricalCipher : public Encryptor, public Decryptor {};

#endif  // SYMETRICALCIPHER_H
