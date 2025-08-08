#include "DefaultEntity.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "UserInterface.h"

CDefaultEntity::CDefaultEntity() :
    CAnimEntity()
{

}

HRESULT CDefaultEntity::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    Default_Desc* Desc = static_cast<Default_Desc*>(pArg);
    m_pOwner = Desc->pOwner;

    ZeroMemory(m_StartTrasnform, sizeof(_float3) * 3);
    ZeroMemory(m_EndTransform, sizeof(_float3) * 3);
    ZeroMemory(m_ApplyFlag, sizeof(_bool) * 3);

    return S_OK;
}

void CDefaultEntity::Priority_Update()
{
    __super::Priority_Update();
}

void CDefaultEntity::Update()
{
    for (int i = 0; i < 3; ++i)
    {
        if (m_ApplyFlag[i])
        {
            _float3 vResult = {};
            auto vStart = XMLoadFloat3(&m_StartTrasnform[i]);
            auto vEnd = XMLoadFloat3(&m_EndTransform[i]);

            float T = m_fCurFrame / m_fFrame;
            XMStoreFloat3(&vResult, XMVectorLerp(vStart, vEnd, T));
           
            switch (i)
            {
            case 0 :
                m_pOwner->SetScale(vResult);
                break;

            case 1:
                m_pOwner->SetRotation(vResult);
                break;

            case 2:
                m_pOwner->SetLocation(vResult);
                break;
            }
        }
    }

    __super::Update();
}

void CDefaultEntity::Late_Update()
{
    __super::Late_Update();
}

void CDefaultEntity::SetEnitityState(_uInt iType, _float3 vStart, _float3 vEnd)
{
    m_StartTrasnform[iType] = vStart;
    m_EndTransform[iType] = vEnd;
}

void CDefaultEntity::SetFlag(_uInt iFlag)
{
    m_ApplyFlag[iFlag] = true;
}

void CDefaultEntity::Free()
{
    __super::Free();
}
