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
};

// ===== Process Packet =====
bool Handle_INVALID(CPacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_C_LOGIN(CPacketSessionRef& session, Protocol::C_LOGIN& pkt);
bool Handle_C_ENTER_GAME(CPacketSessionRef& session, Protocol::C_ENTER_GAME& pkt);
bool Handle_C_ENTER_GAME_SUCCESS(CPacketSessionRef& session, Protocol::C_ENTER_GAME_SUCCESS& pkt);
bool Handle_C_LEAVE_GAME(CPacketSessionRef& session, Protocol::C_LEAVE_GAME& pkt);
bool Handle_C_SPAWN_PROJECTILE(CPacketSessionRef& session, Protocol::C_SPAWN_PROJECTILE& pkt);
bool Handle_C_MOVE(CPacketSessionRef& session, Protocol::C_MOVE& pkt);
bool Handle_C_MOVE_PROJECTILE(CPacketSessionRef& session, Protocol::C_MOVE_PROJECTILE& pkt);

class ServerPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			g_PacketHandler[i] = Handle_INVALID;
		g_PacketHandler[PKT_C_LOGIN] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_LOGIN>(Handle_C_LOGIN, session, buffer, len); };
		g_PacketHandler[PKT_C_ENTER_GAME] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ENTER_GAME>(Handle_C_ENTER_GAME, session, buffer, len); };
		g_PacketHandler[PKT_C_ENTER_GAME_SUCCESS] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ENTER_GAME_SUCCESS>(Handle_C_ENTER_GAME_SUCCESS, session, buffer, len); };
		g_PacketHandler[PKT_C_LEAVE_GAME] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_LEAVE_GAME>(Handle_C_LEAVE_GAME, session, buffer, len); };
		g_PacketHandler[PKT_C_SPAWN_PROJECTILE] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_SPAWN_PROJECTILE>(Handle_C_SPAWN_PROJECTILE, session, buffer, len); };
		g_PacketHandler[PKT_C_MOVE] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_MOVE>(Handle_C_MOVE, session, buffer, len); };
		g_PacketHandler[PKT_C_MOVE_PROJECTILE] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_MOVE_PROJECTILE>(Handle_C_MOVE_PROJECTILE, session, buffer, len); };
	}

	static bool HandlePacket(CPacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return g_PacketHandler[header->Id](session, buffer, len);
	}
	static CSendBufferRef MakeSendBuffer(Protocol::S_LOGIN& pkt) { return MakeSendBuffer(pkt, PKT_S_LOGIN); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_S_ENTER_GAME); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_SPAWN_PROJECTILE_SUCESSE& pkt) { return MakeSendBuffer(pkt, PKT_S_SPAWN_PROJECTILE_SUCESSE); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_LEAVE_GAME& pkt) { return MakeSendBuffer(pkt, PKT_S_LEAVE_GAME); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_MOVE& pkt) { return MakeSendBuffer(pkt, PKT_S_MOVE); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_UPDATE_PLAYER& pkt) { return MakeSendBuffer(pkt, PKT_S_UPDATE_PLAYER); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_MONSTER_INFO& pkt) { return MakeSendBuffer(pkt, PKT_S_MONSTER_INFO); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_PROJECTILE_INFO& pkt) { return MakeSendBuffer(pkt, PKT_S_PROJECTILE_INFO); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_SPAWN& pkt) { return MakeSendBuffer(pkt, PKT_S_SPAWN); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_SPAWN_NEW_PLAYER& pkt) { return MakeSendBuffer(pkt, PKT_S_SPAWN_NEW_PLAYER); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_SPAWN_EXISTING_PLAYER& pkt) { return MakeSendBuffer(pkt, PKT_S_SPAWN_EXISTING_PLAYER); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_DESPAWN_PLAYER& pkt) { return MakeSendBuffer(pkt, PKT_S_DESPAWN_PLAYER); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_DESPAWN& pkt) { return MakeSendBuffer(pkt, PKT_S_DESPAWN); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_GATE_OPNE& pkt) { return MakeSendBuffer(pkt, PKT_S_GATE_OPNE); }
	static CSendBufferRef MakeSendBuffer(Protocol::S_GATE_CLOSE& pkt) { return MakeSendBuffer(pkt, PKT_S_GATE_CLOSE); }

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