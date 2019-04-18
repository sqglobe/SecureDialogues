#ifndef CONNECTIONINFOCONTAINER_H
#define CONNECTIONINFOCONTAINER_H

#include "basecontainer.h"
#include "primitives/connectionholder.h"

class MessageDespatcher;
class AbstractChannelAdapter;

/**
 * @brief Контейнер для хранения информации о подключении для каналов.
 */
class ConnectionInfoContainer
    : public BaseContainer<ConnectionHolder, ConnectionHolder> {
 public:
  ConnectionInfoContainer() = default;

 private:
};

std::string get_id(const ConnectionInfoContainer::element& elem);

#endif  // CONNECTIONINFOCONTAINER_H
