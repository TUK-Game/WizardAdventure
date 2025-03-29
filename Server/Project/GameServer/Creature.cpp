#include "pch.h"
#include "Creature.h"

CCreature::CCreature()
{
	ObjectInfo->set_object_type(Protocol::ObjectType::OBJECT_TYPE_CREATURE);
}

CCreature::~CCreature()
{
}
