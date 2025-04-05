#pragma once
class CJsonConverter :
	public CSingleton<CJsonConverter>
{
	DECLARE_SINGLETON(CJsonConverter)

public:
	void SaveMapCollision(const std::wstring& fileName);
};

