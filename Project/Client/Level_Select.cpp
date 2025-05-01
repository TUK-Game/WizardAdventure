#include "pch.h"
#include "Level_Select.h"
#include <Engine/AssetManager.h>
#include <Engine/Transform.h>
#include <Engine/Camera.h>
#include <Engine/MeshRenderer.h>
#include "SelectLevelWidgetWindow.h"

CLevel_Select::CLevel_Select()
{
}

CLevel_Select::~CLevel_Select()
{
}

void CLevel_Select::Init()
{
	CLevel::Init();

	CAssetManager::GetInst()->LoadSound("BGM", "Play", false, "e.mp3");
	CAssetManager::GetInst()->SetVolume("BGM", 30);
	CAssetManager::GetInst()->SoundPlay("Play");

	CAssetManager::GetInst()->LoadSound("UI", "Click", false, "click.mp3");
	CAssetManager::GetInst()->SetVolume("UI", 30);

	CGameObject* c = new CGameObject();
	c->SetName(L"Orthographic_Camera");
	c->AddComponent(new CTransform);
	c->AddComponent(new CCamera); // Near=1, Far=1000, 800*600
	c->GetCamera()->SetProjType(EProjection_Type::Orthographic);
	c->GetCamera()->SetPriority(1); 
	c->GetCamera()->CheckLayerClear(); 
	c->GetCamera()->CheckLayer(4); 
	c->GetTransform()->SetRelativePosition(Vec3(0.f, 0.f, 0.f));
	this->AddGameObject(c, LAYER_CAMERA, false);

	CGameObject* camera = new CGameObject;
	camera->SetName(L"MainCamera");
	camera->AddComponent(new CTransform);
	camera->AddComponent(new CCamera);
	camera->GetCamera()->SetProjType(EProjection_Type::Perspective);
	camera->GetCamera()->SetPriority(0); // 0 : main camera
	camera->GetCamera()->CheckLayerAll();
	camera->GetCamera()->CheckLayer(4);
	camera->GetTransform()->SetRelativePosition(0.f, 0.f, 0.f);
	this->AddGameObject(camera, LAYER_CAMERA, false);

	CGameObject* ui = new CGameObject;
	ui->SetName(L"SelectBackground");
	ui->AddComponent(new CTransform);
	ui->AddComponent(new CMeshRenderer);
	ui->GetTransform()->SetRelativeScale(Vec3(2.f, 2.f, 1.f));
	ui->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle"));
	CMaterial* material = new CMaterial;
	CTexture* texture;
	texture = CAssetManager::GetInst()->FindAsset<CTexture>(L"SelectBackground");
	CGraphicShader* shader = CAssetManager::GetInst()->FindAsset<CGraphicShader>(L"Texture");
	material->SetTexture(0, texture);
	material->SetGraphicsShader(shader);
	ui->GetMeshRenderer()->SetMaterial(material);
	this->AddGameObject(ui, LAYER_UI, false);

	CreateWidgetWindow<CSelectLevelWidgetWindow>(EWIDGETWINDOW_TYPE::START_WINDOW, L"SelectWindow", nullptr);
}

void CLevel_Select::Begin()
{
	CLevel::Begin();
}

void CLevel_Select::Update()
{
	CLevel::Update();
}

void CLevel_Select::FinalUpdate()
{
	CLevel::FinalUpdate();
}

void CLevel_Select::End()
{
	CLevel::End();
	CAssetManager::GetInst()->SoundStop("Play");
}

