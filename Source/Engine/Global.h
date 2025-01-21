#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include <windows.h>
#include <crtdbg.h>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <filesystem>
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
#include <FBX/fbxsdk.h>
#include "d3dx12.h"
#include "SimpleMath.h"

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")


#ifndef _DEBUG
#pragma comment(lib, "DirectxTex/DirectXTex.lib")
#else
#pragma comment(lib, "DirectxTex/DirectXTex_d.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "FBX/lib/debug/libfbxsdk-md.lib")
#pragma comment(lib, "FBX/lib/debug/libxml2-md.lib")
#pragma comment(lib, "FBX/lib/debug/zlib-md.lib")
#else
#pragma comment(lib, "FBX/lib/release/libfbxsdk-md.lib")
#pragma comment(lib, "FBX/lib/release/libxml2-md.lib")
#pragma comment(lib, "FBX/lib/release/zlib-md.lib")
#endif 



#include "Singleton.h"
#include "Flag.h"
#include "Define.h"
#include "Ref.h"
#include "SharedPtr.h"

using Vec2		= DirectX::SimpleMath::Vector2;
using Vec3		= DirectX::SimpleMath::Vector3;
using Vec4		= DirectX::SimpleMath::Vector4;
using Vector2	= DirectX::SimpleMath::Vector2;
using Vector3	= DirectX::SimpleMath::Vector3;
using Vector4	= DirectX::SimpleMath::Vector4;
using Matrix	= DirectX::SimpleMath::Matrix;

struct WindowInfo
{
	HINSTANCE		hInstance;
	HACCEL			hAccelTable;
	HWND			hWnd;
	HDC				hDC;
	std::wstring	TitleName;
	int				Width;
	int				Height;
	bool			bWindowed;	// 전체화면 or 창모드
};

struct Vertex
{
	Vertex() {}
	Vertex(Vec3 pos, Vec2 uv, Vec3 normal, Vec3 tangent)
		: Pos(pos), UV(uv), Normal(normal), Tangent(tangent)
	{
	}

	Vec3	Pos;
	Vec2	UV;
	Vec3	Normal;
	Vec3	Tangent;
	Vec4	weights;
	Vec4	indices;
};

#define MATERIAL_INT_COUNT		5
#define MATERIAL_FLOAT_COUNT	5
#define MATERIAL_TEXTURE_COUNT	5

struct MaterialParams
{
	void SetInt(unsigned char index, int value) { IntParams[index] = value; }
	void SetFloat(unsigned char index, float value) { FloatParams[index] = value; }

	std::array<int, MATERIAL_INT_COUNT> IntParams;
	std::array<float, MATERIAL_FLOAT_COUNT> FloatParams;
};

struct TransformParams
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProjection;
	Matrix matWV;
	Matrix matWVP;
};


struct CollisionProfile
{
	std::string							name;
	ECollision_Channel					channel;
	bool								enable = true;
	std::vector<ECollision_Interaction>	vecCollisionInteraction;
};

enum class LIGHT_TYPE : UINT8
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

struct LightColor
{
	Vec4	diffuse;
	Vec4	ambient;
	Vec4	specular;
};

struct LightInfo
{
	LightColor	color;
	Vec4		position;
	Vec4		direction;
	INT32		lightType;
	float		range;
	float		angle;
	INT32		padding;
};

struct LightParams
{
	UINT32		lightCount;
	Vec3		padding;
	LightInfo	lights[50];
};
