// Auto Create
#pragma once
#include "Protocol.pb.h"

using PacketHandlerFunc = std::function<bool(CPacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc g_PacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_C_LOGIN = 1000,
	PKT_S_LOGIN = 1001,
	PKT_C_ENTER_GAME = 1002,
	PKT_S_ENTER_GAME = 1003,
	PKT_C_LEAVE_GAME = 1004,
	PKT_S_LEAVE_GAME = 1005,
	PKT_C_MOVE = 1006,
	PKT_S_MOVE = 1007,
	PKT_S_MONSTER_INFO = 1008,
	PKT_S_SPAWN = 1009,
	PKT_S_SPAWN_NEW_PLAYER = 1010,
	PKT_S_SPAWN_EXISTING_PLAYER = 1011,
	PKT_S_DESPAWN_PLAYER = 1012,
	PKT_S_DESPAWN = 1013,
};

// ===== Process Packet =====
bool Handle_INVALID(CPacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_S_LOGIN(CPacketSessionRef& session, Protocol::S_LOGIN& pkt);
bool Handle_S_ENTER_GAME(CPacketSessionRef& session, Protocol::S_ENTER_GAME& pkt);
bool Handle_S_LEAVE_GAME(CPacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt);
bool Handle_S_MOVE(CPacketSessionRef& session, Protocol::S_MOVE& pkt);
bool Handle_S_MONSTER_INFO(CPacketSessionRef& session, Protocol::S_MONSTER_INFO& pkt);
bool Handle_S_SPAWN(CPacketSessionRef& session, Protocol::S_SPAWN& pkt);
bool Handle_S_SPAWN_NEW_PLAYER(CPacketSessionRef& session, Protocol::S_SPAWN_NEW_PLAYER& pkt);
bool Handle_S_SPAWN_EXISTING_PLAYER(CPacketSessionRef& session, Protocol::S_SPAWN_EXISTING_PLAYER& pkt);
bool Handle_S_DESPAWN_PLAYER(CPacketSessionRef& session, Protocol::S_DESPAWN_PLAYER& pkt);
bool Handle_S_DESPAWN(CPacketSessionRef& session, Protocol::S_DESPAWN& pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			g_PacketHandler[i] = Handle_INVALID;
		g_PacketHandler[PKT_S_LOGIN] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LOGIN>(Handle_S_LOGIN, session, buffer, len); };
		g_PacketHandler[PKT_S_ENTER_GAME] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ENTER_GAME>(Handle_S_ENTER_GAME, session, buffer, len); };
		g_PacketHandler[PKT_S_LEAVE_GAME] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LEAVE_GAME>(Handle_S_LEAVE_GAME, session, buffer, len); };
		g_PacketHandler[PKT_S_MOVE] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MOVE>(Handle_S_MOVE, session, buffer, len); };
		g_PacketHandler[PKT_S_MONSTER_INFO] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MONSTER_INFO>(Handle_S_MONSTER_INFO, session, buffer, len); };
		g_PacketHandler[PKT_S_SPAWN] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWN>(Handle_S_SPAWN, session, buffer, len); };
		g_PacketHandler[PKT_S_SPAWN_NEW_PLAYER] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWN_NEW_PLAYER>(Handle_S_SPAWN_NEW_PLAYER, session, buffer, len); };
		g_PacketHandler[PKT_S_SPAWN_EXISTING_PLAYER] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWN_EXISTING_PLAYER>(Handle_S_SPAWN_EXISTING_PLAYER, session, buffer, len); };
		g_PacketHandler[PKT_S_DESPAWN_PLAYER] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DESPAWN_PLAYER>(Handle_S_DESPAWN_PLAYER, session, buffer, len); };
		g_PacketHandler[PKT_S_DESPAWN] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DESPAWN>(Handle_S_DESPAWN, session, buffer, len); };
	}

	static bool HandlePacket(CPacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return g_PacketHandler[header->Id](session, buffer, len);
	}
	static CSendBufferRef MakeSendBuffer(Protocol::C_LOGIN& pkt) { return MakeSendBuffer(pkt, PKT_C_LOGIN); }
	static CSendBufferRef MakeSendBuffer(Protocol::C_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_ENTER_GAME); }
	static CSendBufferRef MakeSendBuffer(Protocol::C_LEAVE_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_LEAVE_GAME); }
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

	// ===== Create SendBuffer (Serialization) =====
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