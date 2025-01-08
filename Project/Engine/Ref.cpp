#include "pch.h"
#include "Ref.h"

CRef::CRef()
    : m_RefCount(0)
    , m_TypeID(0)
    , m_bEnable(true)
    , m_bActive(true)
{
}

CRef::CRef(const CRef& ref)
    : m_RefCount(0)
    , m_TypeName(ref.m_TypeName)
    , m_TypeID(ref.m_TypeID)
    , m_bEnable(ref.m_bEnable)
    , m_bActive(ref.m_bActive)
{
}

CRef::~CRef()
{
}

void CRef::AddRef()
{
    ++m_RefCount;
}

int CRef::ReleaseRef()
{
    --m_RefCount;

    if (m_RefCount <= 0)
    {
        delete this;
        return 0;
    }

    return m_RefCount;
}

void CRef::Destroy()
{
    m_bActive = false;
}
