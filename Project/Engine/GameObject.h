#pragma once

class CComponent;
class CScript;
class CRenderComponent;
class CTransform;
class CMeshRenderer;
class CCamera;
class CBaseCollider;

class CGameObject :
    public CRef
{
    friend class CLayer;

public:
    CGameObject();
    ~CGameObject();

public:
    CGameObject* GetParent() const                      { return m_Parent; }
    const std::vector<CGameObject*>& GetChild() const   { return m_vecChild; }
    CComponent* GetComponent(EComponent_Type type)      { return m_arrComponent[(int)type]; }
    CRenderComponent* GetRenderComponent()              { return m_RenderComponent; }
    CTransform* GetTransform()                          { return (CTransform*)GetComponent(EComponent_Type::Transform); }
    CMeshRenderer* GetMeshRenderer() { return (CMeshRenderer*)GetComponent(EComponent_Type::MeshRenderer); }
    CBaseCollider* GetCollider()                    { return (CBaseCollider*)GetComponent(EComponent_Type::Collider); }
    CCamera* GetCamera()                                { return (CCamera*)GetComponent(EComponent_Type::Camera); }
    bool GetCheckFrustum() { return _checkFrustum; }

    void SetCheckFrustum(bool checkFrustum) { _checkFrustum = checkFrustum; }
    void SetParentTransform(CTransform* transform);
    void SetParent(CGameObject* parent) { m_Parent = parent; } 

    void AddComponent(CComponent* component);
    void AddChild(CGameObject* obj);
public:
    void Begin();
    void Update();
    void FinalUpdate();
    void Render();

public:
    virtual CGameObject* Clone() override { return new CGameObject(*this); }

    virtual void CollisionBegin(CBaseCollider* src, CBaseCollider* dest);
    virtual void CollisionEnd(CBaseCollider* src, CBaseCollider* dest) {}

private:
    std::array<CComponent*, (int)EComponent_Type::END>  m_arrComponent;
    std::vector<CScript*>   m_vecScript;
    CRenderComponent*       m_RenderComponent;  // m_arrComponent 에서 현재 렌더링 컴포넌트를 가리키는 포인터

    CGameObject*                m_Parent;
    std::vector<CGameObject*>   m_vecChild;

    int m_LayerIndex; // 소속 레이어 번호
    bool _checkFrustum = true; // 프러스텀 여부
};

