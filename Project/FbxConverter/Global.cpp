#include "Global.h"

std::wstring s2ws(const std::string& s)
{
	int32_t len;
	int32_t slength = static_cast<int32_t>(s.length()) + 1;
	len = ::MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	::MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring ret(buf);
	delete[] buf;
	return ret;
}

std::string ws2s(const std::wstring& s)
{
	int32_t len;
	int32_t slength = static_cast<int32_t>(s.length()) + 1;
	len = ::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	std::string r(len, '\0');
	::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}

Matrix GetMatrix(const FbxAMatrix& matrix)
{
	Matrix mat;

	for (INT32 y = 0; y < 4; ++y)
	{
		for (INT32 x = 0; x < 4; ++x)
		{
			mat.m[y][x] = static_cast<float>(matrix.Get(y, x));
		}
	}

	return mat;
}