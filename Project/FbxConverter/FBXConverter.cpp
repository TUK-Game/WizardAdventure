#include "FBXConverter.h"
#define MAX_BONE 100

CFBXConverter::~CFBXConverter()
{
}

void CFBXConverter::LoadFBX(const char* filename)
{
	m_Manager = FbxManager::Create();

    // 2. I/O 설정
    FbxIOSettings* ios = FbxIOSettings::Create(m_Manager, IOSROOT);
    m_Manager->SetIOSettings(ios);

    // 3. FBX Importer 생성
    FbxImporter* importer = FbxImporter::Create(m_Manager, "");


    std::string fName = "../../Content/Texture/FBX/";
    fName += filename;
    fName += ".fbx";

    if (!importer->Initialize(fName.c_str(), -1, m_Manager->GetIOSettings())) {
        std::cerr << "Failed to load FBX file: " << fName << std::endl;
        return;
    }

    // 4. FBX Scene 로드
    m_Scene = FbxScene::Create(m_Manager, "MyScene");
    importer->Import(m_Scene);
    importer->Destroy();  // Importer는 더 이상 필요 없음

  /*  FbxAxisSystem directXSystem(FbxAxisSystem::eDirectX);
    directXSystem.ConvertScene(m_Scene);*/

    FbxGeometryConverter geometryConverter(m_Manager);
    if (!geometryConverter.Triangulate(m_Scene, true))
    {
        std::cerr << "Triangulation failed!" << std::endl;
    }

    // Animation
    LoadBones(m_Scene->GetRootNode());
    LoadAnimationInfo();

    // 5. Mesh 데이터 추출
    Parsing(m_Scene->GetRootNode());

    fName = filename;
    fName += ".jhd";
    SaveBinary(fName.c_str(), m_Meshes);

    m_Scene->Destroy();
    m_Manager->Destroy();
}
int meshNum = 0;
void CFBXConverter::LoadMesh(FbxMesh* mesh)
{
    m_Meshes.push_back(FbxMeshInfo());
    FbxMeshInfo& meshInfo = m_Meshes.back();

    std::string temp = (mesh->GetNode()->GetName());
    RemoveNumber(temp);
    meshInfo.name = s2ws(temp);

    // 노드의 기하학적 변환 포함한 월드 행렬 생성
    FbxNode* node = mesh->GetNode();
    FbxAMatrix worldTransform = node->EvaluateGlobalTransform();

    meshInfo.matrix = GetMatrix(worldTransform);
    FbxVector4 trans = worldTransform.GetT();
    FbxVector4 Rot = worldTransform.GetR();
    FbxVector4 scale = worldTransform.GetS();

    meshInfo.translate[0] = trans[0];
    meshInfo.translate[1] = trans[1];
    meshInfo.translate[2] = trans[2];
    meshInfo.translate[3] = trans[3];

    meshInfo.rotation[0] = Rot[0];
    meshInfo.rotation[1] = Rot[1];
    meshInfo.rotation[2] = Rot[2];
    meshInfo.rotation[3] = Rot[3];

    meshInfo.scale[0] = scale[0];
    meshInfo.scale[1] = scale[1];
    meshInfo.scale[2] = scale[2];
    meshInfo.scale[3] = scale[3];

    const uint32_t vertexCount = mesh->GetControlPointsCount();

    const uint32_t materialCount = mesh->GetNode()->GetMaterialCount();
    meshInfo.indices.resize(materialCount);

    FbxGeometryElementMaterial* geometryElementMaterial = mesh->GetElementMaterial();

    const uint32_t polygonSize = mesh->GetPolygonSize(0);
    assert(polygonSize == 3);

    std::vector<std::vector<float>> pos;
    GetControlPoints(mesh, pos, meshInfo);

    uint32_t vertexCounter = 0;
    std::unordered_map<Vertex, uint16_t> indexMapping;

    // 바운딩 박스 초기값 설정
    std::vector<float> minBounds{ FLT_MAX, FLT_MAX, FLT_MAX };
    std::vector<float> maxBounds{ -FLT_MAX, -FLT_MAX, -FLT_MAX };

    const uint32_t triCount = mesh->GetPolygonCount();
    for (uint32_t i = 0; i < triCount; i++)
    {
        uint32_t arrIdx[3];
        for (uint32_t j = 0; j < 3; j++)
        {
            uint32_t controlPointIndex = mesh->GetPolygonVertex(i, j);
            arrIdx[j] = controlPointIndex;

            std::vector<float> position = pos[controlPointIndex];
            std::vector<float> normal = GetNormal(mesh, controlPointIndex, vertexCounter);
            std::vector<float> tangent = GetTangent(mesh, controlPointIndex, vertexCounter);
            std::vector<float> biNormal = GetBiNormal(mesh, controlPointIndex, vertexCounter);
            std::vector<float> uv;
            uv = GetUV(mesh, controlPointIndex, vertexCounter);

            if (position.size() >= 3)
            {
                minBounds[0] = (std::min)(minBounds[0], position[0]);
                minBounds[1] = (std::min)(minBounds[1], position[1]);
                minBounds[2] = (std::min)(minBounds[2], position[2]);

                maxBounds[0] = (std::max)(maxBounds[0], position[0]);
                maxBounds[1] = (std::max)(maxBounds[1], position[1]);
                maxBounds[2] = (std::max)(maxBounds[2], position[2]);
            }

            InsertVertex(position, normal, tangent, biNormal, uv, meshInfo, indexMapping);
            vertexCounter++;
        }
    }
    meshInfo.centerPos[0] = static_cast<float>((maxBounds[0] + minBounds[0]) * 0.5f);
    meshInfo.centerPos[1] = static_cast<float>((maxBounds[1] + minBounds[1]) * 0.5f);
    meshInfo.centerPos[2] = static_cast<float>((maxBounds[2] + minBounds[2]) * 0.5f);
    meshInfo.centerPos[3] = static_cast<float>(0);

    meshInfo.maxPos[0] = static_cast<float>(maxBounds[0]);
    meshInfo.maxPos[1] = static_cast<float>(maxBounds[1]);
    meshInfo.maxPos[2] = static_cast<float>(maxBounds[2]);

    meshInfo.minPos[0] = static_cast<float>(minBounds[0]);
    meshInfo.minPos[1] = static_cast<float>(minBounds[1]);
    meshInfo.minPos[2] = static_cast<float>(minBounds[2]);
    meshInfo.boneWeights.resize(meshInfo.vertices.size());

    LoadAnimationData(mesh, &meshInfo);

}

void CFBXConverter::Parsing(FbxNode* node)
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
        Parsing(node->GetChild(i));
}

void CFBXConverter::LoadMaterial(FbxSurfaceMaterial* surfaceMaterial)
{
    FbxMaterialInfo material{};

    material.name = s2ws(surfaceMaterial->GetName());

    *(material.diffuse) = *GetMaterialData(surfaceMaterial, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
    *material.ambient = *GetMaterialData(surfaceMaterial, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
    *material.specular = *GetMaterialData(surfaceMaterial, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);

    material.diffuseTexName = GetTextureRelativeName(surfaceMaterial, FbxSurfaceMaterial::sDiffuse);
    material.normalTexName = GetTextureRelativeName(surfaceMaterial, FbxSurfaceMaterial::sNormalMap);
    material.specularTexName = GetTextureRelativeName(surfaceMaterial, FbxSurfaceMaterial::sSpecular);

    m_Meshes.back().materials.push_back(material);
}

void CFBXConverter::LoadBones(FbxNode* node, INT32 idx, INT32 parentIdx)
{
    FbxNodeAttribute* attribute = node->GetNodeAttribute();

    if (attribute && attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
    {
        std::shared_ptr<FbxBoneInfo> bone = std::make_shared<FbxBoneInfo>();
        bone->boneName = s2ws(node->GetName());
        bone->parentIndex = parentIdx;
        m_Bones.push_back(bone);
    }

    const INT32 childCount = node->GetChildCount();
    for (INT32 i = 0; i < childCount; i++)
        LoadBones(node->GetChild(i), static_cast<INT32>(m_Bones.size()), idx);
}

void CFBXConverter::LoadAnimationInfo()
{
    m_Scene->FillAnimStackNameArray(OUT m_AnimNames);

    const INT32 animCount = m_AnimNames.GetCount();
    for (INT32 i = 0; i < animCount; i++)
    {
        FbxAnimStack* animStack = m_Scene->FindMember<FbxAnimStack>(m_AnimNames[i]->Buffer());
        if (animStack == nullptr)
            continue;

        std::shared_ptr<FbxAnimClipInfo> animClip = std::make_shared<FbxAnimClipInfo>();
        animClip->name = s2ws(animStack->GetName());
        for(int j = 0; j < MAX_BONE; ++j)
           animClip->keyFrames[j].resize(m_Bones.size()); // 키프레임은 본의 개수만큼

        FbxTakeInfo* takeInfo = m_Scene->GetTakeInfo(animStack->GetName());
        animClip->startTime = takeInfo->mLocalTimeSpan.GetStart();
        animClip->endTime = takeInfo->mLocalTimeSpan.GetStop();
        animClip->mode = m_Scene->GetGlobalSettings().GetTimeMode();

        m_AnimClips.push_back(animClip);
    }
}

void CFBXConverter::LoadAnimationData(FbxMesh* mesh, FbxMeshInfo* meshInfo)
{
    const INT32 skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
    if (skinCount <= 0 || m_AnimClips.empty())
        return;
    
    static bool ba = false;
    //if (ba)
    //    return;
    meshInfo->hasAnimation = true;
    ba = true;
    for (INT32 i = 0; i < skinCount; i++)
    {
        FbxSkin* fbxSkin = static_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));

        if (fbxSkin)
        {
            FbxSkin::EType type = fbxSkin->GetSkinningType();
            if (FbxSkin::eRigid == type || FbxSkin::eLinear == type)
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

                    const INT32 animCount = m_AnimNames.Size();
                    for (INT32 k = 0; k < animCount; k++)
                        LoadKeyframe(k, mesh->GetNode(), cluster, matNodeTransform, boneIdx, meshInfo);
                }
            }
        }
    }

    FillBoneWeight(mesh, meshInfo);

    std::vector<std::shared_ptr<FbxBoneInfo>> bones = m_Bones;
    m_Bone.push_back(bones);
    meshNum++;
}

void CFBXConverter::LoadBoneWeight(FbxCluster* cluster, INT32 boneIdx, FbxMeshInfo* meshInfo)
{
    const INT32 indicesCount = cluster->GetControlPointIndicesCount();
    std::vector<Vertex> v;
    std::vector<std::pair<INT32, INT32>> s;
    for (INT32 i = 0; i < indicesCount; i++)
    {
        double weight = cluster->GetControlPointWeights()[i];
        INT32 vtxIdx = cluster->GetControlPointIndices()[i];
        for (int j = 0; j < meshInfo->vertices.size(); ++j)
        {
            if (meshInfo->vertices[j].controlPoint == vtxIdx)
            {
                meshInfo->boneWeights[j].AddWeights(boneIdx, weight);
            }
        }
    }
}

void CFBXConverter::LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& matNodeTransform, INT32 boneIdx, FbxMeshInfo* meshInfo)
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

    m_Bones[boneIdx]->matOffset = GetMatrix(matOffset.Transpose());
}

void CFBXConverter::LoadKeyframe(INT32 animIndex, FbxNode* node, FbxCluster* cluster, const FbxAMatrix& matNodeTransform, INT32 boneIdx, FbxMeshInfo* container)
{
    if (m_AnimClips.empty())
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

    FbxTime::EMode timeMode = m_Scene->GetGlobalSettings().GetTimeMode();

    // 애니메이션 골라줌
    FbxAnimStack* animStack = m_Scene->FindMember<FbxAnimStack>(m_AnimNames[animIndex]->Buffer());
    m_Scene->SetCurrentAnimationStack(OUT animStack);

    FbxLongLong startFrame = m_AnimClips[animIndex]->startTime.GetFrameCount(timeMode);
    FbxLongLong endFrame = m_AnimClips[animIndex]->endTime.GetFrameCount(timeMode);

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
        keyFrameInfo.matTransform = GetMatrix(matTransform);

        m_AnimClips[animIndex]->keyFrames[meshNum][boneIdx].push_back(keyFrameInfo);
    }
}

std::vector<float> CFBXConverter::GetNormal(FbxMesh* mesh, uint32_t controlPointIndex, uint32_t vertexCounter)
{
    if (mesh->GetElementNormalCount() < 1) // 노말의 개수를 센다.
        std::cout << "No normals!" << std::endl;

    const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0); // 노말 획득
    std::vector<float> result;
    result.resize(3);

    switch (vertexNormal->GetMappingMode()) // 매핑 모드
    {
    case FbxGeometryElement::eByControlPoint:
        // control point mapping
        switch (vertexNormal->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
            result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
            result[2] = -static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
        }
        break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex); // 인덱스를 얻어온다.
            result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
            result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
            result[2] = -static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
        }
        break;
        }
    case FbxGeometryElement::eByPolygonVertex:
        switch (vertexNormal->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
            result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
            result[2] = -static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
        }
        break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = vertexNormal->GetIndexArray().GetAt(vertexCounter); // 인덱스를 얻어온다.
            result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
            result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
            result[2] = -static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
        }
        break;
        }
    }

    return result;
}

std::vector<float> CFBXConverter::GetBiNormal(FbxMesh* mesh, uint32_t controlPointIndex, uint32_t vertexCounter)
{
    std::vector<float> result;
    result.resize(3);

    if (mesh->GetElementBinormalCount() < 1)
    {
        result[0] = 1.f;
        result[1] = 0.f;
        result[2] = 0.f;
        return result;
    }

    const FbxGeometryElementBinormal* vertexBiNormal = mesh->GetElementBinormal(0); // 노말 획득

    switch (vertexBiNormal->GetMappingMode()) // 매핑 모드
    {
    case FbxGeometryElement::eByControlPoint:
    {
        switch (vertexBiNormal->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            result[0] = static_cast<float>(vertexBiNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
            result[1] = static_cast<float>(vertexBiNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
            result[2] = -static_cast<float>(vertexBiNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
        }
        break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = vertexBiNormal->GetIndexArray().GetAt(controlPointIndex); // 인덱스를 얻어온다.
            result[0] = static_cast<float>(vertexBiNormal->GetDirectArray().GetAt(index).mData[0]);
            result[1] = static_cast<float>(vertexBiNormal->GetDirectArray().GetAt(index).mData[2]);
            result[2] = -static_cast<float>(vertexBiNormal->GetDirectArray().GetAt(index).mData[1]);
        }
        break;
        }
    }
    case FbxGeometryElement::eByPolygonVertex:
    {
        switch (vertexBiNormal->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            result[0] = static_cast<float>(vertexBiNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
            result[1] = static_cast<float>(vertexBiNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
            result[2] = -static_cast<float>(vertexBiNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
        }
        break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = vertexBiNormal->GetIndexArray().GetAt(vertexCounter); // 인덱스를 얻어온다.
            result[0] = static_cast<float>(vertexBiNormal->GetDirectArray().GetAt(index).mData[0]);
            result[1] = static_cast<float>(vertexBiNormal->GetDirectArray().GetAt(index).mData[2]);
            result[2] = -static_cast<float>(vertexBiNormal->GetDirectArray().GetAt(index).mData[1]);
        }
        break;
        }
    }
    }
    return result;
}

std::vector<float> CFBXConverter::GetTangent(FbxMesh* mesh, uint32_t controlPointIndex, uint32_t vertexCounter)
{
    std::vector<float> result;
    result.resize(3);

    if (mesh->GetElementTangentCount() < 1)
    {
        result[0] = 1.f;
        result[1] = 0.f;
        result[2] = 0.f;
        return result;
    }

    const FbxGeometryElementTangent* vertexTangent = mesh->GetElementTangent(0); // 노말 획득
    
    switch (vertexTangent->GetMappingMode()) // 매핑 모드
    {
    case FbxGeometryElement::eByControlPoint:
    {
        switch (vertexTangent->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            result[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[0]);
            result[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[2]);
            result[2] = -static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[1]);
        }
        break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = vertexTangent->GetIndexArray().GetAt(controlPointIndex); // 인덱스를 얻어온다.
            result[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
            result[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
            result[2] = -static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
        }
        break;
        }
    }
    case FbxGeometryElement::eByPolygonVertex:
    {
        switch (vertexTangent->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            result[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[0]);
            result[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[2]);
            result[2] = -static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[1]);
        }
        break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = vertexTangent->GetIndexArray().GetAt(vertexCounter); // 인덱스를 얻어온다.
            result[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
            result[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
            result[2] = -static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
        }
        break;
        }
    }
    }
    return result;
}

//for (int i = 0; i < 2; ++i)
//{
//    if (abs(uv.mData[i] < 0.00001))
//        uv.mData[i] = 0;
//}


std::vector<float> CFBXConverter::GetUV(FbxMesh* mesh, uint32_t controlPointIndex, uint32_t vertexCounter)
{
    if (mesh->GetElementUVCount() < 1)
        std::cout << "Invalid ****** Number" << std::endl;

    const FbxGeometryElementUV* vertexUV = mesh->GetElementUV(0); // 노말 획득
    std::vector<float> result;
    result.resize(2);

    switch (vertexUV->GetMappingMode()) // 매핑 모드
    {
    case FbxGeometryElement::eByControlPoint:
        // control point mapping
        switch (vertexUV->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            result[0] = static_cast<float>(vertexUV->GetDirectArray().GetAt(controlPointIndex).mData[0]);
            result[1] = 1 - static_cast<float>(vertexUV->GetDirectArray().GetAt(controlPointIndex).mData[1]);
        }
        break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = vertexUV->GetIndexArray().GetAt(controlPointIndex); // 인덱스를 얻어온다.
            result[0] = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
            result[1] = 1 - static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
        }
        break;
        }
        break;
    case FbxGeometryElement::eByPolygonVertex:
        switch (vertexUV->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            result[0] = static_cast<float>(vertexUV->GetDirectArray().GetAt(vertexCounter).mData[0]);
            result[1] = 1 - static_cast<float>(vertexUV->GetDirectArray().GetAt(vertexCounter).mData[1]);
        }
        break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = vertexUV->GetIndexArray().GetAt(vertexCounter); // 인덱스를 얻어온다.
            result[0] = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
            result[1] = 1 - (static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]));
        }
        break;
        }
        break;
    }

    return result;
}



float* CFBXConverter::GetMaterialData(FbxSurfaceMaterial* surface, const char* materialName, const char* factorName)
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

    float ret[4](
        static_cast<float>(material.mData[0] * factor),
        static_cast<float>(material.mData[1] * factor),
        static_cast<float>(material.mData[2] * factor),
        static_cast<float>(factor));

    return ret;
}

std::wstring CFBXConverter::GetTextureRelativeName(FbxSurfaceMaterial* surface, const char* materialProperty)
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

INT32 CFBXConverter::FindBoneIndex(std::string name)
{
    std::wstring boneName = std::wstring(name.begin(), name.end());

    for (UINT i = 0; i < m_Bones.size(); ++i)
    {
        if (m_Bones[i]->boneName == boneName)
            return i;
    }

    return -1;
}

FbxAMatrix CFBXConverter::GetTransform(FbxNode* node)
{
    // Geometric Transform
    FbxVector4 geoTranslation = node->GetGeometricTranslation(FbxNode::eSourcePivot);
    FbxVector4 geoRotation = node->GetGeometricRotation(FbxNode::eSourcePivot);
    FbxVector4 geoScaling = node->GetGeometricScaling(FbxNode::eSourcePivot);
    FbxAMatrix geoTransform(geoTranslation, geoRotation, geoScaling);

    return geoTransform;
}

void CFBXConverter::FillBoneWeight(FbxMesh* mesh, FbxMeshInfo* meshInfo)
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

        // 요주
        memcpy(&meshInfo->vertices[v].indices, animBoneIndex, sizeof(float) * 4);
        memcpy(&meshInfo->vertices[v].weights, animBoneWeight, sizeof(float) * 4);
    }
}

void CFBXConverter::SaveBinary(const char* filename, const std::vector<FbxMeshInfo>& mesh)
{
    std::string fileDir = "../../Content/Texture/JHD/";
    fileDir += filename;
    std::ofstream file(fileDir, std::ios::binary | std::ios::out | std::ios::trunc);

    if (!file.is_open()) 
    {
        std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
        return;
    }

    for (size_t i = 0; i < mesh.size(); ++i)
    {
        std::string name = ws2s(mesh[i].name);

        uint32_t vertexCount = static_cast<uint32_t>(mesh[i].vertices.size());
        uint32_t indexCount = static_cast<uint32_t>(mesh[i].indices.size());
        uint32_t materialCount = static_cast<uint32_t>(mesh[i].materials.size());

        WriteString("Mesh Name: ", file);
        WriteString(name, file);

        WriteString("Material Count: ", file);
        file.write(reinterpret_cast<const char*>(&materialCount), sizeof(uint32_t));

        for (const auto& fbxInfo : mesh[i].materials)
        {
            std::string diffuseTexName = std::filesystem::path(fbxInfo.diffuseTexName).filename().string();
            std::string normalTexName = std::filesystem::path(fbxInfo.normalTexName).filename().string();
            std::string specularTexName = std::filesystem::path(fbxInfo.specularTexName).filename().string();

            if (diffuseTexName.empty())
                diffuseTexName = "EMPTY";
            if (normalTexName.empty())
                normalTexName = "EMPTY";
            if (specularTexName.empty())
                specularTexName = "EMPTY";

            std::string n = ws2s(fbxInfo.name);
            if (n == "")
            {
                int a = 3;
            }
            WriteString("Material Name: ", file);
            WriteString(n, file);
            WriteString("Diffuse: ", file);
            WriteString(diffuseTexName, file);
            WriteString("Normal: ", file);
            WriteString(normalTexName, file);
            WriteString("Specular: ", file);
            WriteString(specularTexName, file);
        }

        WriteString("Vertex Count: ", file);
        file.write(reinterpret_cast<const char*>(&vertexCount), sizeof(uint32_t));

        WriteString("Vertices:\n", file);
        for (const auto& v : mesh[i].vertices) 
        {
            file.write(reinterpret_cast<const char*>(&v.position), sizeof(v.position));
        }

        WriteString("Normals:\n", file);
        for (const auto& v : mesh[i].vertices)
        {
            file.write(reinterpret_cast<const char*>(&v.normal), sizeof(v.normal));
        }

        WriteString("Tangents:\n", file);
        for (const auto& v : mesh[i].vertices)
        {
            file.write(reinterpret_cast<const char*>(&v.tangent), sizeof(v.tangent));
        }


        WriteString("UV:\n", file);
        for (const auto& v : mesh[i].vertices)
        {
            file.write(reinterpret_cast<const char*>(&v.uv), sizeof(v.uv));
        }

        WriteString("Transform:\n", file);
        file.write(reinterpret_cast<const char*>(&mesh[i].matrix), sizeof(mesh[i].matrix));

        WriteString("Translate:\n", file);
        file.write(reinterpret_cast<const char*>(&mesh[i].translate), sizeof(mesh[i].translate));
        WriteString("Rotation:\n", file);
        file.write(reinterpret_cast<const char*>(&mesh[i].rotation), sizeof(mesh[i].rotation));
        WriteString("Scale:\n", file);
        file.write(reinterpret_cast<const char*>(&mesh[i].scale), sizeof(mesh[i].scale));

        WriteString("BoundingBox:\n", file);
        file.write(reinterpret_cast<const char*>(&mesh[i].centerPos), sizeof(mesh[i].centerPos));
        file.write(reinterpret_cast<const char*>(&mesh[i].maxPos), sizeof(mesh[i].maxPos));
        file.write(reinterpret_cast<const char*>(&mesh[i].minPos), sizeof(mesh[i].minPos));

        WriteString("Index Count: ", file);
        size_t ic = mesh[i].indices.size();
        file.write(reinterpret_cast<const char*>(&ic), sizeof(size_t));
        WriteString("Indices:\n", file);
        for (const auto& row : mesh[i].indices) 
        {
            uint32_t colCount = row.size();
            file.write(reinterpret_cast<const char*>(&colCount), sizeof(uint32_t)); // 열 크기 저장
            file.write(reinterpret_cast<const char*>(row.data()), colCount * sizeof(UINT32)); // 데이터 저장
        }
        if (!mesh[i].hasAnimation)
        {
            WriteString("NEXT", file);
            continue;
        }

        WriteString("Weights:\n", file);
        for (const auto& v : mesh[i].vertices)
        {
            file.write(reinterpret_cast<const char*>(&v.weights), sizeof(v.weights));
        }

        WriteString("AnimIndices:\n", file);
        for (const auto& v : mesh[i].vertices)
        {
            file.write(reinterpret_cast<const char*>(&v.indices), sizeof(v.indices));
        }

        // 요주
        WriteString("BoneInfo:\n", file);
        size_t size = m_Bone[i].size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
        for (const auto& bone : m_Bone[i])
        {
            std::string temp = ws2s(bone->boneName);
            WriteString(temp, file);
            file.write(reinterpret_cast<const char*>(&(bone->parentIndex)), sizeof(INT32));
            Matrix mat = bone->matOffset;
            file.write(reinterpret_cast<const char*>(&(mat)), sizeof(Matrix));
        }

        // 요주
        WriteString("NEXT", file);
    }

    WriteString("AnimClipInfo:\n", file);
    size_t size = m_AnimClips.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    for (const auto& clipInfo : m_AnimClips)
    {
        std::string temp = ws2s(clipInfo->name);
        WriteString(temp, file);
        double timeValue = clipInfo->startTime.GetSecondDouble();
        INT32 startFrame = static_cast<INT32>(clipInfo->startTime.GetFrameCount(clipInfo->mode));
        file.write(reinterpret_cast<const char*>(&timeValue), sizeof(double));
        file.write(reinterpret_cast<const char*>(&startFrame), sizeof(INT32));
        timeValue = clipInfo->endTime.GetSecondDouble();
        INT32 endFrame = static_cast<INT32>(clipInfo->endTime.GetFrameCount(clipInfo->mode));
        file.write(reinterpret_cast<const char*>(&timeValue), sizeof(double));
        file.write(reinterpret_cast<const char*>(&endFrame), sizeof(INT32));
        file.write(reinterpret_cast<const char*>(&meshNum), sizeof(meshNum));
        for (int i = 0; i < meshNum; ++i)
        {
            // 1. 외부 벡터의 크기 저장
            size_t outerSize = clipInfo->keyFrames[i].size();
            file.write(reinterpret_cast<const char*>(&outerSize), sizeof(outerSize));

            // 2. 내부 벡터 크기 및 내용 저장
            for (const auto& innerVec : clipInfo->keyFrames[i])
            {
                // 2.1 내부 벡터의 크기 저장
                size_t innerSize = innerVec.size();
                file.write(reinterpret_cast<const char*>(&innerSize), sizeof(innerSize));

                // 2.2 내부 벡터의 각 FbxKeyFrameInfo 객체 저장
                for (const auto& keyFrame : innerVec)
                {
                    file.write(reinterpret_cast<const char*>(&keyFrame.time), sizeof(keyFrame.time));
                    file.write(reinterpret_cast<const char*>(&keyFrame.matTransform), sizeof(keyFrame.matTransform));
                }
            }
        }
    }

    WriteString("END", file);
    file.close(); 
}

void CFBXConverter::RemoveNumber(std::string& name)
{
    std::regex pattern("(_\\d+)$");
    name = std::regex_replace(name, pattern, "");

    size_t pos = name.find(" (");
    if (pos != std::string::npos) {
        name = name.substr(0, pos); // 숫자가 시작하는 부분 이전까지만 저장
    }
}

void CFBXConverter::WriteString(std::string& str, std::ofstream& file)
{
    if (!strcmp(str.c_str(), ""))
        return;

    uint32_t  len = static_cast<uint32_t>(str.size());
    file.write(reinterpret_cast<const char*>(&len), sizeof(len));
    file.write(str.c_str(), len);
}

void CFBXConverter::WriteString(const char* str, std::ofstream& file)
{
    std::string s = str;
    WriteString(s, file);
}

void CFBXConverter::WriteEndl(std::string& str, std::ofstream& file)
{
    str.push_back('\n');
}

void CFBXConverter::GetControlPoints(FbxMesh* mesh, std::vector<std::vector<float>>& pos, FbxMeshInfo& info)
{
    unsigned int count = mesh->GetControlPointsCount();
    FbxVector4* controlPoints = mesh->GetControlPoints();
    pos.resize(count);
    for (int i = 0; i < count; ++i)
    {
        pos[i].resize(4);
        FbxVector4 worldPos = controlPoints[i];

        pos[i][0] = static_cast<float>(worldPos.mData[0]);
        pos[i][1] = static_cast<float>(worldPos.mData[2]);
        pos[i][2] = static_cast<float>(worldPos.mData[1]);
        pos[i][3] = i;
    }
}

void CFBXConverter::InsertVertex(std::vector<float>& position, std::vector<float>& normal, std::vector<float>& tangent, std::vector<float>& biNormal, std::vector<float>& uv, FbxMeshInfo& info, std::unordered_map<Vertex, uint16_t>& indexMapping)
{
    Vertex vertex = { position, normal, tangent, biNormal, uv };
    vertex.controlPoint = position[3];
    auto lookup = indexMapping.find(vertex);
    uint16_t index;

    if (lookup != indexMapping.end())
    {
        index = lookup->second;
    }
    else
    {
        index = static_cast<uint16_t>(info.vertices.size());
        indexMapping[vertex] = index;
        info.vertices.push_back(vertex);
    }

    // 삼각형 정점이 3개 모이면 와인딩 순서 확인 후 저장
    info.indices[0].push_back(index);

    if (info.indices[0].size() % 3 == 0) // 삼각형 하나가 완성되었을 때
    {
        size_t i = info.indices[0].size();
        std::swap(info.indices[0][i - 2], info.indices[0][i - 1]);  // 정점 순서 반전
    }
}

