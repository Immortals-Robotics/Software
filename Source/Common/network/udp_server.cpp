#include "udp_server.h"

#include <quill/Quill.h>

namespace Immortals
{
UdpServer::UdpServer()
{
    m_context = std::make_unique<asio::io_context>();

    m_socket = std::make_unique<asio::ip::udp::socket>(*m_context, asio::ip::udp::v4());
}

bool UdpServer::send(const google::protobuf::MessageLite &t_message, const NetworkAddress &t_address)
{
    if (!t_message.SerializeToArray(m_buffer.data(), m_buffer.size())){
        LOG_ERROR("Failed to serializeToArray (Maybe need to adjust Setting::kMaxUdpPacketSize)");
	return false;
    }

    const size_t serialized_size = t_message.ByteSizeLong();

    return send(serialized_size, t_address);
}

bool UdpServer::send(const size_t t_size, const NetworkAddress &t_address)
{
    const asio::ip::address_v4    address = asio::ip::make_address_v4(t_address.ip);
    const asio::ip::udp::endpoint endpoint{address, t_address.port};

    const size_t m_sent_size = m_socket->send_to(asio::buffer(m_buffer, t_size), endpoint);

    return m_sent_size == t_size;
}
} // namespace Immortals
