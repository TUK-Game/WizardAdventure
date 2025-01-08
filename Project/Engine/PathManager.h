#pragma once

class CPathManager :
	public CSingleton<CPathManager>
{
	DECLARE_SINGLETON(CPathManager)

public:
	void Init();

public:
	bool AddPath(const std::wstring& name, const std::wstring& directoryName, const std::wstring& basePathName = ROOT_PATH);
	std::filesystem::path FindPath(const std::wstring& name);

private:
	std::unordered_map<std::wstring, std::filesystem::path>	m_mapPath;
};

