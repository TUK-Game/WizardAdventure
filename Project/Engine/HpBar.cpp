#include "pch.h"
#include "HpBar.h"
#include "NetworkManager.h"
#include "ServerSession.h"
#include "Player.h"

void CHpBar::Update()
{
	if (CNetworkManager::s_GameSession)
	{
		CPlayer* player = CNetworkManager::s_GameSession->GetOwnPlayer();
		if (player)
		{
			Stats* stat = player->GetStats();
			if(stat)
			{
				SetGauge(stat->currentHp);
				CGaugeWidget::Update();
			}
		}
	}
}
