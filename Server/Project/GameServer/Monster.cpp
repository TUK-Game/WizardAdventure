#include "pch.h"
#include "Monster.h"

CMonster::CMonster()
{
	ObjectInfo = new Protocol::ObjectInfo();
}

CMonster::~CMonster()
{
	delete ObjectInfo;
}