#pragma once
#include "TriggerBox.h"
class CSavePositionBox : public CTriggerBox
{
public:
	CSavePositionBox();
	virtual ~CSavePositionBox();

public:
	virtual void CollisionBegin(CBoxCollider* src, CBoxCollider* dest) override;


};

