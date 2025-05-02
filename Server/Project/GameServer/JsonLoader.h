#pragma once
class CJsonLoader
{
public:
	static void LoadMap(const std::wstring& fileName, CRoomRef room);
	static void LoadMonster(const std::wstring& fileName, CRoomRef room);
	static void LoadNPC(const std::wstring& fileName, CRoomRef room);
	static void LoadItem(const std::wstring& fileName, std::unordered_map<uint32, CItemRef>& itemMap);
};

