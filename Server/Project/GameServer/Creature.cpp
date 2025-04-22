#include "pch.h"
#include "Creature.h"

CCreature::CCreature()
{
	ObjectInfo->set_object_type(Protocol::ObjectType::OBJECT_TYPE_CREATURE);
	m_Stats = new Ablity;
}

CCreature::~CCreature()
{
	if (m_Stats)
		delete m_Stats;
}
