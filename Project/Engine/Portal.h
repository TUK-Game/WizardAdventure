#pragma once
#include "GameObject.h"

class CPortal : public CGameObject
{
public:
    CPortal();
    virtual ~CPortal();

public:
    virtual void Update();

private:
    float m_RotationSpeed; 
    int m_PortalParticleId = -1;

    CGameObject* m_Floor = nullptr;
    Vec4 tint = Vec4(0.5f, 0.7f, 1.0f, .7f);
};