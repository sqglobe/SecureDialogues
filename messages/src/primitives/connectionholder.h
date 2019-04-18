#ifndef CONNECTIONHOLDER_H
#define CONNECTIONHOLDER_H

#include <any>
#include <string>

enum class ConnectionType { UNDEF = 0, UDP, GMAIL, VK };

struct UdpConnection {};

struct GmailConnaection {
  std::string email;
  std::string accessToken;
};

struct VkConnection {
  std::string userId;
  std::string accessToken;
};

template <typename T>
struct ConnectionTraits;

template <>
struct ConnectionTraits<UdpConnection> {
  static const ConnectionType TYPE = ConnectionType::UDP;
  using ConnectionClass = UdpConnection;
};

template <>
struct ConnectionTraits<GmailConnaection> {
  static const ConnectionType TYPE = ConnectionType::GMAIL;
  using ConnectionClass = GmailConnaection;
};

template <>
struct ConnectionTraits<VkConnection> {
  static const ConnectionType TYPE = ConnectionType::VK;
  using ConnectionClass = VkConnection;
};

class ConnectionHolder {
 public:
  template <class C>
  ConnectionHolder(const C& conn, const std::string& name);
  explicit ConnectionHolder(const std::string& name);

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

 private:
  std::string mConnName;
  ConnectionType mType;
  std::any mData;
};

template <class C>
ConnectionHolder::ConnectionHolder(const C& conn, const std::string& name) :
    mConnName(name) {
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
