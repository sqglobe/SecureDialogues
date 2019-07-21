#include "connectionmarshaller.h"

#include "persistent-storage/store_primitives.h"

#include <cstring>

/*
  std::string userName;
  std::string password;
  bool tlsUsed;
  std::string from;
  */

template <typename T>
const void* restore_simple_type(T& t, const void* src) {
  memcpy(&t, src, sizeof(t));
  return static_cast<const char*>(src) + sizeof(t);
}

template <typename T>
void* store_simple_type(T t, void* src) {
  memcpy(src, &t, sizeof(t));
  return static_cast<char*>(src) + sizeof(t);
}

void ConnectionMarshaller::restore(ConnectionHolder& elem, const void* src) {
  std::string connName;
  ConnectionType type;
  src = prstorage::restore_str(connName, src);
  src = restore_simple_type(type, src);

  elem.setConnectionName(connName);

  if (type == ConnectionType::VK) {
    VkConnection conn;
    src = prstorage::restore_str(conn.userId, src);
    src = prstorage::restore_str(conn.accessToken, src);
    elem.setConnection(conn);
  } else if (type == ConnectionType::UDP) {
    elem.setConnection(UdpConnection{});
  } else if (type == ConnectionType::EMAIL) {
    EmailConnection conn;
    src = prstorage::restore_str(conn.userName, src);
    src = prstorage::restore_str(conn.password, src);

    src = restore_simple_type(conn.tlsUsed, src);
    src = prstorage::restore_str(conn.from, src);

    src = prstorage::restore_str(conn.imapAddr, src);
    src = restore_simple_type(conn.imapPort, src);

    src = prstorage::restore_str(conn.smtpAddr, src);
    src = restore_simple_type(conn.smtpPort, src);
    elem.setConnection(conn);
  } else if (type == ConnectionType::GMAIL) {
    GmailConnection conn;

    src = prstorage::restore_str(conn.email, src);
    src = prstorage::restore_str(conn.accessToken, src);

    elem.setConnection(conn);
  }
}

size_t ConnectionMarshaller::size(const ConnectionHolder& element) {
  size_t size = 0;
  size += sizeof(std::string::size_type) + element.getConnectionName().length();
  size += sizeof(element.getType());

  if (element.getType() == ConnectionType::VK) {
    const auto& conn = element.getConnection<VkConnection>();
    size += sizeof(std::string::size_type) + conn.userId.length();
    size += sizeof(std::string::size_type) + conn.accessToken.length();
  } else if (element.getType() == ConnectionType::UDP) {
    // do this empty
  } else if (element.getType() == ConnectionType::EMAIL) {
    const auto& conn = element.getConnection<EmailConnection>();

    size += sizeof(std::string::size_type) + conn.userName.length();
    size += sizeof(std::string::size_type) + conn.password.length();

    size += sizeof(conn.tlsUsed);

    size += sizeof(std::string::size_type) + conn.from.length();

    size += sizeof(std::string::size_type) + conn.imapAddr.length();
    size += sizeof(conn.imapPort);

    size += sizeof(std::string::size_type) + conn.smtpAddr.length();
    size += sizeof(conn.smtpPort);
  } else if (element.getType() == ConnectionType::GMAIL) {
    const auto& conn = element.getConnection<GmailConnection>();

    size += sizeof(std::string::size_type) + conn.email.length();
    size += sizeof(std::string::size_type) + conn.accessToken.length();
  }

  return size;
}

void ConnectionMarshaller::store(void* dest, const ConnectionHolder& elem) {
  dest = prstorage::save_str(elem.getConnectionName(), dest);
  dest = store_simple_type(elem.getType(), dest);

  if (elem.getType() == ConnectionType::VK) {
    const auto& conn = elem.getConnection<VkConnection>();
    dest = prstorage::save_str(conn.userId, dest);
    dest = prstorage::save_str(conn.accessToken, dest);
  } else if (elem.getType() == ConnectionType::UDP) {
    // do this empty
  } else if (elem.getType() == ConnectionType::EMAIL) {
    const auto& conn = elem.getConnection<EmailConnection>();

    dest = prstorage::save_str(conn.userName, dest);
    dest = prstorage::save_str(conn.password, dest);

    dest = store_simple_type(conn.tlsUsed, dest);
    dest = prstorage::save_str(conn.from, dest);

    dest = prstorage::save_str(conn.imapAddr, dest);
    dest = store_simple_type(conn.imapPort, dest);

    dest = prstorage::save_str(conn.smtpAddr, dest);
    dest = store_simple_type(conn.smtpPort, dest);
  } else if (elem.getType() == ConnectionType::GMAIL) {
    const auto& conn = elem.getConnection<GmailConnection>();

    dest = prstorage::save_str(conn.email, dest);
    dest = prstorage::save_str(conn.accessToken, dest);
  }
}
