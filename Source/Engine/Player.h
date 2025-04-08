#pragma once
#include "GameObject.h"
#include "SkillManager.h"

class CInterpolator
{
public:
    void SetTarget(const Vec3& newPos)
    {
        auto now = std::chrono::high_resolution_clock::now();

        m_PrevPos = GetInterpolatedPos();  // 현재 보간 위치를 기준으로 보간 시작
        m_TargetPos = newPos;

        // duration 계산
        if (m_HasPrevRecvTime)
        {
            m_Duration = std::chrono::duration<float>(now - m_LastRecvTime).count();
            std::cout << "LAST " << m_Duration << '\n';
        }
        else
        {
            m_Duration = 0.1f; // 첫 패킷이면 기본값
            m_HasPrevRecvTime = true;
        }

        m_LastRecvTime = now;
        m_ElapsedTime = 0.f;
    }

    void Update(float deltaTime)
    {
        m_ElapsedTime += deltaTime;
    }

    Vec3 GetInterpolatedPos() const
    {
        float t = (m_Duration > 0.f) ? m_ElapsedTime / m_Duration : 1.f;
        t = std::clamp(t, 0.f, 1.f);
        return Vec3::Lerp(m_PrevPos, m_TargetPos, t);
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

    void CreateStateManager();

    Vec3 GetCurrentMoveDir() { return m_currentMoveDir; }
    EPlayerAttribute GetAttribute() const { return m_Attribute; }

public:
    virtual void CollisionBegin(CBaseCollider* src, CBaseCollider* dest) override;
    Vec3 m_Amount;
    Vec3 m_NextPosition;

    CInterpolator m_temp;
private:
    Vec3 m_currentMoveDir;
    EPlayerAttribute m_Attribute;  
    CSkillManager* m_SkillManager;
    float m_Speed = 1000.f;
};

