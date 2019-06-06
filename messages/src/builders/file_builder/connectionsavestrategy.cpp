#include "connectionsavestrategy.h"
#include <sstream>
#include "interfaces/changewatcher.h"
#include "primitives/connectionholder.h"
#include "utils.h"

bool is_num(const std::string& str) {
  return !str.empty() &&
         std::find_if(std::cbegin(str), std::cend(str),
                      [](auto c) { return std::isdigit(c); }) != std::cend(str);
}

std::vector<typename ConnectionSaveStrategy::container::const_element>
ConnectionSaveStrategy::getContainerData(
    const ConnectionSaveStrategy::container& container) {
  return container.getAllElements();
}
/*
 * struct EmailConnection {
    std::string userName;
    std::string password;
    bool tlsUsed;
    std::string from;

    std::string smtpAddr;
    int smtpPort;

    std::string imapAddr;
    int imapPort;
};

 */
std::string ConnectionSaveStrategy::to_str(
    const ConnectionSaveStrategy::container::const_element& obj) {
  std::stringstream ss;

  ss << obj.getConnectionName() << ";" << static_cast<int>(obj.getType());

  switch (obj.getType()) {
    case ConnectionType::UDP:
      break;
    case ConnectionType::GMAIL: {
      auto conn = obj.getConnection<GmailConnection>();
      ss << ";" << conn.email << ";" << conn.accessToken;
      break;
    }
    case ConnectionType::VK: {
      auto conn = obj.getConnection<VkConnection>();
      ss << ";" << conn.userId << ";" << conn.accessToken;
      break;
    }
    case ConnectionType::EMAIL: {
      auto conn = obj.getConnection<EmailConnection>();
      ss << ";" << conn.userName << ";" << conn.password << ";" << conn.tlsUsed
         << ";" << conn.from << ";" << conn.smtpAddr << ";" << conn.smtpPort
         << ";" << conn.imapAddr << ";" << conn.imapPort;
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
      if (cols.size() == 4)
        obj.setConnection(GmailConnection{cols[2], cols[3]});
      break;
    }
    case ConnectionType::VK: {
      if (cols.size() == 4)
        obj.setConnection(VkConnection{cols[2], cols[3]});
      break;
    }
    case ConnectionType::EMAIL: {
      auto [userName, password, tlsUsed, from, smtpAddr, smtpPort, imapAddr,
            imapPort] = std::tie(cols[2], cols[3], cols[4], cols[5], cols[6],
                                 cols[7], cols[8], cols[9]);
      if (cols.size() == 10 && is_num(tlsUsed) && is_num(smtpPort) &&
          is_num(imapPort)) {
        obj.setConnection(EmailConnection{
            userName, password, std::stoi(tlsUsed) == 1, from, smtpAddr,
            std::stoi(smtpPort), imapAddr, std::stoi(imapPort)});
      }
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
