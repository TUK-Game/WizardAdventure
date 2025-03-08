#pragma once
#include "Protocol.pb.h"
#include "PacketFlag.h"

extern std::function<bool(CPacketSessionRef&, BYTE*, int32)> g_ServerPacketHandler[UINT16_MAX];

// Custom Handlers
bool Handle_C_INVALID(CPacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_C_ENTER_GAME(CPacketSessionRef& session, Protocol::C_ENTER_GAME& pkt);
bool Handle_C_MOVE(CPacketSessionRef& session, Protocol::C_MOVE& pkt);

class ServerPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			g_ServerPacketHandler[i] = Handle_C_INVALID;
		g_ServerPacketHandler[PKT_C_ENTER_GAME] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ENTER_GAME>(Handle_C_ENTER_GAME, session, buffer, len); };
		g_ServerPacketHandler[PKT_C_MOVE] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_MOVE>(Handle_C_MOVE, session, buffer, len); };
	}

	static bool HandlePacket(CPacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return g_ServerPacketHandler[header->Id](session, buffer, len);
	}
	static CSendBufferRef MakeSendBuffer(Protocol::S_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_S_ENTER_GAME); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_SPAWN& pkt) { return MakeSendBuffer(pkt, PKT_S_SPAWN); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_DESPAWN& pkt) { return MakeSendBuffer(pkt, PKT_S_DESPAWN); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_MOVE& pkt) { return MakeSendBuffer(pkt, PKT_S_MOVE); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, CPacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static CSendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		CSendBufferRef sendBuffer = std::make_shared<CSendBuffer>(packetSize);

		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->Size = packetSize;
		header->Id = pktId;
		pkt.SerializeToArray(&header[1], dataSize);
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};