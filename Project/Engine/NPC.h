#pragma once
#include "GameObject.h"

class CNPC : public CGameObject
{
public:
    CNPC();
    virtual ~CNPC();

    virtual void Begin();
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render();

    void CreateStateManager();
};

