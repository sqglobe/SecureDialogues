#ifndef FILEBUILDERS_H
#define FILEBUILDERS_H

#include "file_builder/connectionsavestrategy.h"
#include "file_builder/contactsavestrategy.h"
#include "file_builder/dialogsavestrategy.h"
#include "file_builder/filebuilder.h"

/**
 * Все функции возвращают построитель контейнеров из источника, для которого
 * определены свободные функции
 *  std::string getline(F &fileobject);
 *  bool at_end(F &fileObject);
 *  void writeline(F &fileObject, const std::string &)
 */

template <typename F, typename = std::enable_if<check_has_file_funct<F>::value>>
FileBuilder<F, ContactSaveStrategy> builder_realisation(
    F& dataSource,
    ContactContainer& container) {
  return FileBuilder<F, ContactSaveStrategy>(dataSource, container);
}

template <typename F, typename = std::enable_if<check_has_file_funct<F>::value>>
FileBuilder<F, DialogSaveStrategy> builder_realisation(
    F& dataSource,
    DialogManager& container) {
  return FileBuilder<F, DialogSaveStrategy>(dataSource, container);
}

template <typename F, typename = std::enable_if<check_has_file_funct<F>::value>>
FileBuilder<F, ConnectionSaveStrategy> builder_realisation(
    F& dataSource,
    ConnectionInfoContainer& container) {
  return FileBuilder<F, ConnectionSaveStrategy>(dataSource, container);
}

#endif  // FILEBUILDERS_H
