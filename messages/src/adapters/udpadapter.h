#ifndef UDPADAPTER_H
#define UDPADAPTER_H

#include "interfaces/abstractchanneladapter.h"

#include <memory>

#include <boost/asio/ip/udp.hpp>

class UdpAdapter : public AbstractChannelAdapter {
 public:
  using AbstractChannelAdapter::AbstractChannelAdapter;

 public:
  void send(const std::string& message,
            const std::string& adress) noexcept(false) override;
  std::list<std::pair<std::string, std::string>> receive() noexcept(
      false) override;
  bool connect() override;

 private:
  std::unique_ptr<boost::asio::ip::udp::socket> mSocket;
};

#endif  // UDPADAPTER_H
