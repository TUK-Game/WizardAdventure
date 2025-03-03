// pch.cpp: 미리 컴파일된 헤더에 해당하는 소스 파일

#include "pch.h"

// 미리 컴파일된 헤더를 사용하는 경우 컴파일이 성공하려면 이 소스 파일이 필요합니다.

std::wstring s2ws(const std::string& s)
{
	INT32 len;
	INT32 slength = static_cast<INT32>(s.length()) + 1;
	len = ::MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	::MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring ret(buf);
	delete[] buf;
	return ret;
}

std::string ws2s(const std::wstring& s)
{
	INT32 len;
	INT32 slength = static_cast<INT32>(s.length()) + 1;
	len = ::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	std::string r(len, '\0');
	::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}

Matrix GetMatrix(const FbxAMatrix& matrix)
{
	Matrix mat;

	for (INT32 y = 0; y < 4; ++y)
		for (INT32 x = 0; x < 4; ++x)
			mat.m[y][x] = static_cast<float>(matrix.Get(y, x));

	return mat;
}

std::string Trim(const std::string& str)
{
	std::string result = str;
	result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char ch) { return !std::isspace(ch); }));
	result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), result.end());
	return result;
}

float* MatrixToFloatPtr(DirectX::SimpleMath::Matrix& matrix)
{
	static float glmCompatibleMatrix[16]; 

	glmCompatibleMatrix[0] = matrix._11; glmCompatibleMatrix[4] = matrix._12; glmCompatibleMatrix[8] = matrix._13; glmCompatibleMatrix[12] = matrix._14;
	glmCompatibleMatrix[1] = matrix._21; glmCompatibleMatrix[5] = matrix._22; glmCompatibleMatrix[9] = matrix._23; glmCompatibleMatrix[13] = matrix._24;
	glmCompatibleMatrix[2] = matrix._31; glmCompatibleMatrix[6] = matrix._32; glmCompatibleMatrix[10] = matrix._33; glmCompatibleMatrix[14] = matrix._34;
	glmCompatibleMatrix[3] = matrix._41; glmCompatibleMatrix[7] = matrix._42; glmCompatibleMatrix[11] = matrix._43; glmCompatibleMatrix[15] = matrix._44;

	return glmCompatibleMatrix;  
}