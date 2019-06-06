#ifndef AESKEYGENERATOR_H
#define AESKEYGENERATOR_H
#include "aeskeystruct.h"
#include "interfaces/generator.h"

/**
 * @brief Генератор ключа для алгоритма AES
 */
class AesKeyGenerator : public Generator<AesKeyStruct> {
 public:
  AesKeyGenerator::Value generate() noexcept(false) override;
};

#endif  // AESKEYGENERATOR_H
