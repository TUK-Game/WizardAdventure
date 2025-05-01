#pragma once
class CJsonConverter :
	public CSingleton<CJsonConverter>
{
	DECLARE_SINGLETON(CJsonConverter)

public:
	void SaveMapCollision(const std::wstring& fileName);
	void SaveMonster(const std::wstring& fileName);
	void SaveNPC(const std::wstring& fileName);
};

