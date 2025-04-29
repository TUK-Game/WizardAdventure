#pragma once

#include "Asset.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "GraphicShader.h"
#include "ComputeShader.h"
#include "JHDLoader.h"

class CAssetManager
	: public CSingleton<CAssetManager>
{
	DECLARE_SINGLETON(CAssetManager)

public:
	template<typename T>
	EAsset_Type GetAssetType();

	template<typename T>
	std::vector<std::wstring> GetAllAssetNames();

public:
	int Init();
	void Update();

public:
	void AddAsset(const std::wstring& key, CSharedPtr<CAsset> asset);

	template<typename T>
	CSharedPtr<T> FindAsset(const std::wstring& key);

	class CMeshData* LoadJHD(const std::wstring& path, const std::wstring& texturePath = L"");
	class CMeshData* LoadJHDForAnotherColor(const std::wstring& path, const std::wstring& materialName, int idx);

	CTexture* CreateTexture(const std::wstring& name, DXGI_FORMAT format, UINT32 width, UINT32 height,
		const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags,
		D3D12_RESOURCE_FLAGS resFlags = D3D12_RESOURCE_FLAG_NONE, Vec4 clearColor = Vec4());

	CTexture* CreateTextureFromResource(const std::wstring& name, ComPtr<ID3D12Resource> tex2D);

public:	// ============================ Sound ================================
	bool CreateSoundChannel(const std::string& name);
	bool LoadSound(const std::string& groupName, const std::string& name, bool loop, const char* fileName, const std::wstring& pathName = SOUND_PATH);
	bool SetVolume(int volume);
	bool SetVolume(const std::string& groupName, int volume);
	bool SoundPlay(const std::string& name);
	bool SoundStop(const std::string& name);
	bool SoundPause(const std::string& name);
	bool SoundResume(const std::string& name);

	FMOD::ChannelGroup* FindChannelGroup(const std::string& name);
	class CSound* FindSound(const std::string& name);
	void ReleaseSound(const std::string& name);

private:
	int LoadMesh();
	int LoadTexture();
	int LoadMaterial();
	int LoadMeshData();
	int LoadParticle();
	int LoadGraphicShader();
	int LoadComputeShader();
	int LoadShader(class CShader* shader, std::wstring& shaderName, ShaderInfo info = ShaderInfo(), 
		const std::string& vs = "VS_Main", const std::string& ps = "PS_Main", const std::string& gs = "", const std::string& cs = "") const;

	int CreatePointMesh();
	int CreateCubeMesh();
	int CreateSphereMesh();
	int CreateRectangleMesh();
	int CreateCircleMesh();


private:
	std::array<std::unordered_map<std::wstring, CSharedPtr<CAsset>>, (int)EAsset_Type::END>	m_mapAsset;
	class CSoundManager* m_soundManager;

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
	else if constexpr (std::is_same_v<JHDMeshInfo, T>)
		return EAsset_Type::JHDMeshData;
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

template<typename T>
std::vector<std::wstring> CAssetManager::GetAllAssetNames()
{
	std::vector<std::wstring> assetNames;

	EAsset_Type type = GetAssetType<T>();
	auto& assetMap = m_mapAsset[(int)type];

	for (const auto& pair : assetMap)
	{
		assetNames.push_back(pair.first); 
	}

	return assetNames;
}