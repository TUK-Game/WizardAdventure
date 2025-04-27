#include "pch.h"
#include "HpBar.h"
#include "NetworkManager.h"
#include "ServerSession.h"
#include "Player.h"

void CHpBar::Update()
{
	if (m_OwnerPlayer)
	{
		SetGauge(m_OwnerPlayer->GetStats()->currentHp);
		CGaugeWidget::Update();
	}
}
