#include "pch.h"
#include "MeshData.h"
#include "Mesh.h"
#include "Material.h"
#include "AssetManager.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Animator.h"
#include "JHDLoader.h"
#include "BaseCollider.h"
#include "BoxCollider.h"
#include "CollisionManager.h"


CMeshData::CMeshData()
	: CAsset(EAsset_Type::FBX)
{
}

CMeshData::~CMeshData()
{
}

CMeshData* CMeshData::LoadFromJHD(const std::wstring& path, const std::wstring& texturePath)
{
	CJHDLoader loader;
	loader.LoadFile(ws2s(path).c_str(), texturePath);

	CMeshData* meshData = new CMeshData;

	for (INT32 i = 0; i < loader.GetMeshCount(); i++)
	{
		CMesh* mesh = CAssetManager::GetInst()->FindAsset<CMesh>(loader.GetMesh(i).name);
		if (mesh == NULL)
		{
			mesh = CMesh::CreateFromJHD(&loader.GetMesh(i), loader, i);
			mesh->m_indexInfo = loader.GetMesh(i).indices[0];
			mesh->m_vertexInfo = loader.GetMesh(i).vertices;
			CAssetManager::GetInst()->AddAsset(mesh->GetName(), mesh);
		}

		// Material 찾아서 연동
		std::vector<CMaterial*> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			CMaterial* material = CAssetManager::GetInst()->FindAsset<CMaterial>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		info.matrix = GetMatrix(loader.GetMesh(i).matrix);
		info.boundingBoxMax = loader.GetMesh(i).BoundingBoxMax;
		info.boundingBoxMin = loader.GetMesh(i).BoundingBoxMin;
		info.centerPos = loader.GetMesh(i).centerPos;
		info.translation = loader.GetMesh(i).translate;
		info.rotation = loader.GetMesh(i).rotation;
		info.scale = loader.GetMesh(i).scale;
			
		meshData->_meshRenders.push_back(info);
	}
	 
	return meshData;
}

void CMeshData::Load(const std::wstring& _strFilePath)
{
	// TODO
}

void CMeshData::Save(const std::wstring& _strFilePath)
{
	// TODO
}

std::vector<CGameObject*> CMeshData::Instantiate(ECollision_Channel channel)
{
	std::vector<CGameObject*> v;

	for (MeshRenderInfo& info : _meshRenders)
	{
		CGameObject* gameObject = new CGameObject;
		gameObject->AddComponent(new CTransform);
		gameObject->GetTransform()->SetRelativePosition(info.translation.x, info.translation.y, -info.translation.z);
		gameObject->GetTransform()->SetRelativeRotation(-info.rotation.x -90.f, -info.rotation.y, info.rotation.z);
		gameObject->GetTransform()->SetRelativeScale(info.scale.x, info.scale.y, info.scale.z);
		gameObject->GetTransform()->FinalUpdate();

		gameObject->AddComponent(new CMeshRenderer);
		info.mesh->SetMeshSize(Vec3(info.boundingBoxMax - info.boundingBoxMin));
		gameObject->GetMeshRenderer()->SetMesh(info.mesh);

		float posy = gameObject->GetTransform()->GetWorldPosition().y;
		//if (gameObject->GetTransform()->GetWorldPosition().y >= -20.f)
		{
			if (channel == ECollision_Channel::Wall)
			{
				gameObject->AddComponent(new CBoxCollider);
				gameObject->GetCollider()->SetProfile(CCollisionManager::GetInst()->FindProfile("Wall"));
				//else if (channel == ECollision_Channel::Player)
				//	gameObject->GetCollider()->SetProfile(CCollisionManager::GetInst()->FindProfile("Player"));
				gameObject->GetCollider()->SetMaxMinPos(info.centerPos, info.boundingBoxMax, info.boundingBoxMin);
			}
		}

		for (UINT32 i = 0; i < info.materials.size(); i++)
			gameObject->GetMeshRenderer()->SetMaterial(info.materials[i], i);

		if (info.mesh->IsAnimMesh())
		{
			CAnimator* animator = new CAnimator;
			gameObject->AddComponent(animator);
			animator->SetBones(info.mesh->GetBones());
			animator->SetAnimClip(info.mesh->GetAnimClip());
		}


		v.push_back(gameObject);
	}


	return v;
}

