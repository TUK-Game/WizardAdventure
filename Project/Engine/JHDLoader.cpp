#include "pch.h"
#include "JHDLoader.h"
#include "iostream"
#include "fstream"
#include "string"
#include "Texture.h"
#include "AssetManager.h"

void CJHDLoader::LoadFile(const char* filename, const std::wstring& textureFilename)
{
	if (textureFilename.empty())
		m_ResourceDirectory = std::filesystem::path(filename).parent_path().wstring() + L"\\" + std::filesystem::path(filename).filename().stem().wstring() + L".fbm";
	else
		m_ResourceDirectory = std::filesystem::path(filename).parent_path().wstring() + L"\\" + textureFilename + L".fbm";

	std::ifstream file(filename, std::ios::binary);
	if (!file) {
		std::cerr << "������ �� �� �����ϴ�: " << filename << std::endl;
		return;
	}
	uint32_t length;

	m_Meshes.push_back(JHDMeshInfo());
	JHDMeshInfo* meshInfo = &m_Meshes.back();
	int num;
	bool bCopy = false;
	for (; ; )
	{
		char pstrToken[64] = { '\0' };
		file.read(reinterpret_cast<char*>(&length), sizeof(length));
		file.read(pstrToken, length);

		if (strlen(pstrToken) != 0)
			std::cout << pstrToken << "\n";


		if (!strcmp(pstrToken, "END"))
		{
			break;
		}
		else if (!strcmp(pstrToken, "NEXT"))
		{
			if (!bCopy)
			{
				JHDMeshInfo* info = meshInfo->Clone();
				CAssetManager::GetInst()->AddAsset(info->name, info->Clone());
			}
			m_Meshes.push_back(JHDMeshInfo());
			meshInfo = &m_Meshes.back();
			bCopy = false;
		}
		if (!strcmp(pstrToken, "Mesh Name: "))
		{
			char mName[100] = { '\0' };
			file.read(reinterpret_cast<char*>(&length), sizeof(length));
			file.read(mName, length);
			meshInfo->name = s2ws(mName);
			CSharedPtr<JHDMeshInfo> info = CAssetManager::GetInst()->FindAsset<JHDMeshInfo>(meshInfo->name);
			if (info)
			{
				//meshInfo->vertices = info.Get()->vertices;
				//meshInfo->indices = info.Get()->indices;
				//meshInfo->materials = info.Get()->materials;
				//
				//bCopy = true;
			}
		}
		else if (!strcmp(pstrToken, "Material Count: "))
		{
			uint32_t num;
			file.read(reinterpret_cast<char*>(&num), sizeof(num));
		}
		else if (!strcmp(pstrToken, "Material Name: "))
		{
			LoadMaterial(file, bCopy);
		}
		else if (!strcmp(pstrToken, "Vertex Count: "))
		{
			uint32_t num;
			file.read(reinterpret_cast<char*>(&num), sizeof(num));
			meshInfo->vertices.resize(num);
		}
		else if (!strcmp(pstrToken, "Vertices:\n"))
		{
			for (int i = 0; i < meshInfo->vertices.size(); ++i)
			{
				float num[3];
				file.read(reinterpret_cast<char*>(&num), sizeof(num));
				if (bCopy)
					continue;
				meshInfo->vertices[i].Pos = Vec3(num[0], num[1], num[2]);
			}
		}
		else if (!strcmp(pstrToken, "Normals:\n"))
		{
			for (int i = 0; i < meshInfo->vertices.size(); ++i)
			{
				float num[3];
				file.read(reinterpret_cast<char*>(&num), sizeof(num));
				if (bCopy)
					continue;
				meshInfo->vertices[i].Normal = Vec3(num[0], num[1], num[2]);
			}
		}
		else if (!strcmp(pstrToken, "Tangents:\n"))
		{
			for (int i = 0; i < meshInfo->vertices.size(); ++i)
			{
				float num[3];
				file.read(reinterpret_cast<char*>(&num), sizeof(num));
				if (bCopy)
					continue;
				meshInfo->vertices[i].Tangent = Vec3(num[0], num[1], num[2]);
			}
		}
		else if (!strcmp(pstrToken, "UV:\n"))
		{
			for (int i = 0; i < meshInfo->vertices.size(); ++i)
			{
				float num[2];
				file.read(reinterpret_cast<char*>(&num), sizeof(num));
				if (bCopy)
					continue;
				meshInfo->vertices[i].UV = Vec2(num[0], num[1]);
			}
		}
		else if (!strcmp(pstrToken, "Weights:\n"))
		{
			for (int i = 0; i < meshInfo->vertices.size(); ++i)
			{
				float num[4];
				file.read(reinterpret_cast<char*>(&num), sizeof(num));
				if (bCopy)
					continue;
				meshInfo->vertices[i].weights = Vec4(num[0], num[1], num[2], num[3]);
			}
		}
		else if (!strcmp(pstrToken, "AnimIndices:\n"))
		{
			for (int i = 0; i < meshInfo->vertices.size(); ++i)
			{
				float num[4];
				file.read(reinterpret_cast<char*>(&num), sizeof(num));
				if (bCopy)
					continue;
				meshInfo->vertices[i].indices = Vec4(num[0], num[1], num[2], num[3]);
			}
		}
		else if (!strcmp(pstrToken, "Transform:\n"))
		{
			FbxAMatrix num;
			file.read(reinterpret_cast<char*>(&num), sizeof(num));
			meshInfo->matrix = num;
		}
		else if (!strcmp(pstrToken, "BoundingBox:\n"))
		{
			Vec3 num;
			Vec3 num2;
			Vec4 num3;
			file.read(reinterpret_cast<char*>(&num3), sizeof(num3));
			file.read(reinterpret_cast<char*>(&num), sizeof(num));
			file.read(reinterpret_cast<char*>(&num2), sizeof(num2));
			meshInfo->centerPos = num3;
			meshInfo->BoundingBoxMax = num;
			meshInfo->BoundingBoxMin = num2;
		}
		else if (!strcmp(pstrToken, "Index Count: "))
		{
			size_t num;
			file.read(reinterpret_cast<char*>(&num), sizeof(num));
			meshInfo->indices.resize(num);
		}
		else if (!strcmp(pstrToken, "Indices:\n"))
		{
			for (int j = 0; j < meshInfo->indices.size(); ++j)
			{
				uint32_t colCount;
				file.read(reinterpret_cast<char*>(&colCount), sizeof(uint32_t)); // �� ũ�� �б�
				meshInfo->indices[j].resize(colCount);
				file.read(reinterpret_cast<char*>(meshInfo->indices[j].data()), colCount * sizeof(UINT32)); // ������ �б�
			}
		}
		else if (!strcmp(pstrToken, "BoneInfo:\n"))
		{
			size_t size;
			file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
			m_Bones.resize(size);
			for (int j = 0; j < size; ++j)
			{
				INT32 iNum;
				double dNum;
				FbxAMatrix matrix;
				char mName[100] = { '\0' };
				file.read(reinterpret_cast<char*>(&length), sizeof(length));
				file.read(mName, length);
				file.read(reinterpret_cast<char*>(&iNum), sizeof(INT32));
				file.read(reinterpret_cast<char*>(&matrix), sizeof(FbxAMatrix));

				m_Bones[j] = std::make_shared<FbxBoneInfo>();
				m_Bones[j]->boneName = s2ws(mName);
				m_Bones[j]->parentIndex = iNum;
				m_Bones[j]->matOffset = matrix;
			}
		}
		else if (!strcmp(pstrToken, "AnimClipInfo:\n"))
		{
			size_t size;
			file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
			m_AnimClips.resize(size);
			for (int j = 0; j < size; ++j)
			{	
				m_AnimClips[j] = std::make_shared<FbxAnimClipInfo>();

				size_t outSize, innerSize;
				char mName[100] = { '\0' };
				long long time;
				FbxTime::EMode mode;
				file.read(reinterpret_cast<char*>(&length), sizeof(length));
				file.read(mName, length);
				file.read(reinterpret_cast<char*>(&time), sizeof(long long));
				m_AnimClips[j]->startTime = time;

				file.read(reinterpret_cast<char*>(&time), sizeof(long long));
				m_AnimClips[j]->endTime = time;

				file.read(reinterpret_cast<char*>(&mode), sizeof(mode));
				file.read(reinterpret_cast<char*>(&outSize), sizeof(size_t));

				m_AnimClips[j]->name = s2ws(mName);
				m_AnimClips[j]->mode = mode;
				m_AnimClips[j]->keyFrames.resize(outSize);
				for (int k = 0; k < outSize; ++k)
				{
					file.read(reinterpret_cast<char*>(&innerSize), sizeof(size_t));
					m_AnimClips[j]->keyFrames[k].resize(innerSize);
					for (int m = 0; m < innerSize; ++m)
					{
						FbxKeyFrameInfo in;
						file.read(reinterpret_cast<char*>(&in.time), sizeof(double));
						file.read(reinterpret_cast<char*>(&in.matTransform), sizeof(FbxAMatrix));
						m_AnimClips[j]->keyFrames[k][m].matTransform = in.matTransform;
						m_AnimClips[j]->keyFrames[k][m].time = in.time;
					}
				}
			}
		}
	}
	m_Meshes.pop_back();
	file.close();

	CreateTextures();
	CreateMaterials();
}

void CJHDLoader::LoadMaterial(std::ifstream& file, bool bCopy)
{
	JHDMaterialInfo info;

	{
		char mName[100] = { '\0' };
		uint32_t length;
		file.read(reinterpret_cast<char*>(&length), sizeof(length));
		file.read(mName, length);
		info.name = s2ws(mName);
	}
	// diffuse
	{
		char no[100] = { '\0' };
		char mName[100] = { '\0' };
		uint32_t length;
		file.read(reinterpret_cast<char*>(&length), sizeof(length));
		file.read(no, length);
		file.read(reinterpret_cast<char*>(&length), sizeof(length));
		file.read(mName, length);
		if (strcmp(mName, "EMPTY") != 0)
			info.diffuseTexName = s2ws(mName);
	}
	//	Normal
	{
		char no[100] = { '\0' };
		char mName[100] = { '\0' };
		uint32_t length;
		file.read(reinterpret_cast<char*>(&length), sizeof(length));
		file.read(no, length);
		file.read(reinterpret_cast<char*>(&length), sizeof(length));
		file.read(mName, length);
		if (strcmp(mName, "EMPTY") != 0)
			info.normalTexName = s2ws(mName);
	}
	// Specular
	{
		char sp[100] = { '\0' };
		char mName[100] = { '\0' };
		uint32_t length;
		file.read(reinterpret_cast<char*>(&length), sizeof(length));
		file.read(sp, length);
		file.read(reinterpret_cast<char*>(&length), sizeof(length));
		file.read(mName, length);
		if (strcmp(mName, "EMPTY") != 0)
			info.specularTexName = s2ws(mName);
	}

	if(!bCopy)
		m_Meshes.back().materials.push_back(info);
}

void CJHDLoader::CreateTextures()
{
	for (size_t i = 0; i < m_Meshes.size(); i++)
	{
		for (size_t j = 0; j < m_Meshes[i].materials.size(); j++)
		{
			// DiffuseTexture
			{
				std::wstring relativePath = m_Meshes[i].materials[j].diffuseTexName.c_str();
				std::wstring filename = std::filesystem::path(relativePath).filename();
				std::wstring fullPath = m_ResourceDirectory + L"\\" + filename;
				if (filename.empty() == false)
				{
					if (!CAssetManager::GetInst()->FindAsset<CTexture>(filename))
					{
						CTexture* tex = new CTexture();
						tex->Init(fullPath);
						CAssetManager::GetInst()->AddAsset(filename, tex);
					}
				}
			}

			// NormalTexture
			{
				std::wstring relativePath = m_Meshes[i].materials[j].normalTexName.c_str();
				std::wstring filename = std::filesystem::path(relativePath).filename();
				std::wstring fullPath = m_ResourceDirectory + L"\\" + filename;
				if (filename.empty() == false)
				{
					if (!CAssetManager::GetInst()->FindAsset<CTexture>(filename))
					{
						CTexture* tex = new CTexture();
						tex->Init(fullPath);
						CAssetManager::GetInst()->AddAsset(filename, tex);
					}
				}
			}

			// SpecularTexture
			{
				std::wstring relativePath = m_Meshes[i].materials[j].specularTexName.c_str();
				std::wstring filename = std::filesystem::path(relativePath).filename();
				std::wstring fullPath = m_ResourceDirectory + L"\\" + filename;
				if (filename.empty() == false)
				{
					if (!CAssetManager::GetInst()->FindAsset<CTexture>(filename))
					{
						CTexture* tex = new CTexture();
						tex->Init(fullPath);
						CAssetManager::GetInst()->AddAsset(filename, tex);
					}
				}
			}
		}
	}
}

void CJHDLoader::CreateMaterials()
{
	for (size_t i = 0; i < m_Meshes.size(); i++)
	{
		for (size_t j = 0; j < m_Meshes[i].materials.size(); j++)
		{
			if (m_Meshes[i].materials.size() >= 2)
			{
				int a = 0;
			}
			CMaterial* material = new CMaterial;
			std::wstring key = m_Meshes[i].materials[j].name;
			material->SetName(key);
			material->SetGraphicsShader(CAssetManager::GetInst()->FindAsset<CGraphicShader>(L"Deferred"));

			{
				std::wstring diffuseName = m_Meshes[i].materials[j].diffuseTexName.c_str();
				std::wstring filename = std::filesystem::path(diffuseName).filename();
				std::wstring key = filename;
				CSharedPtr<CTexture> diffuseTexture = CAssetManager::GetInst()->FindAsset<CTexture>(key);
				if (diffuseTexture)
					material->SetTexture(0, diffuseTexture);
			}

			{
				std::wstring normalName = m_Meshes[i].materials[j].normalTexName.c_str();
				std::wstring filename = std::filesystem::path(normalName).filename();
				std::wstring key = filename;
				CSharedPtr<CTexture> normalTexture = CAssetManager::GetInst()->FindAsset<CTexture>(key);
				if (normalTexture)
					material->SetTexture(1, normalTexture);
			}

			{
				std::wstring specularName = m_Meshes[i].materials[j].specularTexName.c_str();
				std::wstring filename = std::filesystem::path(specularName).filename();
				std::wstring key = filename;
				CSharedPtr<CTexture> specularTexture = CAssetManager::GetInst()->FindAsset<CTexture>(key);
				if (specularTexture)
					material->SetTexture(2, specularTexture);
			}

			CAssetManager::GetInst()->AddAsset(material->GetName(), material);
		}
	}
}

