#include "connectionsavestrategy.h"
#include <sstream>
#include "interfaces/changewatcher.h"
#include "primitives/connectionholder.h"
#include "utils.h"

std::vector<typename ConnectionSaveStrategy::container::const_element>
ConnectionSaveStrategy::getContainerData(
    const ConnectionSaveStrategy::container& container) {
  return container.getAllElements();
}

std::string ConnectionSaveStrategy::to_str(
    const ConnectionSaveStrategy::container::const_element& obj) {
  std::stringstream ss;

  ss << obj.getConnectionName() << ";" << static_cast<int>(obj.getType());

  switch (obj.getType()) {
    case ConnectionType::UDP:
      break;
    case ConnectionType::GMAIL: {
      auto conn = obj.getConnection<GmailConnaection>();
      ss << ";" << conn.email << ";" << conn.accessToken;
      break;
    }
    case ConnectionType::VK: {
      auto conn = obj.getConnection<VkConnection>();
      ss << ";" << conn.userId << ";" << conn.accessToken;
      break;
    }
    default:
      assert(false);
  }
  return ss.str();
}

ConnectionSaveStrategy::container::element ConnectionSaveStrategy::from_str(
    const std::string& row) {
  std::vector<std::string> cols;
  split(row, ';', std::back_insert_iterator<std::vector<std::string>>(cols));
  auto type = static_cast<ConnectionType>(std::stoi(cols[1]));
  auto obj = ConnectionHolder(cols[0]);
  switch (type) {
    case ConnectionType::UDP:
      obj.setConnection(UdpConnection{});
      break;
    case ConnectionType::GMAIL: {
      obj.setConnection(GmailConnaection{cols[2], cols[3]});
      break;
    }
    case ConnectionType::VK: {
      obj.setConnection(VkConnection{cols[2], cols[3]});
      break;
    }
    default:
      assert(false);
  }

  return obj;
}

void ConnectionSaveStrategy::add(
    ConnectionSaveStrategy::container& container,
    const ConnectionSaveStrategy::container::element& obj) {
  container.add(obj);
}
