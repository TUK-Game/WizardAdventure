#include "pch.h"
#include "MeshData.h"
#include "FBXLoader.h"
#include "Mesh.h"
#include "Material.h"
#include "AssetManager.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Animator.h"

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
		gameObject->GetMeshRenderer()->SetMesh(info.mesh);
		gameObject->GetTransform()->SetWorldMatrix(info.matrix);

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

