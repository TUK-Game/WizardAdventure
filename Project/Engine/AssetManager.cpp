#include "pch.h"
#include "AssetManager.h"
#include "PathManager.h"
#include "GraphicShader.h"
#include "ComputeShader.h"
#include "Texture.h"
#include "Material.h"
#include "MeshData.h"

CAssetManager::CAssetManager()
{
}

CAssetManager::~CAssetManager()
{
}

int CAssetManager::Init()
{
	if (FAILED(LoadMesh()))
		return E_FAIL;

	if (FAILED(LoadTexture()))
		return E_FAIL;

	if (FAILED(LoadGraphicShader()))
		return E_FAIL;

	if (FAILED(LoadMaterial()))
		return E_FAIL;

	if (FAILED(LoadComputeShader()))
		return E_FAIL;

	if (FAILED(LoadMeshData()))
		return E_FAIL;

	return S_OK;
}

void CAssetManager::AddAsset(const std::wstring& key, CSharedPtr<CAsset> asset)
{
	EAsset_Type type = asset->GetType();

	auto iter = m_mapAsset[(int)type].find(key);

	if(iter != m_mapAsset[(int)type].end())
		return;

	asset->m_Key = key;
	m_mapAsset[(int)type].insert(std::make_pair(key, asset));
}

int CAssetManager::LoadMesh()
{
	if (FAILED(CreateCubeMesh()))
		return E_FAIL;

	if (FAILED(CreateSphereMesh()))
		return E_FAIL;

	return S_OK;
}

int CAssetManager::LoadTexture()
{
	// ===================
	// 텍스쳐 한장 로딩하기
	// ===================
	CTexture* tex = new CTexture;
	auto path = CPathManager::GetInst()->FindPath(TEXTURE_PATH);
	tex->Init(path / L"kita.jpg");
	AddAsset(L"Kita", tex);

	tex = new CTexture;
	tex->Init(path / L"mushroom.png");
	AddAsset(L"Mushroom", tex);

	tex = new CTexture;
	tex->Init(path / L"Ryo.jpg");
	AddAsset(L"Ryo", tex);

	tex = new CTexture;
	tex->Init(path / L"Nigika.jpg");
	AddAsset(L"Nigika", tex);

	tex = new CTexture;
	tex->Init(path / L"Hitori.jpg");
	AddAsset(L"Hitori", tex);

	tex = new CTexture;
	tex->Init(path / L"Skybox03.dds", RESOURCE_TEXTURE_CUBE);
	AddAsset(L"Skybox", tex);
	return S_OK;
}

int CAssetManager::LoadMaterial()
{
	CMaterial* material = new CMaterial;
	material->SetGraphicsShader(FindAsset<CGraphicShader>(L"Deferred"));
	material->SetTexture(0, FindAsset<CTexture>(L"Kita"));
	AddAsset(L"Kita", material);

	material = new CMaterial;
	material->SetGraphicsShader(FindAsset<CGraphicShader>(L"Default"));
	material->SetTexture(0, FindAsset<CTexture>(L"Mushroom"));
	AddAsset(L"Mushroom", material);

	material = new CMaterial;
	material->SetGraphicsShader(FindAsset<CGraphicShader>(L"Deferred"));
	material->SetTexture(0, FindAsset<CTexture>(L"Ryo"));
	AddAsset(L"Ryo", material);

	material = new CMaterial;
	material->SetGraphicsShader(FindAsset<CGraphicShader>(L"Deferred"));
	material->SetTexture(0, FindAsset<CTexture>(L"Nigika"));
	AddAsset(L"Nigika", material);

	material = new CMaterial;
	material->SetGraphicsShader(FindAsset<CGraphicShader>(L"Deferred"));
	material->SetTexture(0, FindAsset<CTexture>(L"Hitori"));
	AddAsset(L"Hitori", material);

	material = new CMaterial;
	material->SetGraphicsShader(FindAsset<CGraphicShader>(L"Skybox"));
	material->SetTexture(0, FindAsset<CTexture>(L"Skybox"));
	AddAsset(L"Skybox", material);

	return S_OK;
}

int CAssetManager::LoadMeshData()
{
	//CMeshData* data = CAssetManager::GetInst()->LoadFBX(L"../../Content/Texture/FBX/Dragon.fbx");
	//AddAsset(L"Dragon", data);

	//data = CAssetManager::GetInst()->LoadFBX(L"../../Content/Texture/FBX/wolf.fbx");
	//AddAsset(L"Wolf", data);

	//data = CAssetManager::GetInst()->LoadFBX(L"../../Content/Texture/FBX/floor_world.fbx");
	//AddAsset(L"Floor", data);
	return S_OK;
}

int CAssetManager::LoadGraphicShader()
{
	CGraphicShader* shader = new CGraphicShader;
	std::wstring name = L"Forward.hlsl";
	LoadShader(shader, name);
	AddAsset(L"Forward", shader);


	shader = new CGraphicShader;
	name = L"Skybox.hlsl";
	LoadShader(shader, name, { SHADER_TYPE::FORWARD, RASTERIZER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::LESS_EQUAL });
	AddAsset(L"Skybox", shader);

	shader = new CGraphicShader;
	name = L"Deferred.hlsl";
	LoadShader(shader, name, { SHADER_TYPE::DEFERRED });
	AddAsset(L"Deferred", shader);
	return S_OK;
}

int CAssetManager::LoadComputeShader()
{
	CComputeShader* shader = new CComputeShader;
	std::wstring name = L"Compute.hlsl";
	LoadShader(shader, name);
	AddAsset(L"Compute", shader);


	return S_OK;
}

int CAssetManager::LoadShader(CShader* shader, std::wstring& shaderName, ShaderInfo info) const
{
	auto path = CPathManager::GetInst()->FindPath(HLSL_PATH);

	path /= shaderName;

	if (FAILED(shader->Init(path, ws2s(shaderName), info)))
		return E_FAIL;

	return S_OK;
}

CMeshData* CAssetManager::LoadFBX(const std::wstring& path)
{
	std::wstring key = path;

	CMeshData* meshData = FindAsset<CMeshData>(key);
	if (meshData)
		return meshData;

	meshData = CMeshData::LoadFromFBX(path);
	meshData->SetName(key);
	AddAsset(key, meshData);

	return meshData;
}

int CAssetManager::CreateCubeMesh()
{
	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	std::vector<Vertex> vecVertex(24);

	// 앞면
	vecVertex[0] = Vertex(Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vecVertex[1] = Vertex(Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vecVertex[2] = Vertex(Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vecVertex[3] = Vertex(Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	// 뒷면
	vecVertex[4] = Vertex(Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vecVertex[5] = Vertex(Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vecVertex[6] = Vertex(Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vecVertex[7] = Vertex(Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// 윗면
	vecVertex[8] = Vertex(Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vecVertex[9] = Vertex(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vecVertex[10] = Vertex(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vecVertex[11] = Vertex(Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	// 아랫면
	vecVertex[12] = Vertex(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vecVertex[13] = Vertex(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vecVertex[14] = Vertex(Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vecVertex[15] = Vertex(Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// 왼쪽면
	vecVertex[16] = Vertex(Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vecVertex[17] = Vertex(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vecVertex[18] = Vertex(Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vecVertex[19] = Vertex(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	// 오른쪽면
	vecVertex[20] = Vertex(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vecVertex[21] = Vertex(Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vecVertex[22] = Vertex(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vecVertex[23] = Vertex(Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));

	std::vector<UINT> vecIndex(36);

	// 앞면
	vecIndex[0] = 0; vecIndex[1] = 1; vecIndex[2] = 2;
	vecIndex[3] = 0; vecIndex[4] = 2; vecIndex[5] = 3;
	// 뒷면
	vecIndex[6] = 4; vecIndex[7] = 5; vecIndex[8] = 6;
	vecIndex[9] = 4; vecIndex[10] = 6; vecIndex[11] = 7;
	// 윗면
	vecIndex[12] = 8; vecIndex[13] = 9; vecIndex[14] = 10;
	vecIndex[15] = 8; vecIndex[16] = 10; vecIndex[17] = 11;
	// 아랫면
	vecIndex[18] = 12; vecIndex[19] = 13; vecIndex[20] = 14;
	vecIndex[21] = 12; vecIndex[22] = 14; vecIndex[23] = 15;
	// 왼쪽면
	vecIndex[24] = 16; vecIndex[25] = 17; vecIndex[26] = 18;
	vecIndex[27] = 16; vecIndex[28] = 18; vecIndex[29] = 19;
	// 오른쪽면
	vecIndex[30] = 20; vecIndex[31] = 21; vecIndex[32] = 22;
	vecIndex[33] = 20; vecIndex[34] = 22; vecIndex[35] = 23;

	CMesh* mesh = new CMesh;

	if (FAILED(mesh->Init(vecVertex, vecIndex)))
		return E_FAIL;

	mesh->SetMeshSize(Vec3(w2, h2, d2));

	AddAsset(L"Cube", mesh);

	return S_OK;
}

int CAssetManager::CreateSphereMesh()
{
	float radius = 0.5f; // 구의 반지름
	UINT stackCount = 20; // 가로 분할 
	UINT sliceCount = 20; // 세로 분할

	std::vector<Vertex> vecVertex;

	Vertex v;

	// 북극
	v.Pos = Vec3(0.0f, radius, 0.0f);
	v.UV = Vec2(0.5f, 0.0f);
	v.Normal = v.Pos;
	v.Normal.Normalize();
	v.Tangent = Vec3(1.0f, 0.0f, 1.0f);
	vecVertex.push_back(v);

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
	for (UINT y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// 고리에 위치한 정점
		for (UINT x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			v.Pos.x = radius * sinf(phi) * cosf(theta);
			v.Pos.y = radius * cosf(phi);
			v.Pos.z = radius * sinf(phi) * sinf(theta);

			v.UV = Vec2(deltaU * x, deltaV * y);

			v.Normal = v.Pos;
			v.Normal.Normalize();

			v.Tangent.x = -radius * sinf(phi) * sinf(theta);
			v.Tangent.y = 0.0f;
			v.Tangent.z = radius * sinf(phi) * cosf(theta);
			v.Tangent.Normalize();

			vecVertex.push_back(v);
		}
	}

	// 남극
	v.Pos = Vec3(0.0f, -radius, 0.0f);
	v.UV = Vec2(0.5f, 1.0f);
	v.Normal = v.Pos;
	v.Normal.Normalize();
	v.Tangent = Vec3(1.0f, 0.0f, 0.0f);
	vecVertex.push_back(v);

	std::vector<UINT> vecIndex(36);

	// 북극 인덱스
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		vecIndex.push_back(0);
		vecIndex.push_back(i + 2);
		vecIndex.push_back(i + 1);
	}

	// 몸통 인덱스
	UINT ringVertexCount = sliceCount + 1;
	for (UINT y = 0; y < stackCount - 2; ++y)
	{
		for (UINT x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			vecIndex.push_back(1 + (y)*ringVertexCount + (x));
			vecIndex.push_back(1 + (y)*ringVertexCount + (x + 1));
			vecIndex.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			vecIndex.push_back(1 + (y + 1) * ringVertexCount + (x));
			vecIndex.push_back(1 + (y)*ringVertexCount + (x + 1));
			vecIndex.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// 남극 인덱스
	UINT bottomIndex = static_cast<UINT>(vecVertex.size()) - 1;
	UINT lastRingStartIndex = bottomIndex - ringVertexCount;
	for (UINT i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		vecIndex.push_back(bottomIndex);
		vecIndex.push_back(lastRingStartIndex + i);
		vecIndex.push_back(lastRingStartIndex + i + 1);
	}

	CMesh* mesh = new CMesh;

	if (FAILED(mesh->Init(vecVertex, vecIndex)))
		return E_FAIL;

	AddAsset(L"Sphere", mesh);

	return S_OK;
}

CTexture* CAssetManager::CreateTexture(const std::wstring& name, DXGI_FORMAT format, UINT32 width, UINT32 height, const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags, D3D12_RESOURCE_FLAGS resFlags, Vec4 clearColor)
{
	CTexture* texture = new CTexture;
	texture->Create(format, width, height, heapProperty, heapFlags, resFlags, clearColor);
	AddAsset(name, texture);

	return texture;
}

CTexture* CAssetManager::CreateTextureFromResource(const std::wstring& name, ComPtr<ID3D12Resource> tex2D)
{
	CTexture* texture = new CTexture;
	texture->CreateFromResource(tex2D);
	AddAsset(name, texture);

	return texture;
}
