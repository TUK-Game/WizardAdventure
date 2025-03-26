#include "pch.h"
#include "AssetManager.h"
#include "PathManager.h"
#include "GraphicShader.h"
#include "ComputeShader.h"
#include "Texture.h"
#include "Material.h"
#include "MeshData.h"
#include "Engine.h"
#include "ParticleSystem.h"
#include "SoundManager.h"

CAssetManager::CAssetManager()
{
}

CAssetManager::~CAssetManager()
{
	if (m_soundManager) {
		delete m_soundManager;
		m_soundManager = nullptr;
	}
}

int CAssetManager::Init()
{
	if (FAILED(LoadMesh()))
		return E_FAIL;

	if (FAILED(LoadTexture()))
		return E_FAIL;

	if (FAILED(LoadGraphicShader()))
		return E_FAIL;

	if (FAILED(LoadComputeShader()))
		return E_FAIL;

	if (FAILED(LoadMaterial()))
		return E_FAIL;

	if (FAILED(LoadMeshData()))
		return E_FAIL;

	if (FAILED(LoadParticle()))
		return E_FAIL;

	m_soundManager = new CSoundManager;
	if (!m_soundManager->Init())
		return E_FAIL;

	return S_OK;
}

void CAssetManager::Update()
{
	m_soundManager->Update();
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
	if (FAILED(CreatePointMesh()))
		return E_FAIL;

	if (FAILED(CreateCubeMesh()))
		return E_FAIL;

	if (FAILED(CreateSphereMesh()))
		return E_FAIL;

	if (FAILED(CreateRectangleMesh()))
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
	tex->Init(path / L"SkyboxDark.dds", RESOURCE_TEXTURE_CUBE);
	AddAsset(L"Skybox", tex);

	tex = new CTexture;
	tex->Init(path / L"bubble.png");
	AddAsset(L"Bubble", tex);

	tex = new CTexture;
	tex->Init(path / L"Leather.png");
	AddAsset(L"Leather", tex);

	tex = new CTexture;
	tex->Init(path / L"Leather_Normal.png");
	AddAsset(L"Leather_Normal", tex);

	tex = new CTexture;
	tex->Init(path / L"Rock_Normal.png");
	AddAsset(L"Rock_Normal", tex);

	tex = new CTexture;
	tex->Init(path / L"Rock.png");
	AddAsset(L"Rock", tex);

	tex = new CTexture;
	tex->Init(path / L"Map.jpg");
	AddAsset(L"Map", tex);

	tex = new CTexture;
	tex->Init(path / L"Lava.png");
	AddAsset(L"Lava", tex);

	tex = new CTexture;
	tex->Init(path / L"Lava_Normal.png");
	AddAsset(L"Lava_Normal", tex);
	return S_OK;
}

int CAssetManager::LoadMaterial()
{
	CMaterial* material = new CMaterial;
	material->SetGraphicsShader(FindAsset<CGraphicShader>(L"Deferred"));
	material->SetTexture(0, FindAsset<CTexture>(L"Kita"));
	AddAsset(L"Kita", material);

	material = new CMaterial;
	material->SetGraphicsShader(FindAsset<CGraphicShader>(L"Deferred"));
	material->SetTexture(0, FindAsset<CTexture>(L"Mushroom"));
	AddAsset(L"Mushroom", material);

	material = new CMaterial;
	material->SetGraphicsShader(FindAsset<CGraphicShader>(L"Deferred"));
	material->SetTexture(0, FindAsset<CTexture>(L"Map"));
	AddAsset(L"Map", material);

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

	// Normal Mapping
	material = new CMaterial;
	material->SetGraphicsShader(FindAsset<CGraphicShader>(L"Deferred"));
	material->SetTexture(0, FindAsset<CTexture>(L"Leather"));
	material->SetTexture(1, FindAsset<CTexture>(L"Leather_Normal"));
	AddAsset(L"Leather", material);

	material = new CMaterial;
	material->SetGraphicsShader(FindAsset<CGraphicShader>(L"Deferred"));
	material->SetTexture(0, FindAsset<CTexture>(L"Rock"));
	material->SetTexture(1, FindAsset<CTexture>(L"Rock_Normal"));
	AddAsset(L"Rock", material);

	material = new CMaterial;
	material->SetGraphicsShader(FindAsset<CGraphicShader>(L"Deferred"));
	material->SetTexture(0, FindAsset<CTexture>(L"Lava"));
	material->SetTexture(1, FindAsset<CTexture>(L"Lava_Normal"));
	AddAsset(L"Lava", material);


	material = new CMaterial;
	material->SetGraphicsShader(FindAsset<CGraphicShader>(L"Particle"));
	AddAsset(L"Particle", material);

	material = new CMaterial;
	material->SetGraphicsShader(FindAsset<CGraphicShader>(L"DebugCollision"));
	material->SetTexture(0, FindAsset<CTexture>(L"Kita"));
	AddAsset(L"DC", material);
	// LIGHT
	{
		material = new CMaterial;
		material->SetGraphicsShader(FindAsset<CGraphicShader>(L"DirLight"));
		material->SetTexture(0, FindAsset<CTexture>(L"PositionTarget"));
		material->SetTexture(1, FindAsset<CTexture>(L"NormalTarget"));
		AddAsset(L"DirLight", material);

		material = new CMaterial;
		material->SetGraphicsShader(FindAsset<CGraphicShader>(L"PointLight"));
		material->SetTexture(0, FindAsset<CTexture>(L"PositionTarget"));
		material->SetTexture(1, FindAsset<CTexture>(L"NormalTarget"));
		material->SetVec2(0, { static_cast<float>(CEngine::GetInst()->GetWindowInfo().Width), static_cast<float>(CEngine::GetInst()->GetInst()->GetWindowInfo().Height) });
		AddAsset(L"PointLight", material);

		material = new CMaterial;
		material->SetGraphicsShader(FindAsset<CGraphicShader>(L"Final"));
		material->SetTexture(0, FindAsset<CTexture>(L"DiffuseTarget"));
		material->SetTexture(1, FindAsset<CTexture>(L"DiffuseLightTarget"));
		material->SetTexture(2, FindAsset<CTexture>(L"SpecularLightTarget"));
		AddAsset(L"Final", material);
	}

	// SHADOW
	{
		material = new CMaterial;
		material->SetGraphicsShader(FindAsset<CGraphicShader>(L"Shadow"));
		AddAsset(L"Shadow", material);
	}

	{
		material = new CMaterial;
		material->SetGraphicsShader(FindAsset<CGraphicShader>(L"DeferredMap"));
		AddAsset(L"DeferredMap", material);
	}

	{
		material = new CMaterial;
		material->SetComputeShader(FindAsset<CComputeShader>(L"ComputeParticle"));
		AddAsset(L"ComputeParticle", material);

		material = new CMaterial;
		material->SetComputeShader(FindAsset<CComputeShader>(L"Compute"));
		AddAsset(L"Compute", material);

		material = new CMaterial;
		material->SetComputeShader(FindAsset<CComputeShader>(L"ComputeAnimation"));
		AddAsset(L"ComputeAnimation", material);
	}
	return S_OK;
}

int CAssetManager::LoadMeshData()
{
	CMeshData* data = CAssetManager::GetInst()->LoadJHD(L"../../Content/Texture/JHD/Mage.jhd");
	AddAsset(L"Mage", data);

	data= CAssetManager::GetInst()->LoadJHD(L"../../Content/Texture/JHD/Crab.jhd", L"Crab");
	AddAsset(L"Crab", data);

	//CMeshData* data = CAssetManager::GetInst()->LoadJHD(L"../../Content/Texture/JHD/Mage.jhd");
	//CMeshData* data = CAssetManager::GetInst()->LoadJHD(L"../../Content/Texture/JHD/wizard.jhd", L"wizard");

	CMeshData* data1 = CAssetManager::GetInst()->LoadJHD(L"../../Content/Texture/JHD/level_1.jhd", L"map");
	AddAsset(L"level_1", data1);	

	//data = CAssetManager::GetInst()->LoadFBX(L"../../Content/Texture/FBX/floor_world.fbx");
	//AddAsset(L"Floor", data);
	return S_OK;
}

int CAssetManager::LoadParticle()
{
	return S_OK;
}

int CAssetManager::LoadGraphicShader()
{
	CGraphicShader* shader = new CGraphicShader;
	std::wstring name = L"Forward.hlsl";
	LoadShader(shader, name);
	AddAsset(L"Forward", shader);

	shader = new CGraphicShader;
	name = L"Deferred.hlsl";
	LoadShader(shader, name, {SHADER_TYPE::DEFERRED, RASTERIZER_TYPE::WIREFRAME });
	AddAsset(L"DebugCollision", shader);

	shader = new CGraphicShader;
	name = L"Skybox.hlsl";
	LoadShader(shader, name, { SHADER_TYPE::FORWARD, RASTERIZER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::LESS_EQUAL });
	AddAsset(L"Skybox", shader);

	shader = new CGraphicShader;
	name = L"Deferred.hlsl";
	LoadShader(shader, name, { SHADER_TYPE::DEFERRED });
	AddAsset(L"Deferred", shader);

	shader = new CGraphicShader;
	name = L"Deferred.hlsl";
	LoadShader(shader, name, { SHADER_TYPE::DEFERRED, RASTERIZER_TYPE::CULL_BACK, DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE, BLEND_TYPE::DEFAULT }, "VS_Main", "PS_Map");
	AddAsset(L"DeferredMap", shader);

	shader = new CGraphicShader;
	name = L"particle.hlsl";
	LoadShader(shader, name, {SHADER_TYPE::PARTICLE, RASTERIZER_TYPE::CULL_BACK, DEPTH_STENCIL_TYPE::LESS_NO_WRITE, BLEND_TYPE::ALPHA_BLEND, D3D_PRIMITIVE_TOPOLOGY_POINTLIST},
		"VS_Main", "PS_Main", "GS_Main");
	AddAsset(L"Particle", shader);

	shader = new CGraphicShader;
	name = L"Forward.hlsl";
	LoadShader(shader, name, { SHADER_TYPE::FORWARD, RASTERIZER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE}, "VS_Tex", "PS_Tex");
	AddAsset(L"Texture", shader);

	// LIGHT
	{
		// DirLight
		shader = new CGraphicShader;
		name = L"Light.hlsl";
		LoadShader(shader, name, { SHADER_TYPE::LIGHTING, RASTERIZER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE, BLEND_TYPE::ONE_TO_ONE_BLEND }, "VS_DirLight", "PS_DirLight");
		AddAsset(L"DirLight", shader);

		// PointLight
		shader = new CGraphicShader;
		name = L"Light.hlsl";
		LoadShader(shader, name, { SHADER_TYPE::LIGHTING, RASTERIZER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::GREATER_EQUAL_NO_WRITE, BLEND_TYPE::ONE_TO_ONE_BLEND }, "VS_PointLight", "PS_PointLight");
		AddAsset(L"PointLight", shader);

		// Final	
		shader = new CGraphicShader;
		name = L"Light.hlsl";
		LoadShader(shader, name, { SHADER_TYPE::LIGHTING, RASTERIZER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE }, "VS_Final", "PS_Final");
		AddAsset(L"Final", shader);
	}

	// SHADOW
	{
		shader = new CGraphicShader;
		name = L"Shadow.hlsl";
		LoadShader(shader, name, { SHADER_TYPE::SHADOW, RASTERIZER_TYPE::CULL_BACK, DEPTH_STENCIL_TYPE::LESS}, "VS_MAIN", "PS_MAIN");
		AddAsset(L"Shadow", shader);
	}

	return S_OK;
}

int CAssetManager::LoadComputeShader()
{
	CComputeShader* shader = new CComputeShader;
	std::wstring name = L"Compute.hlsl";
	LoadShader(shader, name, {SHADER_TYPE::COMPUTE}, "", "", "", "CS_Main");
	AddAsset(L"Compute", shader);

	shader = new CComputeShader;
	name = L"particle.hlsl";
	LoadShader(shader, name, {SHADER_TYPE::COMPUTE}, "", "", "", "CS_Main");
	AddAsset(L"ComputeParticle", shader);

	shader = new CComputeShader;
	name = L"Animation.hlsl";
	LoadShader(shader, name, { SHADER_TYPE::COMPUTE }, "", "", "", "CS_Main");
	AddAsset(L"ComputeAnimation", shader);

	return S_OK;
}

int CAssetManager::LoadShader(CShader* shader, std::wstring& shaderName, ShaderInfo info, 
	const std::string& vs, const std::string& ps, const std::string& gs, const std::string& cs) const
{
	auto path = CPathManager::GetInst()->FindPath(HLSL_PATH);

	path /= shaderName;

	if (info.shaderType == SHADER_TYPE::COMPUTE)
	{
		if (FAILED(((CComputeShader*)shader)->Init(path, info, cs)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(((CGraphicShader*)shader)->Init(path, info, vs, ps, gs)))
			return E_FAIL;
	}

	return S_OK;
}

CMeshData* CAssetManager::LoadJHD(const std::wstring& path, const std::wstring& texturePath)
{
	std::wstring key = path;

	CMeshData* meshData = FindAsset<CMeshData>(key);
	if (meshData)
		return meshData;

	meshData = CMeshData::LoadFromJHD(path, texturePath);
	meshData->SetName(key);
	AddAsset(key, meshData);

	return meshData;
}

int CAssetManager::CreatePointMesh()
{
	std::vector<Vertex> vec(1);
	vec[0] = Vertex(Vec3(0, 0, 0), Vec2(0.5f, 0.5f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));

	std::vector<UINT> idx(1);
	idx[0] = 0;

	CMesh* mesh = new CMesh;
	if (FAILED(mesh->Init(vec, idx)))
		return E_FAIL;

	//mesh->SetMeshSize(Vec3(w2, h2, d2));

	AddAsset(L"Point", mesh);

	return S_OK;
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

int CAssetManager::CreateRectangleMesh()
{
	float w2 = 0.5f;
	float h2 = 0.5f;

	std::vector<Vertex> vec(4);

	// 앞면
	vec[0] = Vertex(Vec3(-w2, -h2, 0), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[1] = Vertex(Vec3(-w2, +h2, 0), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[2] = Vertex(Vec3(+w2, +h2, 0), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[3] = Vertex(Vec3(+w2, -h2, 0), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));

	std::vector<UINT> idx(6);

	// 앞면
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;

	CMesh* mesh = new CMesh;
	if (FAILED(mesh->Init(vec, idx)))
		return E_FAIL;

	AddAsset(L"Rectangle", mesh);

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


bool CAssetManager::CreateSoundChannel(const std::string& name)
{
	return m_soundManager->CreateSoundChannel(name);
}

bool CAssetManager::LoadSound(const std::string& groupName, const std::string& name, bool loop, const char* fileName, const std::wstring& pathName)
{
	return m_soundManager->LoadSound(groupName, name, loop, fileName, pathName);
}

bool CAssetManager::SetVolume(int volume)
{
	return m_soundManager->SetVolume(volume);
}

bool CAssetManager::SetVolume(const std::string& groupName, int volume)
{
	return m_soundManager->SetVolume(groupName, volume);
}

bool CAssetManager::SoundPlay(const std::string& name)
{
	return m_soundManager->SoundPlay(name);
}

bool CAssetManager::SoundStop(const std::string& name)
{
	return m_soundManager->SoundStop(name);
}

bool CAssetManager::SoundPause(const std::string& name)
{
	return m_soundManager->SoundPause(name);
}

bool CAssetManager::SoundResume(const std::string& name)
{
	return m_soundManager->SoundResume(name);
}

FMOD::ChannelGroup* CAssetManager::FindChannelGroup(const std::string& name)
{
	return m_soundManager->FindChannelGroup(name);
}

CSound* CAssetManager::FindSound(const std::string& name)
{
	return m_soundManager->FindSound(name);
}

void CAssetManager::ReleaseSound(const std::string& name)
{
	return m_soundManager->ReleaseSound(name);
}