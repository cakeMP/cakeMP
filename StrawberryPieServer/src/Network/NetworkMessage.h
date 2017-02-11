#pragma once

#include <Common.h>

#include <enet/enet.h>

#define NETWORK_MESSAGE_DEFAULT_SIZE 32
#define NETWORK_MESSAGE_ALLOCATE_SIZE 64

class NetworkMessage
{
public:
	bool m_outgoing = false;
	uint32_t m_forPeer = 0xFFFFFFFF;

	uint8_t* m_data = nullptr;
	size_t m_length = 0;
	size_t m_lengthAlloc = 0;

private:
	size_t m_current = 0;
	ENetPacket* m_packet = nullptr;

public:
	NetworkMessage();
	NetworkMessage(uint32_t peer, ENetPacket* packet);
	~NetworkMessage();

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

	template<typename T>
	inline NetworkMessage &operator >>(T &dst)
	{
		Read(dst);
		return *this;
	}

	template<typename T>
	inline NetworkMessage &operator <<(const T &src)
	{
		Write(src);
		return *this;
	}

	inline NetworkMessage &operator <<(const char* src)
	{
		uint32_t len = strlen(src);
		Write(len);
		WriteRaw(src, len);
		return *this;
	}
};
