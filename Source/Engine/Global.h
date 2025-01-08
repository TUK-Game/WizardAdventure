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
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include "d3dx12.h"
#include "SimpleMath.h"
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

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
};