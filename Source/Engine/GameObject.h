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
class CUIButton;
class CStateManager;
class CMonsterAI;

namespace Protocol 
{
    enum MoveState : int;
}

struct Stats 
{
    int maxHp = 100;
    int currentHp = 100;
    int attack = 10;
    float moveSpeed = 300.f;
    int gold = 0.f;
    int displayHp = 100;

    void TakeDamage(int damage) {
        currentHp = max(currentHp - damage, 0);
    }

    void Heal(int amount) {
        currentHp = min(currentHp + amount, maxHp);
    }

    bool IsDead() const {
        return currentHp <= 0;
    }
};

class CInterpolator
{
public:
    float LerpAngle(float a, float b, float t)
    {
        float delta = fmodf(b - a + 540.0f, 360.0f) - 180.0f;
        return a + delta * t;
    }

    void SetTarget(const Vec3& newPos, const Vec3& newRot)
    {
        auto now = std::chrono::high_resolution_clock::now();


        if (!m_HasPrevRecvTime)
        {
            m_PrevPos = m_TargetPos = newPos;
            m_PrevRot = m_TargetRot = newRot;
            m_ElapsedTime = 0.f;
            m_Duration = 1.f;
            m_LastRecvTime = now;
            m_HasPrevRecvTime = true;
            return;
        }

        m_PrevPos = GetInterpolatedPos();
        m_PrevRot = GetInterpolatedRot();

        // 너무 멀면 보간안하고 즉시 반영 (ex: 순간이동)
        float dist = (newPos - m_PrevPos).Length();
        float distRot = (newRot - m_PrevRot).Length();
        constexpr float MAX_ALLOWED_DIST = 300.f;
        constexpr float MIN_ALLOWED_DIST = 1.f;
        if (dist > MAX_ALLOWED_DIST || dist < MIN_ALLOWED_DIST)
        {
            m_PrevPos = newPos;
            m_TargetPos = newPos;
            m_PrevRot = newRot;
            m_TargetRot = newRot;
            m_ElapsedTime = 0.f;
            m_Duration = 0.1f;
            m_LastRecvTime = now;
            return;
        }

        m_TargetPos = newPos;
        m_TargetRot = newRot;

        // 서버에서 마지막 패킷 받은지 얼마나 지났는지
        m_Duration = std::chrono::duration<float>(now - m_LastRecvTime).count();

        // duration 보정
        constexpr float MIN_DUR = 0.033f;
        constexpr float MAX_DUR = 0.2f;
        m_Duration = std::clamp(m_Duration, MIN_DUR, MAX_DUR);

        m_ElapsedTime = 0.f;
        m_LastRecvTime = now;
    }

    void Update(float deltaTime)
    {
        constexpr float MAX_DT = 0.05f;
        deltaTime = std::clamp(deltaTime, 0.f, MAX_DT);
        m_ElapsedTime += deltaTime;
    }

    Vec3 GetInterpolatedPos() const
    {
        if (m_Duration <= 0.f)
            return m_TargetPos;

        float t = m_ElapsedTime / m_Duration;
        t = std::clamp(t, 0.f, 1.f);
        return Vec3::Lerp(m_PrevPos, m_TargetPos, t);
    }

    Vec3 GetInterpolatedRot()
    {
        if (m_Duration <= 0.f)
            return m_TargetRot;

        float t = m_ElapsedTime / m_Duration;
        t = std::clamp(t, 0.f, 1.f);

        return Vec3(
            LerpAngle(m_PrevRot.x, m_TargetRot.x, t),
            LerpAngle(m_PrevRot.y, m_TargetRot.y, t),
            LerpAngle(m_PrevRot.z, m_TargetRot.z, t)
        );
    }

private:
    Vec3 m_PrevPos;
    Vec3 m_TargetPos;

    Vec3 m_PrevRot;
    Vec3 m_TargetRot;

    float m_ElapsedTime = 0.f;
    float m_Duration = 0.1f;

    std::chrono::high_resolution_clock::time_point m_LastRecvTime;
    bool m_HasPrevRecvTime = false;
};


class CGameObject :
    public CRef
{
    friend class CLayer;

public:
    CGameObject();
    ~CGameObject();

public:
    CGameObject* GetParent() const                      { return m_Parent; }
    const CGameObject* GetRootObject() const;
    CGameObject* GetRootObject();
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
    CUIButton* GetUIButton()                            { return (CUIButton*)GetComponent(EComponent_Type::UIButton); }
    CMonsterAI* GetMonsterAI()                          { return (CMonsterAI*)GetComponent(EComponent_Type::MonsterAI); }
    CScript* GetScript(int idx = 0)                     { return m_vecScript[idx]; }
    CStateManager* GetStateManager()                    { return m_StateManager; }
    bool GetInstancing()                                { return m_bInstancing; }
    bool GetCheckFrustum() { return m_CheckFrustum; }
    Vec3 GetTotalMeshSize() { return m_TotalMeshSize; }
    float GetThreshold() const { return m_Threshold; }
    bool GetDissolve() const { return m_bDissolve; }

    void SetThreshold(float threshold) { m_Threshold = threshold; }
    Protocol::MoveState GetStateForProtocol();
    bool IsStatic() { return m_Static; }
    void SetDissolve(bool flag) { m_bDissolve = flag; }
    void SetStatic(bool flag) { m_Static = flag; }
    void SetCheckFrustum(bool checkFrustum) { m_CheckFrustum = checkFrustum; }
    void SetParentTransform(CTransform* transform);
    void SetParent(CGameObject* parent) { m_Parent = parent; } 
    void SetLayerIndex(int index) { m_LayerIndex = index; }
    void SetTag(const std::wstring& tag) { m_Tag = tag; }
    void SetInstancing(bool instancing) { m_bInstancing = instancing; }
    void SetProtocolStateForClient(Protocol::MoveState state);
    void SetProtocolStateForClientMonster(Protocol::MoveState state);
    virtual void SetTarget(const Vec3& pos, const Vec3& rot) {}
    void SetTotalMeshSize(const Vec3& size) { m_TotalMeshSize = size; }

    int GetLayerIndex() { return m_LayerIndex; }
    std::wstring GetTag() const { return m_Tag; }

    void AddComponent(CComponent* component);
    void AddComponent(EComponent_Type type);

    void AddChild(CGameObject* obj);
    void RemoveChild(CGameObject* obj);
    void RemoveFromParent();


    Vec3 InteractionCameraPos(Vec3& rot, const Vec3& offset);

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

    int m_ObjectId{};
private:
    std::array<CComponent*, (int)EComponent_Type::END>  m_arrComponent;
    std::vector<CScript*>   m_vecScript;
    CRenderComponent*       m_RenderComponent;  // m_arrComponent 에서 현재 렌더링 컴포넌트를 가리키는 포인터

    CGameObject*                m_Parent;
    std::vector<CGameObject*>   m_vecChild;

    int m_LayerIndex; // 소속 레이어 번호
    bool m_CheckFrustum = true; // 프러스텀 여부
    bool m_Static = false; // 정적/동적 오브젝트 여부 
    bool m_bInstancing = false; // 인스턴싱 여부
    std::wstring m_Tag = L"Default"; // 기본 태그 (Default)


protected:
    CStateManager* m_StateManager;
    Vec3 m_TotalMeshSize{ 1.f, 1.f, 1.f };

    // dissolve
    float m_Threshold{};
    bool m_bDissolve;
};
