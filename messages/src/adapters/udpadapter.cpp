#include "udpadapter.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <strings.h>
#include <boost/asio.hpp>
#include "exception/receiveexception.h"
#include "exception/sendexception.h"
#include "primitives/connectionholder.h"

#include "spdlog/spdlog.h"

static std::shared_ptr<spdlog::logger> LOGGER =
    spdlog::stdout_color_mt("udp-adapter");

void UdpAdapter::send(const std::string& message, const std::string& adress) {
  using boost::asio::ip::udp;
  udp::endpoint remote_endpoint(
      boost::asio::ip::address_v4::from_string(adress), 5555);

  boost::asio::io_service io_context;

  boost::asio::ip::udp::socket socket(io_context);
  socket.open(boost::asio::ip::udp::v4());

  socket.send_to(boost::asio::const_buffers_1(&message[0], message.size()),
                 remote_endpoint);
}

std::pair<std::string, std::string> UdpAdapter::receive() {
  using boost::asio::ip::udp;
  std::string messg(2000, '\0');
  if (mSocket) {
    udp::endpoint remote_endpoint;
    mSocket->receive_from(boost::asio::buffer(&messg[0], messg.size()),
                          remote_endpoint);

    return std::make_pair(remote_endpoint.address().to_string(),
                          std::string(messg));
  }

  return std::make_pair(std::string(), std::string());
}

bool UdpAdapter::connect() {
  using boost::asio::ip::udp;

  boost::asio::io_service io_context;

  mSocket = std::make_unique<boost::asio::ip::udp::socket>(io_context);
  boost::system::error_code ec;
  mSocket->open(boost::asio::ip::udp::v4());
  mSocket->bind(udp::endpoint(udp::v4(), 5555), ec);

  if (ec) {
    LOGGER->error("Get error while created endpoint: {0}", ec.message());
  }

  return ec.value() == 0;
}
