#pragma once
class CJsonConverter :
	public CSingleton<CJsonConverter>
{
	DECLARE_SINGLETON(CJsonConverter)

public:
	void SaveMapCollision(const std::wstring& fileName);
	void SaveMonster(const std::wstring& fileName);

private:
	void Save(std::ofstream& file, const std::vector<class CGameObject*>& objects);
};

