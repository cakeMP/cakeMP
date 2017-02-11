#include <Common.h>

#include <Network/NetworkMessage.h>

NetworkMessage::NetworkMessage()
{
	m_outgoing = true;
}

NetworkMessage::NetworkMessage(ENetPeer* peer, ENetPacket* packet)
{
	m_forPeer = peer;

	m_data = packet->data;
	m_length = packet->dataLength;

	m_packet = packet;
	m_reliable = (packet->flags & ENET_PACKET_FLAG_RELIABLE) != 0;
}

NetworkMessage::~NetworkMessage()
{
	if (m_packet != nullptr) {
		enet_packet_destroy(m_packet);
	} else if (m_data != nullptr) {
		free(m_data);
	}
}

int NetworkMessage::ReadRaw(void *dst, size_t sz)
{
	assert(!m_outgoing);
	assert(m_length <= m_length);

	sz = std::min(sz, m_length - m_current);

	assert(sz > 0);
	if (sz == 0) {
		return 0;
	}

	memcpy(dst, m_data + m_current, sz);
	m_current += sz;

	return sz;
}

void NetworkMessage::WriteRaw(const void* src, size_t sz)
{
	assert(m_outgoing);

	if (m_data == nullptr) {
		m_length = NETWORK_MESSAGE_DEFAULT_SIZE;
		m_data = (uint8_t*)malloc(m_length);
	}

	bool mustRealloc = false;
	while (m_current + sz > m_length) {
		m_length += NETWORK_MESSAGE_ALLOCATE_SIZE;
		mustRealloc = true;
	}

	if (mustRealloc) {
		m_data = (uint8_t*)realloc(m_data, m_length);
	}

	memcpy(m_data + m_current, src, sz);
}
