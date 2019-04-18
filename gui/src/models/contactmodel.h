#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include "containermodelgenerator.h"
#include "containers/contactcontainer.h"

inline QString to_qstring(const ContactContainer::const_element& el) {
  QString format("%1 (%2)");
  return format.arg(el->name().c_str(), el->adress().c_str());
}

/**
 * @brief Сгенерированный класс, который отображает список контактов.
 * Используется в диалоге управления контактами
 */
GENERATE_CONTAINER_MODEL(ContactModel, ContactContainer)

#endif  // CONTACTMODEL_H
