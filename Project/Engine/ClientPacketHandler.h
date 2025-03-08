#pragma once
#include "Protocol.pb.h"
#include "PacketFlag.h"

extern std::function<bool(CPacketSessionRef&, BYTE*, int32)> g_ClientPacketHandler[UINT16_MAX];

// Custom Handlers
bool Handle_S_INVALID(CPacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_S_ENTER_GAME(CPacketSessionRef& session, Protocol::S_ENTER_GAME& pkt);
bool Handle_S_SPAWN(CPacketSessionRef& session, Protocol::S_SPAWN& pkt);
bool Handle_S_DESPAWN(CPacketSessionRef& session, Protocol::S_DESPAWN& pkt);
bool Handle_S_MOVE(CPacketSessionRef& session, Protocol::S_MOVE& pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			g_ClientPacketHandler[i] = Handle_S_INVALID;
		g_ClientPacketHandler[PKT_S_ENTER_GAME] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ENTER_GAME>(Handle_S_ENTER_GAME, session, buffer, len); };
		g_ClientPacketHandler[PKT_S_SPAWN] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWN>(Handle_S_SPAWN, session, buffer, len); };
		g_ClientPacketHandler[PKT_S_DESPAWN] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DESPAWN>(Handle_S_DESPAWN, session, buffer, len); };
		g_ClientPacketHandler[PKT_S_MOVE] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MOVE>(Handle_S_MOVE, session, buffer, len); };
	}

	static bool HandlePacket(CPacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return g_ClientPacketHandler[header->Id](session, buffer, len);
	}
	static CSendBufferRef MakeSendBuffer(Protocol::C_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_ENTER_GAME); }
	static CSendBufferRef MakeSendBuffer(Protocol::C_MOVE& pkt) { return MakeSendBuffer(pkt, PKT_C_MOVE); }

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