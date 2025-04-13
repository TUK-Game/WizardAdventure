#pragma once
class CJsonLoader
{
public:
	static void LoadMap(const std::wstring& fileName, CRoomRef room);
	static void LoadMonster(const std::wstring& fileName, CRoomRef room);

private:
	static void Load(std::ifstream& file, CRoomRef room, ECollision_Channel channel);
};

