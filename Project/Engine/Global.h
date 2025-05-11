#pragma once

#define WIN32_LEAN_AND_MEAN             

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
#include <random>
#include <d3d12.h>
#include <d2d1_3.h>
#include <dwrite.h>
#include <dwrite_1.h>
#include <d3d11on12.h>
#include <d2d1_1helper.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
#include <ServerCore/CoreGlobal.h>
#include <FBX/fbxsdk.h>
#include "d3dx12.h"
#include "SimpleMath.h"
#include "fmod.hpp"

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "d3d11")

#ifdef _DEBUG
#pragma comment(lib, "ServerCore/ServerCore_d.lib")
#pragma comment(lib, "Protobuf/libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore/ServerCore.lib")
#pragma comment(lib, "Protobuf/libprotobuf.lib")
#endif // _DEBUG

#ifndef _DEBUG
#pragma comment(lib, "FMOD/fmodL_vc.lib") 
#else
#pragma comment(lib, "FMOD/fmod_vc.lib") 
#endif

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

#ifndef _DEBUG
#pragma comment(lib, "PhysX/release/PhysXCommon_64.lib")
#pragma comment(lib, "PhysX/release/PhysX_64.lib")
#pragma comment(lib, "PhysX/release/PhysXFoundation_64.lib")
#pragma comment(lib, "PhysX/release/PhysXCooking_64.lib")
#else
#pragma comment(lib, "PhysX/debug/PhysXCommon_64.lib")
#pragma comment(lib, "PhysX/debug/PhysX_64.lib")
#pragma comment(lib, "PhysX/debug/PhysXFoundation_64.lib")
#pragma comment(lib, "PhysX/debug/PhysXCooking_64.lib")
#endif
// https://nvidia-omniverse.github.io/PhysX/physx/5.6.0/index.html

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx12.h>
#include <ImGui/imgui_impl_win32.h>


#include "Singleton.h"
#include "Flag.h"
#include "Define.h"
#include "Ref.h"
#include "SharedPtr.h"

#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

using Vec2		= DirectX::SimpleMath::Vector2;
using Vec3		= DirectX::SimpleMath::Vector3;
using Vec4		= DirectX::SimpleMath::Vector4;
using Vector2	= DirectX::SimpleMath::Vector2;
using Vector3	= DirectX::SimpleMath::Vector3;
using Vector4	= DirectX::SimpleMath::Vector4;
using Matrix	= DirectX::SimpleMath::Matrix;
using Quaternion = DirectX::SimpleMath::Quaternion;

struct WindowInfo
{
	HINSTANCE		hInstance;
	HACCEL			hAccelTable;
	HWND			hWnd;
	HDC				hDC;
	std::wstring	TitleName;
	int				Width;
	int				Height;
	bool			bWindowed;	// ��üȭ�� or â���
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

#define MATERIAL_INT_COUNT		4
#define MATERIAL_FLOAT_COUNT	4
#define MATERIAL_TEXTURE_COUNT	4
#define MATERIAL_VECTOR2_COUNT	4
#define MATERIAL_VECTOR4_COUNT	4
#define MATERIAL_MATRIX_COUNT	4

struct MaterialParams
{
	void SetInt(unsigned char index, int value) { IntParams[index] = value; }
	void SetFloat(unsigned char index, float value) { FloatParams[index] = value; }
	void SetTexOn(unsigned char index, int value) { texOnParams[index] = value; }
	void SetVec2(unsigned char index, Vec2 value) { Vec2Params[index] = value; }
	void SetVec4(unsigned char index, Vec4 value) { Vec4Params[index] = value; }
	void SetMatrix(unsigned char index, Matrix& value) { matrixParams[index] = value; }

	std::array<int, MATERIAL_INT_COUNT> IntParams;
	std::array<float, MATERIAL_FLOAT_COUNT> FloatParams;
	std::array<int, MATERIAL_TEXTURE_COUNT> texOnParams;
	std::array<Vec2, MATERIAL_VECTOR2_COUNT> Vec2Params;
	std::array<Vec4, MATERIAL_VECTOR4_COUNT> Vec4Params;
	std::array<Matrix, MATERIAL_MATRIX_COUNT> matrixParams;
};

struct TransformParams
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProjection;
	Matrix matWV;
	Matrix matWVP;
	Matrix matViewInv;
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

enum class SKILL 
{
	FIRE_BALL,
	FIRE_CIRCLE,
	FIRE_PILLAR,
	FIRE_SWORD,
	FIRE_METEORS,
	FIRE_BALL_EXPLOSION,

	ADC_BALL,
	MAX
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

struct AnimFrameParams
{
	Vec4	scale;
	Vec4	rotation; // Quaternion
	Vec4	translation;
};

struct FogParams
{
	Vec3	cameraPosition;
	float   range;
	Vec4    fogColor;
	float   Start;
};

struct MonsterStat
{
	int maxHP;
	int atk;
	float moveSpeed;
	float visionRange;
};

enum LAYER_OBJECT
{
	LAYER_CAMERA = 0,
	LAYER_BACKGROUND,

	LAYER_PLAYER = 3,
	LAYER_UI,
	LAYER_LIGHT,

	LAYER_MAP = 10,
	LAYER_MONSTER,
	LAYER_PROJECTILE,
	LAYER_EFFECT,
	LAYER_GATE,
	LAYER_NPC,
};

enum class ESkillType {
	None,
	FireBallTowardMouse,
	Meteor,
	FireBallTowardQ,
	FireTower,
	FireSwordSpread,
	// Ice, Electric 스킬도 여기에 추가 

	END
};

inline ESkillType ConvertSkillNameToType(const std::wstring& name)
{
	if (name == L"FireShot")
		return ESkillType::FireBallTowardMouse;
	if (name == L"FireRain")
		return ESkillType::Meteor;
	if (name == L"FireballExplosion")
		return ESkillType::FireBallTowardQ;
	if (name == L"FireTower")
		return ESkillType::FireTower;
	if (name == L"FireSwords")
		return ESkillType::FireSwordSpread;
}

#include <shlobj.h>
#include <filesystem>
static std::wstring GetLatestWinPixGpuCapturerPath_Cpp17()
{
	LPWSTR programFilesPath = nullptr;
	SHGetKnownFolderPath(FOLDERID_ProgramFiles, KF_FLAG_DEFAULT, NULL, &programFilesPath);

	std::filesystem::path pixInstallationPath = programFilesPath;
	pixInstallationPath /= "Microsoft PIX";

	std::wstring newestVersionFound;

	for (auto const& directory_entry : std::filesystem::directory_iterator(pixInstallationPath))
	{
		if (directory_entry.is_directory())
		{
			if (newestVersionFound.empty() || newestVersionFound < directory_entry.path().filename().c_str())
			{
				newestVersionFound = directory_entry.path().filename().c_str();
			}
		}
	}

	if (newestVersionFound.empty())
	{
		// TODO: Error, no PIX installation found
	}

	return pixInstallationPath / newestVersionFound / L"WinPixGpuCapturer.dll";
}


