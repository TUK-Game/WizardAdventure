#include "pch.h"
#include "MeshData.h"
#include "FBXLoader.h"
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

CMeshData* CMeshData::LoadFromFBX(const std::wstring& path)
{
	FBXLoader loader;
	loader.LoadFbx(path);

	CMeshData* meshData = new CMeshData;

	for (INT32 i = 0; i < loader.GetMeshCount(); i++)
	{
		CMesh* mesh = CAssetManager::GetInst()->FindAsset<CMesh>(loader.GetMesh(i).name);
		if (mesh == NULL)
		{
			mesh = CMesh::CreateFromFBX(&loader.GetMesh(i), loader);
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
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

CMeshData* CMeshData::LoadFromJHD(const std::wstring& path, const std::wstring& texturePath)
{
	CJHDLoader loader;
	loader.LoadFile(ws2s(path).c_str(), texturePath);

	CMeshData* meshData = new CMeshData;

	for (INT32 i = 0; i < loader.GetMeshCount(); i++)
	{
		CMesh* mesh = NULL; //= CAssetManager::GetInst()->FindAsset<CMesh>(loader.GetMesh(i).name);
		if (mesh == NULL)
		{
			mesh = CMesh::CreateFromJHD(&loader.GetMesh(i), loader);
			//CAssetManager::GetInst()->AddAsset(mesh->GetName(), mesh);
		}

		// Material 찾아서 연동
		std::vector<CMaterial*> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			JHDMeshInfo n = (loader.GetMesh(i));
			CMaterial* material = CAssetManager::GetInst()->FindAsset<CMaterial>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		info.matrix = GetMatrix(loader.GetMesh(i).matrix);
		info.translation = loader.GetMesh(i).translate;
		info.rotation = loader.GetMesh(i).rotation;
		info.scale = loader.GetMesh(i).scale;
		info.boundingBoxMax = loader.GetMesh(i).BoundingBoxMax;
		info.boundingBoxMin = loader.GetMesh(i).BoundingBoxMin;
		info.centerPos = loader.GetMesh(i).centerPos;
		
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

std::vector<CGameObject*> CMeshData::Instantiate()
{
	std::vector<CGameObject*> v;

	for (MeshRenderInfo& info : _meshRenders)
	{	
		CGameObject* gameObject = new CGameObject;
		gameObject->AddComponent(new CTransform);
		gameObject->AddComponent(new CMeshRenderer);
		info.mesh->SetMeshSize(Vec3(info.boundingBoxMax - info.boundingBoxMin));
		gameObject->GetMeshRenderer()->SetMesh(info.mesh);
		gameObject->AddComponent(new CBoxCollider);
		gameObject->GetCollider()->SetProfile(CCollisionManager::GetInst()->FindProfile("Wall"));
		gameObject->GetCollider()->SetMaxMinPos(info.centerPos, info.boundingBoxMax, info.boundingBoxMin);

		//gameObject->GetTransform()->SetWorldMatrix(info.matrix);
		//gameObject->GetTransform()->SetRelativeRotation(info.rotation.x, info.rotation.y, info.rotation.z);
		//gameObject->GetTransform()->SetRelativePosition(info.translation.x, info.translation.y, info.translation.z);
		

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

