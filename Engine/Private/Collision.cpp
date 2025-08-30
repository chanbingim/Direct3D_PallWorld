#include "Collision.h"

CCollision::CCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CComponent(pDevice, pContext)
{
}

CCollision::CCollision(const CCollision& rhs) :
    CComponent(rhs)
{
}

HRESULT CCollision::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCollision::Initialize(void* pArg)
{
    COLLISION_DESC* CollisionDesc = static_cast<COLLISION_DESC*>(pArg);
    m_pOwner = CollisionDesc->pOwner;

    return S_OK;
}

void CCollision::BindBeginOverlapEvent(function<void(_float3 vDir, CGameObject* pHitActor)> BeginEvent)
{
    m_BeginHitFunc = BeginEvent;
}

void CCollision::BindOverlappingEvent(function<void(_float3 vDir, CGameObject* pHitActor)> OverlappingEvent)
{
    m_OverlapHitFunc = OverlappingEvent;
}

void CCollision::BindEndOverlapEvent(function<void(_float3 vDir, CGameObject* pHitActor)> EndEvent)
{
    m_EndHitFunc = m_EndHitFunc;
}

void CCollision::ADD_HitObejct(CGameObject* pObject)
{
    auto iter = find(m_HitList.begin(), m_HitList.end(), pObject);
    if (iter == m_HitList.end())
    {
        m_HitList.push_back(pObject);
        Safe_AddRef(pObject);
    }
}

void CCollision::UpdateColiision()
{
    list<CGameObject*> ExitObject;
    for (auto& HitObject : m_HitList)
    {
        auto iter = find(m_OldHitList.begin(), m_OldHitList.end(), HitObject);
        if (iter == m_OldHitList.end())
        {
            if (m_BeginHitFunc)
                m_BeginHitFunc({}, *iter);
        }
        else 
        {
            if (m_OverlapHitFunc)
                m_OverlapHitFunc({}, *iter);
        }
    }

    for (auto& OldHitObject : m_OldHitList)
    {
        auto iter = find(m_HitList.begin(), m_HitList.end(), OldHitObject);
        if (iter == m_OldHitList.end())
        {
            if (m_EndHitFunc)
                m_EndHitFunc({}, *iter);

            Safe_Release(OldHitObject);
        }
    }

    m_OldHitList = m_HitList;
    m_HitList.clear();
}

CComponent* CCollision::Clone(void* pArg)
{
    return nullptr;
}

void CCollision::Free()
{
    __super::Free();

    for (auto& pObject : m_OldHitList)
        Safe_Release(pObject);

    for (auto& pObject : m_HitList)
        Safe_Release(pObject);

    m_OldHitList.clear();
    m_HitList.clear();
}