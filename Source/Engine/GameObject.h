#pragma once

class CComponent;
class CScript;
class CRenderComponent;
class CTransform;
class CMeshRenderer;
class CCamera;
class CLight;
class CBaseCollider;
class CParticleSystem;
class CAnimator;
class CRigidBody;
class CUI;

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
    CMeshRenderer* GetMeshRenderer()                    { return (CMeshRenderer*)GetComponent(EComponent_Type::MeshRenderer); }
    CBaseCollider* GetCollider()                        { return (CBaseCollider*)GetComponent(EComponent_Type::Collider); }
    CCamera* GetCamera()                                { return (CCamera*)GetComponent(EComponent_Type::Camera); }
    CLight* GetLight()                                  { return (CLight*)GetComponent(EComponent_Type::Light); }
    CParticleSystem* GetParticleSystem()                { return (CParticleSystem*)GetComponent(EComponent_Type::ParticleSystem); }
    CAnimator* GetAnimator()                            { return (CAnimator*)GetComponent(EComponent_Type::Animator); }
    CRigidBody* GetRigidBody()                          { return (CRigidBody*)GetComponent(EComponent_Type::Rigidbody); }
    CUI* GetUI()                                        { return (CUI*)GetComponent(EComponent_Type::UI); }

    bool GetCheckFrustum() { return m_CheckFrustum; }
    void SetStatic(bool flag) { m_Static = flag; }
    bool IsStatic() { return m_Static; }

    void SetCheckFrustum(bool checkFrustum) { m_CheckFrustum = checkFrustum; }
    void SetParentTransform(CTransform* transform);
    void SetParent(CGameObject* parent) { m_Parent = parent; } 
    void SetLayerIndex(int index) { m_LayerIndex = index; }
    void SetTag(const std::wstring& tag) { m_Tag = tag; }

    int GetLayerIndex() { return m_LayerIndex; }
    std::wstring GetTag() const { return m_Tag; }

    void AddComponent(CComponent* component);
    void AddComponent(EComponent_Type type);

    void AddChild(CGameObject* obj);
    void RemoveChild(CGameObject* obj);
    void RemoveFromParent();

    virtual void Destroy();
    
public:
    virtual void Begin();
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render();

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
    bool m_CheckFrustum = true; // 프러스텀 여부
    bool m_Static = false; // 정적/동적 오브젝트 여부 
    std::wstring m_Tag = L"Default"; // 기본 태그 (Default)

};
