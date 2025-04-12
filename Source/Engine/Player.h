#pragma once
#include "GameObject.h"
#include "SkillManager.h"

class CInterpolator
{
public:
    void SetTarget(const Vec3& newPos)
    {
        auto now = std::chrono::high_resolution_clock::now();


        if (!m_HasPrevRecvTime)
        {
            m_PrevPos = newPos;
            m_TargetPos = newPos;
            m_ElapsedTime = 0.f;
            m_Duration = 1.f;
            m_LastRecvTime = now;
            m_HasPrevRecvTime = true;
            return;
        }

        m_PrevPos = GetInterpolatedPos();

        // 너무 멀면 보간안하고 즉시 반영 (ex: 순간이동)
        float dist = (newPos - m_PrevPos).Length();
        constexpr float MAX_ALLOWED_DIST = 300.f; 
        if (dist > MAX_ALLOWED_DIST)
        {
            m_PrevPos = newPos;
            m_TargetPos = newPos;
            m_ElapsedTime = 0.f;
            m_Duration = 0.1f;
            m_LastRecvTime = now;
            std::cout << "나텔탐\n";
            return;
        }

        m_TargetPos = newPos;

        // 서버에서 마지막 패킷 받은지 얼마나 지났는지
        m_Duration = std::chrono::duration<float>(now - m_LastRecvTime).count();

        // duration 보정
        constexpr float MIN_DUR = 0.033f;
        constexpr float MAX_DUR = 0.2f;
        m_Duration = std::clamp(m_Duration, MIN_DUR, MAX_DUR);

        m_ElapsedTime = 0.f;
        m_LastRecvTime = now;

        std::cout << "[Interpolator] Duration: " << m_Duration << " sec | Dist: " <<   '\n';
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

    bool IsComplete() const
    {
        return m_ElapsedTime >= m_Duration;
    }

private:
    Vec3 m_PrevPos;
    Vec3 m_TargetPos;

    float m_ElapsedTime = 0.f;
    float m_Duration = 0.1f;

    std::chrono::high_resolution_clock::time_point m_LastRecvTime;
    bool m_HasPrevRecvTime = false;
};




class CPlayer : public CGameObject
{
public:
    CPlayer(EPlayerAttribute attribute, bool Owner = false);
    virtual ~CPlayer();

    virtual void Begin();
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render();

    void Move(Vec3 moveDir, bool shouldRotate = true);
    void Attack(int skillIndex);
    void SetTarget(const Vec3& pos) { m_Interpolator->SetTarget(pos); }
    void CreateStateManager();

    Vec3 GetCurrentMoveDir() { return m_currentMoveDir; }
    EPlayerAttribute GetAttribute() const { return m_Attribute; }

    void InitStats();

public:
    virtual void CollisionBegin(CBaseCollider* src, CBaseCollider* dest) override;
    Vec3 m_Amount;
    Vec3 m_NextPosition;

private:
    Vec3 m_currentMoveDir;
    EPlayerAttribute m_Attribute;  
    CSkillManager* m_SkillManager;
    CInterpolator* m_Interpolator;

    float m_Speed = 1000.f;
};

