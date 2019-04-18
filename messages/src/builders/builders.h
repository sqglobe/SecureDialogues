#ifndef BUILDERS_H
#define BUILDERS_H

#include "filebuilders.h"

/**
 * Входная точка для создания классов-билдеров, которые заполняют данными из
 * dataSource контейнер container. Или наоборот сохраняют.
 */
template <typename D, typename C>
auto make_builder(D& dataSource, C& container)
    -> decltype(builder_realisation(dataSource, container)) {
  return builder_realisation(dataSource, container);
}

#endif  // BUILDERS_H
