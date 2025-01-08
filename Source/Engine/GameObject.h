#pragma once

class CComponent;
class CScript;
class CRenderComponent;


class CGameObject :
    public CRef
{
    friend class CLayer;

public:
    CGameObject();
    ~CGameObject();

public:
    void Begin();
    void Update();
    void FinalUpdate();
    void Render();

public:
    virtual CGameObject* Clone() override { return new CGameObject(*this); }

private:
    std::array<CComponent*, (int)EComponent_Type::END>  m_arrComponent;
    std::vector<CScript*>   m_vecScript;
    CRenderComponent*       m_RenderComponent;  // m_arrComponent ���� ���� ������ ������Ʈ�� ����Ű�� ������

    CGameObject*                m_Parent;
    std::vector<CGameObject*>   m_vecChild;

    int m_LayerIdx; // �Ҽ� ���̾� ��ȣ
};

