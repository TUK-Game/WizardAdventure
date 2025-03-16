#pragma once

class CSaveLoadManager :
	public CSingleton<CSaveLoadManager>
{
	DECLARE_SINGLETON(CSaveLoadManager)

public:

	//void Save(std::wstring& fileName);
	void Save();
	void Load(const std::wstring& fileNmae);

	void WriteString(const std::wstring& str, std::ofstream& file);
	void WriteString(std::string& str, std::ofstream& file);
	void WriteString(const char* str, std::ofstream& file);

};

