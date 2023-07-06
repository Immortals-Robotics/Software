#include "Vision.h"

bool VisionModule::connectToVisionServer ( void )
{
	m_visionUDP = std::make_unique<UdpClient>(setting().vision_address);
	return isConnected();
}

bool VisionModule::recievePacket ( void )
{
	if (!isConnected())
		return false;

	if (m_visionUDP->receive(&packet))
	{
		if (packet.has_detection())
		{
			frame[packet.detection().camera_id()] = packet.detection();
			packet_recieved[packet.detection().camera_id()] = true;
		}

		return true;
	}

	return false;
}

bool VisionModule::isConnected ( void )
{
	return m_visionUDP != nullptr && m_visionUDP->isConnected();
}
