#pragma once

#include "Asset.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "GraphicShader.h"
#include "ComputeShader.h"

class CAssetManager
	: public CSingleton<CAssetManager>
{
	DECLARE_SINGLETON(CAssetManager)

public:
	template<typename T>
	EAsset_Type GetAssetType();

public:
	int Init();

public:
	void AddAsset(const std::wstring& key, CSharedPtr<CAsset> asset);

	template<typename T>
	CSharedPtr<T> FindAsset(const std::wstring& key);

	class CMeshData* LoadFBX(const std::wstring& path);

	CTexture* CreateTexture(const std::wstring& name, DXGI_FORMAT format, UINT32 width, UINT32 height,
		const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags,
		D3D12_RESOURCE_FLAGS resFlags = D3D12_RESOURCE_FLAG_NONE, Vec4 clearColor = Vec4());

	CTexture* CreateTextureFromResource(const std::wstring& name, ComPtr<ID3D12Resource> tex2D);
private:
	int LoadMesh();
	int LoadTexture();
	int LoadMaterial();
	int LoadMeshData();
	int LoadGraphicShader();
	int LoadComputeShader();
	int LoadShader(class CShader* shader, std::wstring& shaderName, ShaderInfo info = ShaderInfo()) const;

	int CreateCubeMesh();
	int CreateSphereMesh();

private:
	std::array<std::unordered_map<std::wstring, CSharedPtr<CAsset>>, (int)EAsset_Type::END>	m_mapAsset;
};

template<typename T>
EAsset_Type CAssetManager::GetAssetType()
{
	if constexpr (std::is_same_v<CMesh, T>)
		return EAsset_Type::Mesh;
	else if constexpr (std::is_same_v<CTexture, T>)
		return EAsset_Type::Texture;
	else if constexpr (std::is_same_v<CMaterial, T>)
		return EAsset_Type::Material;
	else if constexpr (std::is_same_v<CGraphicShader, T>)
		return EAsset_Type::GraphicShader;
	else if constexpr (std::is_same_v<CComputeShader, T>)	
		return EAsset_Type::ComputeShader;
	else if constexpr (std::is_same_v<CMeshData, T>)
		return EAsset_Type::FBX;
	return EAsset_Type::END;
}

template<typename T>
CSharedPtr<T> CAssetManager::FindAsset(const std::wstring& key)
{
	EAsset_Type type = GetAssetType<T>();

	auto iter = m_mapAsset[(int)type].find(key);

	if (iter == m_mapAsset[(int)type].end())
		return nullptr;

	return (T*)iter->second.Get();
}
