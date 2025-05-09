#pragma once
#include "GameObject.h"

class CMonsterHPBar :
    public CGameObject
{
public:
    CMonsterHPBar();
    void SetOffsetY(float offsetY);

    virtual void FinalUpdate() override;
    virtual void SetBeforeRenderPushParmater();

private:
    float m_HpRatio = 1.f;
};

