#ifndef CONNECTIONHOLDER_H
#define CONNECTIONHOLDER_H

#include <any>
#include <string>
#include "communication/channel.h"

enum class ConnectionType { UNDEF = 0, UDP, GMAIL, VK, EMAIL };

struct UdpConnection {};

struct GmailConnection {
  std::string email;
  std::string accessToken;
};

struct VkConnection {
  std::string userId;
  std::string accessToken;
};

struct EmailConnection {
  std::string userName;
  std::string password;
  bool tlsUsed;
  std::string from;

  std::string smtpAddr;
  int smtpPort;

  std::string imapAddr;
  int imapPort;
};

template <typename T>
struct ConnectionTraits;

template <>
struct ConnectionTraits<UdpConnection> {
  static const ConnectionType TYPE = ConnectionType::UDP;
  using ConnectionClass = UdpConnection;
};

template <>
struct ConnectionTraits<GmailConnection> {
  static const ConnectionType TYPE = ConnectionType::GMAIL;
  using ConnectionClass = GmailConnection;
};

template <>
struct ConnectionTraits<VkConnection> {
  static const ConnectionType TYPE = ConnectionType::VK;
  using ConnectionClass = VkConnection;
};

template <>
struct ConnectionTraits<EmailConnection> {
  static const ConnectionType TYPE = ConnectionType::EMAIL;
  using ConnectionClass = EmailConnection;
};

class ConnectionHolder {
 public:
  template <class C>
  ConnectionHolder(const C& conn, std::string name);
  ConnectionHolder() = default;
  explicit ConnectionHolder(std::string name);
  ConnectionHolder(ConnectionHolder&& holder) noexcept = default;
  ConnectionHolder(const ConnectionHolder& holder) = default;
  ConnectionHolder& operator=(const ConnectionHolder& holder) = default;

 public:
  template <class C>
  void setConnection(const C& conn);
  template <class C>
  C getConnection() const;
  template <class C>
  bool hasConnection() const;

 public:
  ConnectionType getType() const;
  std::string getConnectionName() const;
  void setConnectionName(const std::string& name);

  Channel::ChannelStatus getStatus() const;
  void setStatus(const Channel::ChannelStatus& status);

  std::string getMessage() const;
  void setMessage(const std::string& message);

 private:
  std::string mConnName;
  ConnectionType mType{ConnectionType::UNDEF};
  std::any mData;
  Channel::ChannelStatus mStatus{Channel::ChannelStatus::UNDEFINED};
  std::string mMessage;
};

template <class C>
ConnectionHolder::ConnectionHolder(const C& conn, std::string name) :
    mConnName(std::move(name)) {
  setConnection(conn);
}

template <class C>
bool ConnectionHolder::hasConnection() const {
  return mData.has_value() && mType == ConnectionTraits<C>::TYPE;
}

template <class C>
C ConnectionHolder::getConnection() const {
  return std::any_cast<C>(mData);
}

template <class C>
void ConnectionHolder::setConnection(const C& conn) {
  mType = ConnectionTraits<C>::TYPE;
  mData = conn;
}

#endif  // CONNECTIONHOLDER_H
