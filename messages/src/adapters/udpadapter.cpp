#include "udpadapter.h"

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include "exception/receiveexception.h"
#include "exception/sendexception.h"
#include "primitives/connectionholder.h"
#include "utils/closer.h"
#include <asio.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "spdlog/spdlog.h"

static std::shared_ptr<spdlog::logger> LOGGER = spdlog::stdout_color_mt("udp-adapter");

void UdpAdapter::send(const std::string& message, const std::string& adress) {

  using asio::ip::udp;
  udp::endpoint remote_endpoint(asio::ip::address_v4::from_string(adress), 5555);

  asio::io_service io_context;

  asio::ip::udp::socket socket(io_context);
  socket.open(asio::ip::udp::v4());

  socket.send_to(asio::const_buffers_1(&message[0], message.size()), remote_endpoint);
}

std::pair<std::string, std::string> UdpAdapter::receive() {
  using asio::ip::udp;
  std::string messg(2000, '\0');
  if (mSocket) {
    udp::endpoint remote_endpoint;
    mSocket->receive_from(asio::buffer(&messg[0], messg.size()), remote_endpoint);

    return std::make_pair(remote_endpoint.address().to_string(),
                          std::string(messg.c_str()));
  }

  return std::make_pair(std::string(), std::string());
}

bool UdpAdapter::connect(const ConnectionHolder&) {
    using asio::ip::udp;

    asio::io_service io_context;

    mSocket = std::make_unique<asio::ip::udp::socket>(io_context);
    asio::error_code ec;
    mSocket->open(asio::ip::udp::v4());
    mSocket->bind(udp::endpoint(udp::v4(), 5555), ec);

    if(ec){
        LOGGER->error("Get error while created endpoint: {0}", ec.message());
    }

    return ec.value() == 0;
}

bool UdpAdapter::isConnected() const noexcept {
  return true;
}
