#include "pch.h"
#include "PlayerScript.h"
#include "Player.h"
#include "StateManager.h"
#include "InputManager.h"
#include "Animator.h"
#include "Level.h"
#include "LevelManager.h"
#include "RenderManager.h"
#include "Camera.h"
#include "CameraScript.h"
#include "NetworkManager.h"
#include "ServerSession.h"

void CPlayerScript::Update()
{
	CPlayer* player = (CPlayer*)GetOwner();

	CStateManager* stateManager = player->GetStateManager();
	if (!stateManager) return;

	if (stateManager->GetCurrentStateType() == EState_Type::Death)
		return;

	if (player->GetStats()->currentHp <= 0)
	{
		//stateManager->HandleEvent(player, "Death");
		if (stateManager->GetCurrentStateType() != EState_Type::Death && m_BeforDeath_TargetId < 0)
		{
			const auto& camera = CRenderManager::GetInst()->GetMainCamera();
			const auto& script = dynamic_cast<CCameraScript*>(camera->GetOwner()->GetScript());
			if (script)
			{
				for (int i = 0; i < MAX_PLAYERS; ++i)
				{
					const auto& p = CLevelManager::GetInst()->GetPlayer(i);
					if (!p)
						continue;

					if (CNetworkManager::GetInst()->s_GameSession->GetClientID() != i)
					{
						script->SetTransform(p->GetTransform());
						camera->SetTarget(p);
						m_BeforDeath_TargetId = i;
						return;
					}
				}
				std::cout << "½ÇÆÐ\n";
			}
		}
		return;
	}

	ECamera_Type type = CRenderManager::GetInst()->GetMainCamera()->GetCameraType();

	CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();
	if (level && type == ECamera_Type::Interaction)
	{
		if (KEY_DOWN(EKey::G))
		{
			if (level->GetWidgetWindowType() == EWIDGETWINDOW_TYPE::STORE_WINDOW)
			{
				const auto window = level->FindWidgetWindow(EWIDGETWINDOW_TYPE::STORE_WINDOW);
				if (window->GetEnable())
				{
					window->SetEnable(false);
					player->FinishInteraction(true);
					level->SetWidgetWindowType(EWIDGETWINDOW_TYPE::END);
				}
			}
		}
		if (KEY_DOWN(EKey::Tab))
		{
			if (level->GetWidgetWindowType() == EWIDGETWINDOW_TYPE::INVENTORY_WINDOW)
			{
				const auto window = level->FindWidgetWindow(EWIDGETWINDOW_TYPE::INVENTORY_WINDOW);
				if (window->GetEnable())
				{
					window->SetEnable(false);
					player->FinishInteraction(false);
					level->SetWidgetWindowType(EWIDGETWINDOW_TYPE::END);
				}
			}
		}
		return;
	}

	if (type != ECamera_Type::Fixed)
		return;

	Vec3 moveDir(0, 0, 0);
	if (KEY_PUSH(EKey::W))    moveDir.z += 1;
	if (KEY_PUSH(EKey::S))  moveDir.z -= 1;
	if (KEY_PUSH(EKey::A))  moveDir.x -= 1;
	if (KEY_PUSH(EKey::D)) moveDir.x += 1;


	if (moveDir.Length() > 0.01f)
	{
		stateManager->HandleEvent(player, "Move");
	}
	if (KEY_DOWN(EKey::Space))
	{
		stateManager->HandleEvent(player, "Dash");
	}
	if (KEY_DOWN(EKey::LButton))
	{
		stateManager->HandleEvent(player, "Attack_LButton");
	}
	if (KEY_DOWN(EKey::RButton))
	{
		stateManager->HandleEvent(player, "Attack_RButton");
	}
	if (KEY_DOWN(EKey::Q))
	{
		stateManager->HandleEvent(player, "Attack_Q");
	}
	if (KEY_DOWN(EKey::E))
	{
		stateManager->HandleEvent(player, "Attack_E");
	}
	if (KEY_DOWN(EKey::R))
	{
		stateManager->HandleEvent(player, "Attack_R");
	}
	if (KEY_DOWN(EKey::F))
	{
		CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();
		if (level->GetWidgetWindowType() == EWIDGETWINDOW_TYPE::END)
		{
			if (player->DetectNPC())
			{
				level->SetWidgetWindowType(EWIDGETWINDOW_TYPE::STORE_WINDOW);
			}
		}
	}
	if (KEY_DOWN(EKey::M))
	{
		CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();
		if (level->GetWidgetWindowType() == EWIDGETWINDOW_TYPE::END || level->GetWidgetWindowType() == EWIDGETWINDOW_TYPE::MAP_WINDOW)
		{
			const auto window = level->FindWidgetWindow(EWIDGETWINDOW_TYPE::MAP_WINDOW);
			if (window->GetEnable())
			{
				window->SetEnable(false);
				level->SetWidgetWindowType(EWIDGETWINDOW_TYPE::END);
			}
			else
			{
				window->SetEnable(true);
				level->SetWidgetWindowType(EWIDGETWINDOW_TYPE::MAP_WINDOW);
			}
		}
	}
	if (KEY_DOWN(EKey::Tab))
	{
		CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();
		if (level->GetWidgetWindowType() == EWIDGETWINDOW_TYPE::END)
		{
			const auto window = level->FindWidgetWindow(EWIDGETWINDOW_TYPE::INVENTORY_WINDOW);
			if (!window->GetEnable())
			{
				player->MoveToInventoryView();
				level->SetWidgetWindowType(EWIDGETWINDOW_TYPE::INVENTORY_WINDOW);
			}
		}
	}

	// temp ----------------------------------------------------------------------------
	if (KEY_DOWN(EKey::Z))
	{
		stateManager->HandleEvent(player, "Knockback");
	}
	if (KEY_DOWN(EKey::X))
	{
		stateManager->HandleEvent(player, "Death");
	}
	// temp ----------------------------------------------------------------------------
}