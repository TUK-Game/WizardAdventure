#include "pch.h"
#include "FBXLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "filesystem"
#include "AssetManager.h"

using namespace std;

FBXLoader::FBXLoader()
{

}

FBXLoader::~FBXLoader()
{
	if (_scene)
		_scene->Destroy();
	if (_manager)
		_manager->Destroy();
}

void FBXLoader::LoadFbx(const std::wstring& path)
{
	// 파일 데이터 로드
	Import(path);

	// Animation	
	LoadBones(_scene->GetRootNode());
	LoadAnimationInfo();

	// 로드된 데이터 파싱 (Mesh/Material/Skin)
	ParseNode(_scene->GetRootNode());

	// 우리 구조에 맞게 Texture / Material 생성
	CreateTextures();
	CreateMaterials();
}

void FBXLoader::Import(const std::wstring& path)
{
	// FBX SDK 관리자 객체 생성
	_manager = FbxManager::Create();

	// IOSettings 객체 생성 및 설정
	FbxIOSettings* settings = FbxIOSettings::Create(_manager, IOSROOT);
	_manager->SetIOSettings(settings);

	// FbxImporter 객체 생성
	_scene = FbxScene::Create(_manager, "");

	// 나중에 Texture 경로 계산할 때 쓸 것
	m_ResourceDirectory = std::filesystem::path(path).parent_path().wstring() + L"\\" + std::filesystem::path(path).filename().stem().wstring() + L".fbm";

	_importer = FbxImporter::Create(_manager, "");

	std::string strPath = ws2s(path);
	_importer->Initialize(strPath.c_str(), -1, _manager->GetIOSettings());

	_importer->Import(_scene);

	_scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::DirectX);

	// 씬 내에서 삼각형화 할 수 있는 모든 노드를 삼각형화 시킨다.
	FbxGeometryConverter geometryConverter(_manager);
	geometryConverter.Triangulate(_scene, true);

	_importer->Destroy();
}

void FBXLoader::ParseNode(FbxNode* node)
{
	FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute)
	{
		switch (attribute->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			LoadMesh(node->GetMesh());
			break;
		}
	}

	// Material 로드
	const UINT32 materialCount = node->GetMaterialCount();
	for (UINT32 i = 0; i < materialCount; ++i)
	{
		FbxSurfaceMaterial* surfaceMaterial = node->GetMaterial(i);
		LoadMaterial(surfaceMaterial);
	}
	
	// Tree 구조 재귀 호출
	const INT32 childCount = node->GetChildCount();
	for (INT32 i = 0; i < childCount; ++i)
		ParseNode(node->GetChild(i));
}

void FBXLoader::LoadMesh(FbxMesh* mesh)
{
	_meshes.push_back(FbxMeshInfo());
	FbxMeshInfo& meshInfo = _meshes.back();
	meshInfo.name = s2ws(mesh->GetNode()->GetName());
	//
	FbxAMatrix worldTransform = mesh->GetNode()->EvaluateGlobalTransform();
	meshInfo.matrix = worldTransform;// *(mesh->GetNode()->EvaluateLocalTransform());
	//
	const INT32 vertexCount = mesh->GetControlPointsCount();
	meshInfo.vertices.resize(vertexCount);
	meshInfo.boneWeights.resize(vertexCount);

	// Position
	FbxVector4* controlPoints = mesh->GetControlPoints();

	for (INT32 i = 0; i < vertexCount; ++i)
	{
		for (INT32 j = 0; j < 3; ++j)
		{
			if (abs(controlPoints[i].mData[j]) < 0.0001)
				controlPoints[i].mData[j] = 0;
		}

		meshInfo.vertices[i].Pos.x = static_cast<float>(controlPoints[i].mData[0]);
		meshInfo.vertices[i].Pos.y = static_cast<float>(controlPoints[i].mData[2]);
		meshInfo.vertices[i].Pos.z = static_cast<float>(controlPoints[i].mData[1]);

		if (meshInfo.vertices[i].Pos.x > meshInfo.maxPos.x)
			meshInfo.maxPos.x = meshInfo.vertices[i].Pos.x;
		else if (meshInfo.vertices[i].Pos.x < meshInfo.minPos.x)
			meshInfo.minPos.x = meshInfo.vertices[i].Pos.x;
		if (meshInfo.vertices[i].Pos.y > meshInfo.maxPos.y)
			meshInfo.maxPos.y = meshInfo.vertices[i].Pos.y;
		else if (meshInfo.vertices[i].Pos.y < meshInfo.minPos.y)
			meshInfo.minPos.y = meshInfo.vertices[i].Pos.y;
		if (meshInfo.vertices[i].Pos.z > meshInfo.maxPos.z)
			meshInfo.maxPos.z = meshInfo.vertices[i].Pos.z;
		else if (meshInfo.vertices[i].Pos.z < meshInfo.minPos.z)
			meshInfo.minPos.z = meshInfo.vertices[i].Pos.z;
	}

	const INT32 materialCount = mesh->GetNode()->GetMaterialCount();
	meshInfo.indices.resize(materialCount);

	FbxGeometryElementMaterial* geometryElementMaterial = mesh->GetElementMaterial();

	const INT32 polygonSize = mesh->GetPolygonSize(0);
	assert(polygonSize == 3);

	UINT32 arrIdx[3];
	UINT32 vertexCounter = 0; // 정점의 개수

	const INT32 triCount = mesh->GetPolygonCount(); // 메쉬의 삼각형 개수를 가져온다
	for (INT32 i = 0; i < triCount; i++) // 삼각형의 개수
	{
		for (INT32 j = 0; j < 3; j++) // 삼각형은 세 개의 정점으로 구성
		{
			INT32 controlPointIndex = mesh->GetPolygonVertex(i, j); // 제어점의 인덱스 추출
			arrIdx[j] = controlPointIndex;

			GetNormal(mesh, &meshInfo, controlPointIndex, vertexCounter);
			GetTangent(mesh, &meshInfo, controlPointIndex, vertexCounter);
			GetUV(mesh, &meshInfo, controlPointIndex, mesh->GetTextureUVIndex(i, j));

			vertexCounter++;
		}

		const UINT32 subsetIdx = geometryElementMaterial->GetIndexArray().GetAt(i);
		meshInfo.indices[subsetIdx].push_back(arrIdx[0]);
		meshInfo.indices[subsetIdx].push_back(arrIdx[2]);
		meshInfo.indices[subsetIdx].push_back(arrIdx[1]);
	}

	// Animation
	LoadAnimationData(mesh, &meshInfo);
}

void FBXLoader::LoadMaterial(FbxSurfaceMaterial* surfaceMaterial)
{
	FbxMaterialInfo material{};

	material.name = s2ws(surfaceMaterial->GetName());

	material.diffuse = GetMaterialData(surfaceMaterial, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	material.ambient = GetMaterialData(surfaceMaterial, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	material.specular = GetMaterialData(surfaceMaterial, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);

	material.diffuseTexName = GetTextureRelativeName(surfaceMaterial, FbxSurfaceMaterial::sDiffuse);
	material.normalTexName = GetTextureRelativeName(surfaceMaterial, FbxSurfaceMaterial::sNormalMap);
	material.specularTexName = GetTextureRelativeName(surfaceMaterial, FbxSurfaceMaterial::sSpecular);

	_meshes.back().materials.push_back(material);
}

void FBXLoader::GetNormal(FbxMesh* mesh, FbxMeshInfo* container, INT32 idx, INT32 vertexCounter)
{
	if (mesh->GetElementNormalCount() == 0)
		return;

	FbxGeometryElementNormal* normal = mesh->GetElementNormal();
	UINT32 normalIdx = 0;

	if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			normalIdx = vertexCounter;
		else
			normalIdx = normal->GetIndexArray().GetAt(vertexCounter);
	}
	else if (normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			normalIdx = idx;
		else
			normalIdx = normal->GetIndexArray().GetAt(idx);
	}
	FbxVector4 vec = normal->GetDirectArray().GetAt(normalIdx);
	container->vertices[idx].Normal.x = static_cast<float>(vec.mData[0]);
	container->vertices[idx].Normal.y = static_cast<float>(vec.mData[2]);
	container->vertices[idx].Normal.z = static_cast<float>(vec.mData[1]);
}

void FBXLoader::GetTangent(FbxMesh* mesh, FbxMeshInfo* meshInfo, INT32 idx, INT32 vertexCounter)
{
	if (mesh->GetElementTangentCount() == 0)
	{
		// TEMP : 원래는 이런 저런 알고리즘으로 Tangent 만들어줘야 함
		meshInfo->vertices[idx].Tangent.x = 1.f;
		meshInfo->vertices[idx].Tangent.y = 0.f;
		meshInfo->vertices[idx].Tangent.z = 0.f;
		return;
	}

	FbxGeometryElementTangent* tangent = mesh->GetElementTangent();
	UINT32 tangentIdx = 0;

	if (tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			tangentIdx = vertexCounter;
		else
			tangentIdx = tangent->GetIndexArray().GetAt(vertexCounter);
	}
	else if (tangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			tangentIdx = idx;
		else
			tangentIdx = tangent->GetIndexArray().GetAt(idx);
	}

	FbxVector4 vec = tangent->GetDirectArray().GetAt(tangentIdx);
	meshInfo->vertices[idx].Tangent.x = static_cast<float>(vec.mData[0]);
	meshInfo->vertices[idx].Tangent.y = static_cast<float>(vec.mData[2]);
	meshInfo->vertices[idx].Tangent.z = static_cast<float>(vec.mData[1]);
}

void FBXLoader::GetUV(FbxMesh* mesh, FbxMeshInfo* meshInfo, INT32 idx, INT32 uvIndex)
{
	FbxVector2 uv = mesh->GetElementUV()->GetDirectArray().GetAt(uvIndex);
	meshInfo->vertices[idx].UV.x = static_cast<float>(uv.mData[0]);
	meshInfo->vertices[idx].UV.y = 1.f - static_cast<float>(uv.mData[1]);
}

Vec4 FBXLoader::GetMaterialData(FbxSurfaceMaterial* surface, const char* materialName, const char* factorName)
{
	FbxDouble3  material;
	FbxDouble	factor = 0.f;

	FbxProperty materialProperty = surface->FindProperty(materialName);
	FbxProperty factorProperty = surface->FindProperty(factorName);

	if (materialProperty.IsValid() && factorProperty.IsValid())
	{
		material = materialProperty.Get<FbxDouble3>();
		factor = factorProperty.Get<FbxDouble>();
	}

	Vec4 ret = Vec4(
		static_cast<float>(material.mData[0] * factor),
		static_cast<float>(material.mData[1] * factor),
		static_cast<float>(material.mData[2] * factor),
		static_cast<float>(factor));

	return ret;
}

std::wstring FBXLoader::GetTextureRelativeName(FbxSurfaceMaterial* surface, const char* materialProperty)
{
	std::string name;

	FbxProperty textureProperty = surface->FindProperty(materialProperty);
	if (textureProperty.IsValid())
	{
		UINT32 count = textureProperty.GetSrcObjectCount();

		if (1 <= count)
		{
			FbxFileTexture* texture = textureProperty.GetSrcObject<FbxFileTexture>(0);
			if (texture)
				name = texture->GetRelativeFileName();
		}
	}

	return s2ws(name);
}

void FBXLoader::CreateTextures()
{
	for (size_t i = 0; i < _meshes.size(); i++)
	{
		for (size_t j = 0; j < _meshes[i].materials.size(); j++)
		{
			// DiffuseTexture
			{
				std::wstring relativePath = _meshes[i].materials[j].diffuseTexName.c_str();
				std::wstring filename = std::filesystem::path(relativePath).filename();
				std::wstring fullPath = m_ResourceDirectory + L"\\" + filename;
				if (filename.empty() == false)
				{
					CTexture* texture = new CTexture();
					texture->Init(fullPath);
					CAssetManager::GetInst()->AddAsset(filename, texture);
				}
			}

			// NormalTexture
			{
				std::wstring relativePath = _meshes[i].materials[j].normalTexName.c_str();
				std::wstring filename = std::filesystem::path(relativePath).filename();
				std::wstring fullPath = m_ResourceDirectory + L"\\" + filename;
				if (filename.empty() == false)
				{
					CTexture* texture = new CTexture();
					texture->Init(fullPath);
					CAssetManager::GetInst()->AddAsset(filename, texture);
				}
			}

			// SpecularTexture
			{
				std::wstring relativePath = _meshes[i].materials[j].specularTexName.c_str();
				std::wstring filename = std::filesystem::path(relativePath).filename();
				std::wstring fullPath = m_ResourceDirectory + L"\\" + filename;
				if (filename.empty() == false)
				{
					CTexture* texture = new CTexture();
					texture->Init(fullPath);
					CAssetManager::GetInst()->AddAsset(filename, texture);
				}
			}
		}
	}
}

void FBXLoader::CreateMaterials()
{
	for (size_t i = 0; i < _meshes.size(); i++)
	{
		for (size_t j = 0; j < _meshes[i].materials.size(); j++)
		{
			CMaterial* material = new CMaterial;
			std::wstring key = _meshes[i].materials[j].name;
			material->SetName(key);
			material->SetGraphicsShader(CAssetManager::GetInst()->FindAsset<CGraphicShader>(L"Deferred"));

			{
				std::wstring diffuseName = _meshes[i].materials[j].diffuseTexName.c_str();
				std::wstring filename = std::filesystem::path(diffuseName).filename();
				std::wstring key = filename;
				CSharedPtr<CTexture> diffuseTexture = CAssetManager::GetInst()->FindAsset<CTexture>(key);
				if (diffuseTexture)
					material->SetTexture(0, diffuseTexture);
			}

			{
				std::wstring normalName = _meshes[i].materials[j].normalTexName.c_str();
				std::wstring filename = std::filesystem::path(normalName).filename();
				std::wstring key = filename;
				CSharedPtr<CTexture> normalTexture = CAssetManager::GetInst()->FindAsset<CTexture>(key);
				if (normalTexture)
					material->SetTexture(1, normalTexture);
			}

			{
				std::wstring specularName = _meshes[i].materials[j].specularTexName.c_str();
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

void FBXLoader::LoadBones(FbxNode* node, INT32 idx, INT32 parentIdx)
{
	FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute && attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		shared_ptr<FbxBoneInfo> bone = make_shared<FbxBoneInfo>();
		bone->boneName = s2ws(node->GetName());
		bone->parentIndex = parentIdx;
		_bones.push_back(bone);
	}

	const INT32 childCount = node->GetChildCount();
	for (INT32 i = 0; i < childCount; i++)
		LoadBones(node->GetChild(i), static_cast<INT32>(_bones.size()), idx);
}

void FBXLoader::LoadAnimationInfo()
{
	_scene->FillAnimStackNameArray(OUT _animNames);

	const INT32 animCount = _animNames.GetCount();
	for (INT32 i = 0; i < animCount; i++)
	{
		FbxAnimStack* animStack = _scene->FindMember<FbxAnimStack>(_animNames[i]->Buffer());
		if (animStack == nullptr)
			continue;

		shared_ptr<FbxAnimClipInfo> animClip = make_shared<FbxAnimClipInfo>();
		animClip->name = s2ws(animStack->GetName());
		animClip->keyFrames.resize(_bones.size()); // 키프레임은 본의 개수만큼

		FbxTakeInfo* takeInfo = _scene->GetTakeInfo(animStack->GetName());
		animClip->startTime = takeInfo->mLocalTimeSpan.GetStart();
		animClip->endTime = takeInfo->mLocalTimeSpan.GetStop();
		animClip->mode = _scene->GetGlobalSettings().GetTimeMode();

		_animClips.push_back(animClip);
	}
}

void FBXLoader::LoadAnimationData(FbxMesh* mesh, FbxMeshInfo* meshInfo)
{
	const INT32 skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount <= 0 || _animClips.empty())
		return;

	meshInfo->hasAnimation = true;

	for (INT32 i = 0; i < skinCount; i++)
	{
		FbxSkin* fbxSkin = static_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));

		if (fbxSkin)
		{
			FbxSkin::EType type = fbxSkin->GetSkinningType();
			if (FbxSkin::eRigid == type || FbxSkin::eLinear)
			{
				const INT32 clusterCount = fbxSkin->GetClusterCount();
				for (INT32 j = 0; j < clusterCount; j++)
				{
					FbxCluster* cluster = fbxSkin->GetCluster(j);
					if (cluster->GetLink() == nullptr)
						continue;

					INT32 boneIdx = FindBoneIndex(cluster->GetLink()->GetName());
					assert(boneIdx >= 0);

					FbxAMatrix matNodeTransform = GetTransform(mesh->GetNode());
					LoadBoneWeight(cluster, boneIdx, meshInfo);
					LoadOffsetMatrix(cluster, matNodeTransform, boneIdx, meshInfo);

					const INT32 animCount = _animNames.Size();
					for (INT32 k = 0; k < animCount; k++)
						LoadKeyframe(k, mesh->GetNode(), cluster, matNodeTransform, boneIdx, meshInfo);
				}
			}
		}
	}

	FillBoneWeight(mesh, meshInfo);
}


void FBXLoader::FillBoneWeight(FbxMesh* mesh, FbxMeshInfo* meshInfo)
{
	const INT32 size = static_cast<INT32>(meshInfo->boneWeights.size());
	for (INT32 v = 0; v < size; v++)
	{
		BoneWeight& boneWeight = meshInfo->boneWeights[v];
		boneWeight.Normalize();

		float animBoneIndex[4] = {};
		float animBoneWeight[4] = {};

		const INT32 weightCount = static_cast<INT32>(boneWeight.boneWeights.size());
		for (INT32 w = 0; w < weightCount; w++)
		{
			animBoneIndex[w] = static_cast<float>(boneWeight.boneWeights[w].first);
			animBoneWeight[w] = static_cast<float>(boneWeight.boneWeights[w].second);
		}

		memcpy(&meshInfo->vertices[v].indices, animBoneIndex, sizeof(Vec4));
		memcpy(&meshInfo->vertices[v].weights, animBoneWeight, sizeof(Vec4));
	}
}

void FBXLoader::LoadBoneWeight(FbxCluster* cluster, INT32 boneIdx, FbxMeshInfo* meshInfo)
{
	const INT32 indicesCount = cluster->GetControlPointIndicesCount();
	for (INT32 i = 0; i < indicesCount; i++)
	{
		double weight = cluster->GetControlPointWeights()[i];
		INT32 vtxIdx = cluster->GetControlPointIndices()[i];
		meshInfo->boneWeights[vtxIdx].AddWeights(boneIdx, weight);
	}
}

void FBXLoader::LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& matNodeTransform, INT32 boneIdx, FbxMeshInfo* meshInfo)
{
	FbxAMatrix matClusterTrans;
	FbxAMatrix matClusterLinkTrans;
	// The transformation of the mesh at binding time 
	cluster->GetTransformMatrix(matClusterTrans);
	// The transformation of the cluster(joint) at binding time from joint space to world space 
	cluster->GetTransformLinkMatrix(matClusterLinkTrans);

	FbxVector4 V0 = { 1, 0, 0, 0 };
	FbxVector4 V1 = { 0, 0, 1, 0 };
	FbxVector4 V2 = { 0, 1, 0, 0 };
	FbxVector4 V3 = { 0, 0, 0, 1 };

	FbxAMatrix matReflect;
	matReflect[0] = V0;
	matReflect[1] = V1;
	matReflect[2] = V2;
	matReflect[3] = V3;

	FbxAMatrix matOffset;
	matOffset = matClusterLinkTrans.Inverse() * matClusterTrans;
	matOffset = matReflect * matOffset * matReflect;

	_bones[boneIdx]->matOffset = matOffset.Transpose();
}

void FBXLoader::LoadKeyframe(INT32 animIndex, FbxNode* node, FbxCluster* cluster, const FbxAMatrix& matNodeTransform, INT32 boneIdx, FbxMeshInfo* meshInfo)
{
	if (_animClips.empty())
		return;

	FbxVector4	v1 = { 1, 0, 0, 0 };
	FbxVector4	v2 = { 0, 0, 1, 0 };
	FbxVector4	v3 = { 0, 1, 0, 0 };
	FbxVector4	v4 = { 0, 0, 0, 1 };
	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	FbxTime::EMode timeMode = _scene->GetGlobalSettings().GetTimeMode();

	// 애니메이션 골라줌
	FbxAnimStack* animStack = _scene->FindMember<FbxAnimStack>(_animNames[animIndex]->Buffer());
	_scene->SetCurrentAnimationStack(OUT animStack);

	FbxLongLong startFrame = _animClips[animIndex]->startTime.GetFrameCount(timeMode);
	FbxLongLong endFrame = _animClips[animIndex]->endTime.GetFrameCount(timeMode);

	// 느림
	for (FbxLongLong frame = startFrame; frame < endFrame; frame++)
	{
		FbxKeyFrameInfo keyFrameInfo = {};
		FbxTime fbxTime = 0;

		fbxTime.SetFrame(frame, timeMode);

		FbxAMatrix matFromNode = node->EvaluateGlobalTransform(fbxTime);
		FbxAMatrix matTransform = matFromNode.Inverse() * cluster->GetLink()->EvaluateGlobalTransform(fbxTime);
		matTransform = matReflect * matTransform * matReflect;

		keyFrameInfo.time = fbxTime.GetSecondDouble();
		keyFrameInfo.matTransform = matTransform;

		_animClips[animIndex]->keyFrames[boneIdx].push_back(keyFrameInfo);
	}
}

INT32 FBXLoader::FindBoneIndex(string name)
{
	wstring boneName = wstring(name.begin(), name.end());

	for (UINT i = 0; i < _bones.size(); ++i)
	{
		if (_bones[i]->boneName == boneName)
			return i;
	}

	return -1;
}

FbxAMatrix FBXLoader::GetTransform(FbxNode* node)
{
	// Geometric Transform
	FbxVector4 geoTranslation = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 geoRotation = node->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 geoScaling = node->GetGeometricScaling(FbxNode::eSourcePivot);
	FbxAMatrix geoTransform(geoTranslation, geoRotation, geoScaling);

	//FbxAMatrix matrix = node->EvaluateGlobalTransform();

	return geoTransform;
}

FbxAMatrix FBXLoader::GetLocalTransform(FbxNode* node)
{
	// Lcl Translation, Rotation, Scaling 추출
	FbxVector4 lclTranslation = node->LclTranslation.Get();
	FbxVector4 lclRotation = node->LclRotation.Get();
	FbxVector4 lclScaling = node->LclScaling.Get();
	FbxAMatrix lclTransform(lclTranslation, lclRotation, lclScaling);

	return lclTransform;
}
