#pragma once
#include "Flag.h"

class CGameObject;

class CState
{
public:
	virtual ~CState() {}
    virtual void Enter(CGameObject* entity) = 0;
    virtual void Update(CGameObject* entity, float deltaTime) = 0;
    virtual void Exit(CGameObject* entity) = 0;

    virtual EState_Type GetStateType() const = 0;
};