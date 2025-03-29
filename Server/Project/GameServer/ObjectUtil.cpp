#include "pch.h"
#include "ObjectUtil.h"
#include "Player.h"
#include "GameSession.h"

std::atomic<int64>	CObjectUtil::s_IdGenerator = 1;

CPlayerRef CObjectUtil::CreatePlayer(CGameSessionRef session)
{
	// ID ����
	const int64 newId = s_IdGenerator.fetch_add(1);

	CPlayerRef player = std::make_shared<CPlayer>();

	// object_id�� ��� ������Ʈ���� �߱޵Ǵ� ���̵�.
	// ��� �÷��̾����� �����ϱ� ���ؼ��� session_id�� �ʿ���. session Ŭ������ id�� ���� �� �� ����.
	// ���� ������� object_id �� player_id(session_id) �� �� ���� ����� �����Ͽ��� ��.
	//player->PlayerInfo->set_session_id(CSessionManager::GetInst()->GetId());	TODO
	player->ObjectInfo->set_object_id(newId);
	player->PosInfo->set_object_id(newId);

	player->m_Session = session;
	session->Player.store(player);

	return player;
}
