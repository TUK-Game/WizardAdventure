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


#ifdef _DEBUG
#pragma comment(lib, "FBX/lib/debug/libfbxsdk-md.lib")
#pragma comment(lib, "FBX/lib/debug/libxml2-md.lib")
#pragma comment(lib, "FBX/lib/debug/zlib-md.lib")
#else
#pragma comment(lib, "FBX/lib/release/libfbxsdk-md.lib")
#pragma comment(lib, "FBX/lib/release/libxml2-md.lib")
#pragma comment(lib, "FBX/lib/release/zlib-md.lib")
#endif 

std::wstring s2ws(const std::string& s);

std::string ws2s(const std::wstring& s);
