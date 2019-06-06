#ifndef FILEASYNCKEYSERIALIZER_H
#define FILEASYNCKEYSERIALIZER_H

#include <memory>
#include <string>
#include "interfaces/asymetricalkeyserializer.h"

class SymetricalCipher;

/**
 * @brief Класс, в котором объединены операции воссоздания ключей для
 * ассиметричного шифрования из файла и для записи их в файл. Перед записью,
 * ключи шифруются с помощью симметричного шифратора.
 */
class FileAsyncKeySerializer : public AsymetricalKeySerializer {
 public:
  /**
   * @brief Конструктор класса
   * @param fileTemplate - шаблон имени файлов, может содержать пути. При
   * сохранении ключа, к этому имени добавляется префикс (вроде префикса PUB для
   * открытого ключа)
   * @param cipher - шифратор, с помощью которого шифруются ключи при
   * сохранении, и дешифруются при считывании из файлов.
   */
  FileAsyncKeySerializer(const std::string& fileTemplate,
                         const std::shared_ptr<const SymetricalCipher>& cipher);

  void serialize(const AsymetricalKeyStore& store) noexcept(false) override;
  void deserialize(AsymetricalKeyStore& store) noexcept(false) override;

 private:
  std::string mFileTemplate;
  std::shared_ptr<const SymetricalCipher> mCipher;
};

#endif  // FILEASYNCKEYSERIALIZER_H
