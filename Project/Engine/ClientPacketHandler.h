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
	PKT_C_ENTER_GAME_SUCCESS = 1003,
	PKT_S_ENTER_GAME = 1004,
	PKT_C_LEAVE_GAME = 1005,
	PKT_C_SPAWN_PROJECTILE = 1006,
	PKT_S_SPAWN_PROJECTILE_SUCESSE = 1007,
	PKT_S_LEAVE_GAME = 1008,
	PKT_C_MOVE = 1009,
	PKT_C_MOVE_PROJECTILE = 1010,
	PKT_S_MOVE = 1011,
	PKT_S_UPDATE_PLAYER = 1012,
	PKT_S_MONSTER_INFO = 1013,
	PKT_S_PROJECTILE_INFO = 1014,
	PKT_S_SPAWN = 1015,
	PKT_S_SPAWN_NEW_PLAYER = 1016,
	PKT_S_SPAWN_EXISTING_PLAYER = 1017,
	PKT_S_DESPAWN_PLAYER = 1018,
	PKT_S_DESPAWN = 1019,
	PKT_S_GATE_OPNE = 1020,
	PKT_S_GATE_CLOSE = 1021,
	PKT_S_SPAWN_NPC = 1022,
	PKT_C_BUY_ITEM = 1023,
	PKT_C_BUY_SKILL = 1024,
	PKT_S_UPDATE_ITEM = 1025,
	PKT_S_BUY_ITEM = 1026,
	PKT_S_BUY_SKILL = 1027,
};

// ===== Process Packet =====
bool Handle_INVALID(CPacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_S_LOGIN(CPacketSessionRef& session, Protocol::S_LOGIN& pkt);
bool Handle_S_ENTER_GAME(CPacketSessionRef& session, Protocol::S_ENTER_GAME& pkt);
bool Handle_S_SPAWN_PROJECTILE_SUCESSE(CPacketSessionRef& session, Protocol::S_SPAWN_PROJECTILE_SUCESSE& pkt);
bool Handle_S_LEAVE_GAME(CPacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt);
bool Handle_S_MOVE(CPacketSessionRef& session, Protocol::S_MOVE& pkt);
bool Handle_S_UPDATE_PLAYER(CPacketSessionRef& session, Protocol::S_UPDATE_PLAYER& pkt);
bool Handle_S_MONSTER_INFO(CPacketSessionRef& session, Protocol::S_MONSTER_INFO& pkt);
bool Handle_S_PROJECTILE_INFO(CPacketSessionRef& session, Protocol::S_PROJECTILE_INFO& pkt);
bool Handle_S_SPAWN(CPacketSessionRef& session, Protocol::S_SPAWN& pkt);
bool Handle_S_SPAWN_NEW_PLAYER(CPacketSessionRef& session, Protocol::S_SPAWN_NEW_PLAYER& pkt);
bool Handle_S_SPAWN_EXISTING_PLAYER(CPacketSessionRef& session, Protocol::S_SPAWN_EXISTING_PLAYER& pkt);
bool Handle_S_DESPAWN_PLAYER(CPacketSessionRef& session, Protocol::S_DESPAWN_PLAYER& pkt);
bool Handle_S_DESPAWN(CPacketSessionRef& session, Protocol::S_DESPAWN& pkt);
bool Handle_S_GATE_OPNE(CPacketSessionRef& session, Protocol::S_GATE_OPNE& pkt);
bool Handle_S_GATE_CLOSE(CPacketSessionRef& session, Protocol::S_GATE_CLOSE& pkt);
bool Handle_S_SPAWN_NPC(CPacketSessionRef& session, Protocol::S_SPAWN_NPC& pkt);
bool Handle_S_UPDATE_ITEM(CPacketSessionRef& session, Protocol::S_UPDATE_ITEM& pkt);
bool Handle_S_BUY_ITEM(CPacketSessionRef& session, Protocol::S_BUY_ITEM& pkt);
bool Handle_S_BUY_SKILL(CPacketSessionRef& session, Protocol::S_BUY_SKILL& pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			g_PacketHandler[i] = Handle_INVALID;
		g_PacketHandler[PKT_S_LOGIN] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LOGIN>(Handle_S_LOGIN, session, buffer, len); };
		g_PacketHandler[PKT_S_ENTER_GAME] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ENTER_GAME>(Handle_S_ENTER_GAME, session, buffer, len); };
		g_PacketHandler[PKT_S_SPAWN_PROJECTILE_SUCESSE] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWN_PROJECTILE_SUCESSE>(Handle_S_SPAWN_PROJECTILE_SUCESSE, session, buffer, len); };
		g_PacketHandler[PKT_S_LEAVE_GAME] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LEAVE_GAME>(Handle_S_LEAVE_GAME, session, buffer, len); };
		g_PacketHandler[PKT_S_MOVE] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MOVE>(Handle_S_MOVE, session, buffer, len); };
		g_PacketHandler[PKT_S_UPDATE_PLAYER] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_UPDATE_PLAYER>(Handle_S_UPDATE_PLAYER, session, buffer, len); };
		g_PacketHandler[PKT_S_MONSTER_INFO] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MONSTER_INFO>(Handle_S_MONSTER_INFO, session, buffer, len); };
		g_PacketHandler[PKT_S_PROJECTILE_INFO] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PROJECTILE_INFO>(Handle_S_PROJECTILE_INFO, session, buffer, len); };
		g_PacketHandler[PKT_S_SPAWN] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWN>(Handle_S_SPAWN, session, buffer, len); };
		g_PacketHandler[PKT_S_SPAWN_NEW_PLAYER] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWN_NEW_PLAYER>(Handle_S_SPAWN_NEW_PLAYER, session, buffer, len); };
		g_PacketHandler[PKT_S_SPAWN_EXISTING_PLAYER] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWN_EXISTING_PLAYER>(Handle_S_SPAWN_EXISTING_PLAYER, session, buffer, len); };
		g_PacketHandler[PKT_S_DESPAWN_PLAYER] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DESPAWN_PLAYER>(Handle_S_DESPAWN_PLAYER, session, buffer, len); };
		g_PacketHandler[PKT_S_DESPAWN] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DESPAWN>(Handle_S_DESPAWN, session, buffer, len); };
		g_PacketHandler[PKT_S_GATE_OPNE] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_GATE_OPNE>(Handle_S_GATE_OPNE, session, buffer, len); };
		g_PacketHandler[PKT_S_GATE_CLOSE] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_GATE_CLOSE>(Handle_S_GATE_CLOSE, session, buffer, len); };
		g_PacketHandler[PKT_S_SPAWN_NPC] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWN_NPC>(Handle_S_SPAWN_NPC, session, buffer, len); };
		g_PacketHandler[PKT_S_UPDATE_ITEM] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_UPDATE_ITEM>(Handle_S_UPDATE_ITEM, session, buffer, len); };
		g_PacketHandler[PKT_S_BUY_ITEM] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_BUY_ITEM>(Handle_S_BUY_ITEM, session, buffer, len); };
		g_PacketHandler[PKT_S_BUY_SKILL] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_BUY_SKILL>(Handle_S_BUY_SKILL, session, buffer, len); };
	}

	static bool HandlePacket(CPacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return g_PacketHandler[header->Id](session, buffer, len);
	}
	static CSendBufferRef MakeSendBuffer(Protocol::C_LOGIN& pkt) { return MakeSendBuffer(pkt, PKT_C_LOGIN); }
	static CSendBufferRef MakeSendBuffer(Protocol::C_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_ENTER_GAME); }
	static CSendBufferRef MakeSendBuffer(Protocol::C_ENTER_GAME_SUCCESS& pkt) { return MakeSendBuffer(pkt, PKT_C_ENTER_GAME_SUCCESS); }
	static CSendBufferRef MakeSendBuffer(Protocol::C_LEAVE_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_LEAVE_GAME); }
	static CSendBufferRef MakeSendBuffer(Protocol::C_SPAWN_PROJECTILE& pkt) { return MakeSendBuffer(pkt, PKT_C_SPAWN_PROJECTILE); }
	static CSendBufferRef MakeSendBuffer(Protocol::C_MOVE& pkt) { return MakeSendBuffer(pkt, PKT_C_MOVE); }
	static CSendBufferRef MakeSendBuffer(Protocol::C_MOVE_PROJECTILE& pkt) { return MakeSendBuffer(pkt, PKT_C_MOVE_PROJECTILE); }
	static CSendBufferRef MakeSendBuffer(Protocol::C_BUY_ITEM& pkt) { return MakeSendBuffer(pkt, PKT_C_BUY_ITEM); }
	static CSendBufferRef MakeSendBuffer(Protocol::C_BUY_SKILL& pkt) { return MakeSendBuffer(pkt, PKT_C_BUY_SKILL); }

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