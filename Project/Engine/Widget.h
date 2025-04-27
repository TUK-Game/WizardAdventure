#pragma once
#include "Ref.h"
#include "GameObject.h"

class CComponent;
class CRenderComponent;
class CTransform;
class CMeshRenderer;

class CWidget :
	public CGameObject
{
protected:
    CWidget();
    virtual ~CWidget();

public:
    virtual bool Init(class CPlayer* player);
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render();
        
    virtual CWidget* Clone() override { return new CWidget(*this); }

protected:
    class CPlayer* m_OwnerPlayer;
    class CWidgetWindow* m_owner;
    bool    m_mouseHovered;
};

