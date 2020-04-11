#include "smtpconnectionserializer.h"
#include <cstring>
#include "serialization-helpers.h"
#include "smtpconnectioninfo.h"
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
    buffer = serialization_helpers::save_str(conn->userName, buffer);
    buffer = serialization_helpers::save_str(conn->password, buffer);

    buffer = serialization_helpers::store_simple_type(conn->tlsUsed, buffer);
    buffer = serialization_helpers::save_str(conn->from, buffer);

    buffer = serialization_helpers::save_str(conn->imapAddr, buffer);
    buffer = serialization_helpers::store_simple_type(conn->imapPort, buffer);

    buffer = serialization_helpers::save_str(conn->smtpAddr, buffer);
    buffer = serialization_helpers::store_simple_type(conn->smtpPort, buffer);
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
    buffer = serialization_helpers::restore_str(conn->userName, buffer);
    buffer = serialization_helpers::restore_str(conn->password, buffer);

    buffer = serialization_helpers::restore_simple_type(conn->tlsUsed, buffer);
    buffer = serialization_helpers::restore_str(conn->from, buffer);

    buffer = serialization_helpers::restore_str(conn->imapAddr, buffer);
    buffer = serialization_helpers::restore_simple_type(conn->imapPort, buffer);

    buffer = serialization_helpers::restore_str(conn->smtpAddr, buffer);
    buffer = serialization_helpers::restore_simple_type(conn->smtpPort, buffer);
    return true;
  }
}
