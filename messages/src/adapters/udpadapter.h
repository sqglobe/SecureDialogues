#ifndef UDPADAPTER_H
#define UDPADAPTER_H

#include "interfaces/abstractchanneladapter.h"

#include <memory>

#include <asio/ip/udp.hpp>

class UdpAdapter : public AbstractChannelAdapter {
 public:
  using AbstractChannelAdapter::AbstractChannelAdapter;

 public:
  virtual void send(const std::string& message,
                    const std::string& adress) noexcept(false);
  virtual std::pair<std::string, std::string> receive() noexcept(false);
  virtual bool connect(const ConnectionHolder&);
  virtual bool isConnected() const noexcept;

 private:
  std::unique_ptr<asio::ip::udp::socket> mSocket;
};

#endif  // UDPADAPTER_H
