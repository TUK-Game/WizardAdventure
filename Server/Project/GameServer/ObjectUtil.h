#pragma once

class CObjectUtil
{
public:
	static CPlayerRef CreatePlayer(CGameSessionRef session);
	static CGameObjectRef CreateObject();
	static CMonsterRef CreateMonster();
	static CProjectileRef CreateProjectile();

	template<typename T>
	static std::shared_ptr<T> CreateObject()
	{
		// ID »ý¼º
		const int64 newId = s_IdGenerator.fetch_add(1);

		std::shared_ptr<T> object = std::make_shared<T>();

		object->ObjectInfo->set_object_id(newId);
		object->PosInfo->set_object_id(newId);


		return object;
	}

private:
	static std::atomic<int64>	s_IdGenerator;
};

