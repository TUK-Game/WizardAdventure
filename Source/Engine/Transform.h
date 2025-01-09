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
    Vec3 GetWorldDir(EDir dirType)      { return m_WorldDir[(UINT)dirType]; }
    Matrix GetWorldMatrix()             { return m_matWorld; }


    void SetRelativePosition(Vec3 position)             { m_RelativePos = position; }
    void SetRelativePosition(float x, float y, float z) { m_RelativePos = Vec3(x, y, z); }
    void SetRelativeScale(Vec3 scale)                   { m_RelativeScale = scale; }
    void SetRelativeScale(float x, float y, float z)    { m_RelativeScale = Vec3(x, y, z); }
    void SetRelativeRotation(Vec3 rotation);
    void SetRelativeRotation(float x, float y, float z);
    void SetWorldMatrix(const Matrix& matrix)           { m_matWorld = matrix; }

public:
    virtual void FinalUpdate() override;
    void Binding();

public:
    virtual CTransform* Clone() override { return new CTransform(*this); }

private:
    // TODO: ������Ʈ �������� ����

    // �θ� �ִٸ�, �θ�κ��� ������� ��
    Vec3    m_RelativePos;  
    Vec3    m_RelativeScale;
    Vec3    m_RelativeRotation;  

    // ������Ʈ�� ��������
    Vec3    m_RelativeDir[(int)EDir::END];
    Vec3    m_WorldDir[(int)EDir::END];

    Matrix  m_matWorld;     // ���� ��ȯ���
};

