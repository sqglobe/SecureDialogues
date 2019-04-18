#ifndef CONNECTIONINFOMODEL_H
#define CONNECTIONINFOMODEL_H

#include "containermodelgenerator.h"
#include "containers/connectioninfocontainer.h"

QString to_qstring(const ConnectionInfoContainer::const_element& el) {
  return QString(el.getConnectionName().c_str());
}

/**
 * @brief Сгенерированный класс, который отображает список подключений.
 * Используется в диалоге управления подключениями
 */

GENERATE_CONTAINER_MODEL(ConnectionInfoModel, ConnectionInfoContainer)

#endif  // CONNECTIONINFOMODEL_H
