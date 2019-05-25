#include "connectioninfocontainer.h"
#include "interfaces/changewatcher.h"
std::string get_id(const ConnectionInfoContainer::element& elem) {
  return elem.getConnectionName();
}
