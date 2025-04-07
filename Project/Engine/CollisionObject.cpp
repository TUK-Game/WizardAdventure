#include "pch.h"
#include "CollisionObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "BoxCollider.h"

CCollisionObject::CCollisionObject()
{
}

CCollisionObject::~CCollisionObject()
{
}

void CCollisionObject::Init(CGameObject* owner)
{
	m_Owner = owner;
	this->SetName(owner->GetName() + L"CO");
	this->AddComponent(new CTransform);
	this->AddComponent(new CMeshRenderer);
	this->AddComponent(new CBoxCollider());
	this->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	this->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"DC"));
	this->GetTransform()->SetRelativeScale(owner->GetMeshRenderer()->GetMesh()->GetMeshSize());
	this->GetTransform()->SetRelativePosition(owner->GetTransform()->GetRelativePosition());
}

void CCollisionObject::InitToChild(CGameObject* owner, Vec3 position, Vec3 scale)
{
	this->SetName(owner->GetName() + L"CO");
	this->AddComponent(new CTransform);
	this->AddComponent(new CMeshRenderer);
	this->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	this->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"DC"));
	this->GetTransform()->SetRelativePosition(position);
	this->GetTransform()->SetRelativeScale(scale);
}

void CCollisionObject::FinalUpdate()
{
	CGameObject::FinalUpdate();
}
