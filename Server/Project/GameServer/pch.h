#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\ServerCore_d.lib")
#pragma comment(lib, "Protobuf\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\libprotobuf.lib")
#endif

#include "CorePch.h"

#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"
#include "ServerPacketHandler.h"
#include "Util.h"
#include "GameSession.h"
#include "Timer.h"
// Collision
#include "DirectXCollision.h"
#include "DirectXMath.h"

using namespace DirectX;

enum class ECollision_Channel
{
	Player,
	Wall,
	Max
};

enum class ECollision_Interaction
{
	Ignore,
	Collision
};


struct CollisionProfile
{
	std::string							name;
	ECollision_Channel					channel;
	bool								enable = true;
	std::vector<ECollision_Interaction>	vecCollisionInteraction;
};


#define MAX_PLAYERS 4

USING_SHARED_PTR(CGameSession);
USING_SHARED_PTR(CPlayer);
USING_SHARED_PTR(CMonster);
USING_SHARED_PTR(CCreature);
USING_SHARED_PTR(CGameObject);
USING_SHARED_PTR(CRoom);
USING_SHARED_PTR(CCollisionManager);

#define SEND_PACKET(pkt)													\
	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);	\
	session->Send(sendBuffer);
