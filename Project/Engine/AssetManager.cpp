#include "pch.h"
#include "AssetManager.h"

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

	if (FAILED(LoadMaterial()))
		return E_FAIL;

	if (FAILED(LoadGraphicShader()))
		return E_FAIL;

	if (FAILED(LoadComputeShader()))
		return E_FAIL;

	return S_OK;
}

void CAssetManager::AddAsset(const std::wstring& key, CSharedPtr<CAsset> asset)
{
	EAsset_Type type = asset->GetType();

	auto iter = m_mapAsset[(int)type].find(key);

	assert(iter == m_mapAsset[(int)type].end());

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
	return S_OK;
}

int CAssetManager::LoadMaterial()
{
	return S_OK;
}

int CAssetManager::LoadGraphicShader()
{
	return S_OK;
}

int CAssetManager::LoadComputeShader()
{
	return S_OK;
}

int CAssetManager::CreateCubeMesh()
{
	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	std::vector<Vertex> vecVertex(24);

	// ¾Õ¸é
	vecVertex[0] = Vertex(Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vecVertex[1] = Vertex(Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vecVertex[2] = Vertex(Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vecVertex[3] = Vertex(Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	// µÞ¸é
	vecVertex[4] = Vertex(Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vecVertex[5] = Vertex(Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vecVertex[6] = Vertex(Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vecVertex[7] = Vertex(Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// À­¸é
	vecVertex[8] = Vertex(Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vecVertex[9] = Vertex(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vecVertex[10] = Vertex(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vecVertex[11] = Vertex(Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	// ¾Æ·§¸é
	vecVertex[12] = Vertex(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vecVertex[13] = Vertex(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vecVertex[14] = Vertex(Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vecVertex[15] = Vertex(Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// ¿ÞÂÊ¸é
	vecVertex[16] = Vertex(Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vecVertex[17] = Vertex(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vecVertex[18] = Vertex(Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vecVertex[19] = Vertex(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	// ¿À¸¥ÂÊ¸é
	vecVertex[20] = Vertex(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vecVertex[21] = Vertex(Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vecVertex[22] = Vertex(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vecVertex[23] = Vertex(Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));

	std::vector<UINT> vecIndex(36);

	// ¾Õ¸é
	vecIndex[0] = 0; vecIndex[1] = 1; vecIndex[2] = 2;
	vecIndex[3] = 0; vecIndex[4] = 2; vecIndex[5] = 3;
	// µÞ¸é
	vecIndex[6] = 4; vecIndex[7] = 5; vecIndex[8] = 6;
	vecIndex[9] = 4; vecIndex[10] = 6; vecIndex[11] = 7;
	// À­¸é
	vecIndex[12] = 8; vecIndex[13] = 9; vecIndex[14] = 10;
	vecIndex[15] = 8; vecIndex[16] = 10; vecIndex[17] = 11;
	// ¾Æ·§¸é
	vecIndex[18] = 12; vecIndex[19] = 13; vecIndex[20] = 14;
	vecIndex[21] = 12; vecIndex[22] = 14; vecIndex[23] = 15;
	// ¿ÞÂÊ¸é
	vecIndex[24] = 16; vecIndex[25] = 17; vecIndex[26] = 18;
	vecIndex[27] = 16; vecIndex[28] = 18; vecIndex[29] = 19;
	// ¿À¸¥ÂÊ¸é
	vecIndex[30] = 20; vecIndex[31] = 21; vecIndex[32] = 22;
	vecIndex[33] = 20; vecIndex[34] = 22; vecIndex[35] = 23;

	std::shared_ptr<CMesh> mesh = std::make_shared<CMesh>();

	if (FAILED(mesh->Init(vecVertex, vecIndex)))
		return E_FAIL;

	return S_OK;
}

int CAssetManager::CreateSphereMesh()
{
	float radius = 0.5f; // ±¸ÀÇ ¹ÝÁö¸§
	UINT stackCount = 20; // °¡·Î ºÐÇÒ
	UINT sliceCount = 20; // ¼¼·Î ºÐÇÒ

	std::vector<Vertex> vecVertex;

	Vertex v;

	// ºÏ±Ø
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

	// °í¸®¸¶´Ù µ¹¸é¼­ Á¤Á¡À» °è»êÇÑ´Ù (ºÏ±Ø/³²±Ø ´ÜÀÏÁ¡Àº °í¸®°¡ X)
	for (UINT y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// °í¸®¿¡ À§Ä¡ÇÑ Á¤Á¡
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

	// ³²±Ø
	v.Pos = Vec3(0.0f, -radius, 0.0f);
	v.UV = Vec2(0.5f, 1.0f);
	v.Normal = v.Pos;
	v.Normal.Normalize();
	v.Tangent = Vec3(1.0f, 0.0f, 0.0f);
	vecVertex.push_back(v);

	std::vector<UINT> vecIndex(36);

	// ºÏ±Ø ÀÎµ¦½º
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		vecIndex.push_back(0);
		vecIndex.push_back(i + 2);
		vecIndex.push_back(i + 1);
	}

	// ¸öÅë ÀÎµ¦½º
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

	// ³²±Ø ÀÎµ¦½º
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

	std::shared_ptr<CMesh> mesh = std::make_shared<CMesh>();

	if (FAILED(mesh->Init(vecVertex, vecIndex)))
		return E_FAIL;

	return S_OK;
}
