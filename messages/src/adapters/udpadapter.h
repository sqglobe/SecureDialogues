#ifndef UDPADAPTER_H
#define UDPADAPTER_H

#include "interfaces/abstractchanneladapter.h"

#include <memory>

#include <boost/asio/ip/udp.hpp>

class UdpAdapter : public AbstractChannelAdapter {
 public:
  using AbstractChannelAdapter::AbstractChannelAdapter;

 public:
  virtual void send(const std::string& message,
                    const std::string& adress) noexcept(false) override;
  virtual std::pair<std::string, std::string> receive() noexcept(
      false) override;
  virtual bool connect() override;

 private:
  std::unique_ptr<boost::asio::ip::udp::socket> mSocket;
};

#endif  // UDPADAPTER_H
