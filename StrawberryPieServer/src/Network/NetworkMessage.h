#pragma once

#include <Common.h>

#include <enet/enet.h>

#define NETWORK_MESSAGE_DEFAULT_SIZE 32
#define NETWORK_MESSAGE_ALLOCATE_SIZE 64

class NetworkMessage
{
public:
	bool m_outgoing = false;
	ENetPeer* m_forPeer = nullptr;

	uint8_t* m_data = nullptr;
	size_t m_length = 0;

private:
	size_t m_current = 0;
	ENetPacket* m_packet = nullptr;
	bool m_reliable = true;

public:
	NetworkMessage();
	NetworkMessage(ENetPeer* peer, ENetPacket* packet);
	~NetworkMessage();

	inline bool Reliable() { return m_reliable; }
	inline void Reliable(bool reliable) { m_reliable = reliable; }

	int ReadRaw(void* dst, size_t sz);
	void WriteRaw(const void* src, size_t sz);

	template<typename T>
	inline void Read(T &dst)
	{
		int sz = ReadRaw(&dst, sizeof(T));
		assert(sz == sizeof(T));
	}

	template<typename T>
	inline void Write(const T &src)
	{
		WriteRaw(&src, sizeof(T));
	}

	inline void Write(const char* src)
	{
		uint32_t len = strlen(src);
		Write(len);
		WriteRaw(src, len);
	}

	inline void Write(const std::string &src)
	{
		Write((uint32_t)src.size());
		WriteRaw(src.c_str(), src.size());
	}
};
