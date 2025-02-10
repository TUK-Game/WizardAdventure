#pragma once

#include "Component.h"

class CTransform :
    public CComponent
{
public:
    CTransform();
    ~CTransform();

public:
    Vec3 GetRelativePosition()          { return m_RelativePos; }
    Vec3 GetRelativeScale()             { return m_RelativeScale; }
    Vec3 GetRelativeRotation();
    Vec3 GetRelativeDir(EDir dirType)   { return m_RelativeDir[(UINT)dirType]; }
    Vec3 GetWorldPosition();
    Vec3 GetWorldScale();
    Vec3 GetWorldRotation();
    Matrix GetWorldRotationMatrix();
    Vec3 GetWorldDir(EDir dirType)      { return m_WorldDir[(UINT)dirType]; }
    Matrix GetWorldMatrix()             { return m_matWorld; }
    Matrix GetRTMatrix()             { return m_matRT; }
    

    void SetRelativePosition(Vec3 position)             { m_RelativePos = position; }
    void SetRelativePosition(float x, float y, float z) { m_RelativePos = Vec3(x, y, z); }
    void SetRelativeScale(Vec3 scale)                   { m_RelativeScale = scale; }
    void SetRelativeScale(float x, float y, float z)    { m_RelativeScale = Vec3(x, y, z); }
    void SetRelativeRotation(Vec3 rotation);
    void SetRelativeRotation(float x, float y, float z);
    void SetWorldMatrix(const Matrix& matrix);
    void SetParentTransform(CTransform* parent) { m_ParentTransform = parent; }

    void LookAt(const Vec3& dir);

    static bool CloseEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon());
    static Vec3 DecomposeRotationMatrix(const Matrix& rotation);

public:
    virtual void FinalUpdate() override;
    void GraphicsBinding();

public:
    virtual CTransform* Clone() override { return new CTransform(*this); }

private:
    // TODO: 오브젝트 계층구조 설계
    CTransform* m_ParentTransform = NULL;

    // 부모가 있다면, 부모로부터 상대적인 값
    Vec3    m_RelativePos;  
    Vec3    m_RelativeScale = Vec3(1.0f, 1.0f, 1.0f); 
    Vec3    m_RelativeRotation;  

    // 오브젝트의 방향정보
    Vec3    m_RelativeDir[(int)EDir::END];
    Vec3    m_WorldDir[(int)EDir::END];

    Matrix  m_matWorld;  // 월드 변환행렬
    Matrix  m_matRT;     // 월드 변환행렬(scailing 제외)

};

