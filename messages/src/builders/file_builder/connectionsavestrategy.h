#ifndef CONNECTINSAVESTRATEGY_H
#define CONNECTINSAVESTRATEGY_H

#include <vector>
#include "containers/connectioninfocontainer.h"

/**
 * Реализует необходимый функционал для сохранения/извлечения элементов
 * контейнера из указанного ресурса
 */
class ConnectionSaveStrategy {
 public:
  using container = ConnectionInfoContainer;

 public:
  /**
   * @brief Возвращает все элементы контейнера в виде вектора.
   * Реализация необходима для использования в родительском классе.
   * @param container контейнер, для которого должен быть возвращен список
   * элементов.
   * @return список элементов контейнера
   */
  static std::vector<typename container::const_element> getContainerData(
      const container& container);

  /**
   * @brief Превращает элемента контейнера в строку
   * @param obj элемент контейнера для сериализации в строку
   * @return строковое представление элемента контейнера
   */
  static std::string to_str(const typename container::const_element& obj);

  /**
   * @brief Конструирует элемент контейнера по его строковому представлению
   * @param row строковое представление элемента контейнера
   * @return элемент контейнера
   */
  static typename container::element from_str(const std::string& row);

  /**
   * @brief добавляет элемент в контейнер
   * @param container контейнер для добавления
   * @param obj элемент, который необходимо добавить
   */
  static void add(container& container, const typename container::element& obj);
};

#endif  // CONNECTINSAVESTRATEGY_H
