#pragma once
#include "GameObject.h"

class CMonsterHPBar :
    public CGameObject
{
public:
    CMonsterHPBar();
    void SetOffsetY(float offsetY);

    virtual void FinalUpdate() override;

private:
};

