#include "smtpconnectionserializer.h"
#include <cstring>
#include "persistent-storage/utils/store_primitives.h"
#include "smtpconnectioninfo.h"
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

uint32_t SmtpConnectionSerializer::get_size(
    const PluginConnectionInfo* connInfo) const noexcept {
  if (const auto* conn = dynamic_cast<const SmtpConnectionInfo*>(connInfo);
      conn == nullptr) {
    return 0;
  } else {
    uint32_t size = 0;
    size += sizeof(std::string::size_type) + conn->password.length();

    size += sizeof(conn->tlsUsed);

    size += sizeof(std::string::size_type) + conn->from.length();

    size += sizeof(std::string::size_type) + conn->imapAddr.length();
    size += sizeof(conn->imapPort);

    size += sizeof(std::string::size_type) + conn->smtpAddr.length();
    size += sizeof(conn->smtpPort);
    size += sizeof(std::string::size_type) + conn->userName.length();
    return size;
  }
}

bool SmtpConnectionSerializer::serialize(
    void* buffer,
    const PluginConnectionInfo* connInfo) const noexcept {
  if (const auto* conn = dynamic_cast<const SmtpConnectionInfo*>(connInfo);
      conn == nullptr) {
    return false;
  } else {
    buffer = prstorage::save_str(conn->userName, buffer);
    buffer = prstorage::save_str(conn->password, buffer);

    buffer = store_simple_type(conn->tlsUsed, buffer);
    buffer = prstorage::save_str(conn->from, buffer);

    buffer = prstorage::save_str(conn->imapAddr, buffer);
    buffer = store_simple_type(conn->imapPort, buffer);

    buffer = prstorage::save_str(conn->smtpAddr, buffer);
    buffer = store_simple_type(conn->smtpPort, buffer);
    return true;
  }
}

bool SmtpConnectionSerializer::deserialize(const void* buffer,
                                           PluginConnectionInfo* connInfo) const
    noexcept {
  if (auto* conn = dynamic_cast<SmtpConnectionInfo*>(connInfo);
      conn == nullptr) {
    return false;
  } else {
    buffer = prstorage::restore_str(conn->userName, buffer);
    buffer = prstorage::restore_str(conn->password, buffer);

    buffer = restore_simple_type(conn->tlsUsed, buffer);
    buffer = prstorage::restore_str(conn->from, buffer);

    buffer = prstorage::restore_str(conn->imapAddr, buffer);
    buffer = restore_simple_type(conn->imapPort, buffer);

    buffer = prstorage::restore_str(conn->smtpAddr, buffer);
    buffer = restore_simple_type(conn->smtpPort, buffer);
    return true;
  }
}
