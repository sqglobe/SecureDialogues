#ifndef ABSTRACTFILEBUILDER_H
#define ABSTRACTFILEBUILDER_H

#include <string>

#include <assert.h>
#include <type_traits>

/**
 *  Needed functions:
 *  std::string getline(F &fileobject);
 *  bool at_end(F &fileObject);
 *  void writeline(F &fileObject, const std::string &)
 */

/**
 * Во время компиляции производит проверку, что класс T имеет определенные для
 * него свободные функции: std::string getline(T &fileobject); bool at_end(T
 * &fileObject); void writeline(T &fileObject, const std::string &)
 */
template <typename T>
struct check_has_file_funct {
  static int detect_get_line(...);
  template <typename U>
  static decltype(getline(std::declval<U&>())) detect_get_line(U);

  static int detect_at_end(...);
  template <typename U>
  static decltype(at_end(std::declval<U&>())) detect_at_end(U);

  static int detect_writeline(...);
  template <typename U>
  static decltype(
      writeline(std::declval<U&>(),
                std::declval<const std::string&>())) detect_writeline(U);

  static const bool value =
      std::is_same<std::string,
                   decltype(detect_get_line(std::declval<T>()))>::value &&
      std::is_same<bool, decltype(detect_at_end(std::declval<T>()))>::value &&
      std::is_same<void, decltype(detect_writeline(std::declval<T>()))>::value;
};

/**
 * Реализует логику чтения строк из источника, добавления объекта в контейнер,
 * или сохранение строк в источник. Превращение элементов контейнера в строку и
 * обратно реализует дочерний класс
 *
 * @param SaveStrategy - класс стратегии, который реализует методы для
 * сериализации/десериализации элементов контейнера
 * @param F - источник данных
 * @param C - класс контейнера
 *
 * В дочернем классе необходимо реализовать публичные методы:
 *   для получения списка элементов контейнера в виде массива
 *   std::vector<typename ContactFileBuilder::const_element>
 * getContainerData(const typename ContactFileBuilder::container &container)
 * const;
 *
 *   для сериализации элемента массива в контейнер
 *   std::string to_str(const typename ContactFileBuilder::const_element &obj)
 * const;
 *
 *   для десериализации элемента контейнера из строки
 *   typename ContactFileBuilder::element from_str(const std::string &row)
 * const;
 *
 *   выполняет добавление элемента в контейнер
 *   void add(typename ContactFileBuilder::container &container, const typename
 * ContactFileBuilder::element &obj);
 */
template <typename F, typename SaveStrategy>
class FileBuilder : public SaveStrategy {
 public:
  using container = typename SaveStrategy::container;
  using element = typename SaveStrategy::container;
  using const_element = typename SaveStrategy::container;

 public:
  FileBuilder(F& fileObj, container& container);

 public:
  /**
   * @brief Сериализует элементы контейнера в строки и сохраняет их в указанном
   * источнике
   */
  void serialize();

  /**
   * @brief Десериализует элементы из указанного источника и добавляет их в
   * контейнер
   */
  void unserialize();

  // needed methods
 public:
  // std::vector<typename ContactFileBuilder::const_element>
  // getContainerData(const typename ContactFileBuilder::container &container)
  // const; std::string to_str(const typename ContactFileBuilder::const_element
  // &obj) const; typename ContactFileBuilder::element from_str(const
  // std::string &row) const; void add(typename ContactFileBuilder::container
  // &container, const typename ContactFileBuilder::element &obj);

 private:
  F& mfileObject;
  container& mContainer;
};

template <typename F, typename SaveStrategy>
FileBuilder<F, SaveStrategy>::FileBuilder(F& fileObj,
                                          FileBuilder::container& container) :
    mfileObject(fileObj),
    mContainer(container) {
  static_assert(check_has_file_funct<F>::value,
                "File object is not has a needed methods");
}

template <typename F, typename SaveStrategy>
void FileBuilder<F, SaveStrategy>::serialize() {
  auto cont_data = SaveStrategy::getContainerData(mContainer);
  for (auto obj : cont_data) {
    std::string line = SaveStrategy::to_str(obj);
    writeline(mfileObject, line);
  }
}

template <typename F, typename SaveStrategy>
void FileBuilder<F, SaveStrategy>::unserialize() {
  while (!at_end(mfileObject)) {
    std::string line = getline(mfileObject);
    if (!line.empty()) {
      auto obj = SaveStrategy::from_str(line);
      SaveStrategy::add(mContainer, obj);
    }
  }
}

#endif  // ABSTRACTFILEBUILDER_H
