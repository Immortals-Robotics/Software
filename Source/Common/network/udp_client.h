#pragma once

#include <asio.hpp>
#include <google/protobuf/message_lite.h>

#include "../setting.h"

namespace Immortals
{
class UdpClient
{
public:
    explicit UdpClient(const NetworkAddress &t_address);

    bool receive(google::protobuf::MessageLite *t_message);

    [[nodiscard]] asio::ip::udp::endpoint getListenEndpoint() const
    {
        return m_listen_endpoint;
    }

    [[nodiscard]] asio::ip::udp::endpoint getLastReceiveEndpoint() const
    {
        return m_last_receive_endpoint;
    }

    [[nodiscard]] asio::ip::address getAddress() const
    {
        return m_address;
    }

    [[nodiscard]] bool isConnected() const
    {
        return m_socket->is_open();
    }

private:
    asio::ip::udp::endpoint m_listen_endpoint;
    asio::ip::address       m_address;

    asio::ip::udp::endpoint m_last_receive_endpoint;

    std::unique_ptr<asio::io_context>      m_context;
    std::unique_ptr<asio::ip::udp::socket> m_socket;

    std::array<char, Setting::kMaxUdpPacketSize> m_buffer = {};
};
} // namespace Immortals
