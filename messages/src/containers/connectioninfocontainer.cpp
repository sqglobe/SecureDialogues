#include "connectioninfocontainer.h"

std::string get_id(const ConnectionInfoContainer::element& elem) {
  return elem.getConnectionName();
}
