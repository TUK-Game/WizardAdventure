#pragma once
#include "Protocol.pb.h"
#include "PacketFlag.h"

extern std::function<bool(CPacketSessionRef&, BYTE*, int32)> g_{{parser.name}}PacketHandler[UINT16_MAX];

// Custom Handlers
bool Handle_{{parser.recv_prefix}}INVALID(CPacketSessionRef& session, BYTE* buffer, int32 len);

{%- for pkt in parser.recv_pkt %}
bool Handle_{{pkt.name}}(CPacketSessionRef& session, Protocol::{{pkt.name}}& pkt);
{%- endfor %}

class {{output}}
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			g_{{parser.name}}PacketHandler[i] = Handle_{{parser.recv_prefix}}INVALID;

{%- for pkt in parser.recv_pkt %}
		g_{{parser.name}}PacketHandler[PKT_{{pkt.name}}] = [](CPacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::{{pkt.name}}>(Handle_{{pkt.name}}, session, buffer, len); };
{%- endfor %}
	}

	static bool HandlePacket(CPacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return g_{{parser.name}}PacketHandler[header->Id](session, buffer, len);
	}

{%- for pkt in parser.send_pkt %}
	static CSendBufferRef MakeSendBuffer(Protocol::{{pkt.name}}& pkt) { return MakeSendBuffer(pkt, PKT_{{pkt.name}}); }
{%- endfor %}

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
