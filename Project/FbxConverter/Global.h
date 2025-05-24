#pragma once
#include <string>
#include <Windows.h>
#include "stdio.h"
#include <vector>
#include <regex>
#include <memory>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <assert.h>
#include <unordered_map>
#include <unordered_set>
#include <DirectXMath.h>
#include "SimpleMath.h"
#include <FBX/fbxsdk.h>


#ifdef _DEBUG
#pragma comment(lib, "FBX/lib/debug/libfbxsdk-md.lib")
#pragma comment(lib, "FBX/lib/debug/libxml2-md.lib")
#pragma comment(lib, "FBX/lib/debug/zlib-md.lib")
#else
#pragma comment(lib, "FBX/lib/release/libfbxsdk-md.lib")
#pragma comment(lib, "FBX/lib/release/libxml2-md.lib")
#pragma comment(lib, "FBX/lib/release/zlib-md.lib")
#endif 

using Vec2 = DirectX::SimpleMath::Vector2;
using Vec3 = DirectX::SimpleMath::Vector3;
using Vec4 = DirectX::SimpleMath::Vector4;
using Vector2 = DirectX::SimpleMath::Vector2;
using Vector3 = DirectX::SimpleMath::Vector3;
using Vector4 = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;
using Quaternion = DirectX::SimpleMath::Quaternion;

std::wstring s2ws(const std::string& s);

std::string ws2s(const std::wstring& s);
Matrix GetMatrix(const FbxAMatrix& matrix);
