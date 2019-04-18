#ifndef GENERATOR_H
#define GENERATOR_H

/**
 * Шаблонный интерфейс для генерации объекта класса T
 */
template <typename T>
class Generator {
 public:
  using Value = T;

 public:
  virtual ~Generator() = default;

  /**
   * @brief Создает объект класса T, инициализирует и возвращает его
   * @return созданный объект
   */
  virtual Value generate() noexcept(false) = 0;
};

#endif  // GENERATOR_H
