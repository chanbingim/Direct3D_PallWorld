#include "PellBody.h"

#include "GameInstance.h"

CPellBody::CPellBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPartObject(pDevice, pContext)
{
}

CPellBody::CPellBody(const CPellBody& rhs) :
    CPartObject(rhs)
{
}

HRESULT CPellBody::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPellBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CPellBody::Priority_Update(_float fDeletaTime)
{
}

void CPellBody::Update(_float fDeletaTime)
{
}

void CPellBody::Late_Update(_float fDeletaTime)
{
    
}

HRESULT CPellBody::Render()
{
    return S_OK;
}

void CPellBody::SetAttachSocket(const _float4x4* pSocket, _char SocketFlag)
{
    m_UseSocketMatrixFlag = SocketFlag;
    m_SocketMatrix = pSocket;
}

void CPellBody::PellPlayAnimation(const char* szAnimName, _bool bIsLoop)
{
    if (szAnimName)
    {
        m_iAnimIndex = m_pVIBufferCom->GetNumAnimation(szAnimName);
        m_bIsAnimLoop = bIsLoop;
    }
}

void CPellBody::ResetPellCurrentAnimation()
{
    m_pVIBufferCom->ResetAnimation(0);
}

_bool CPellBody::FinishedAnimation()
{
    return m_bIsAnimFinished;
}

CGameObject* CPellBody::Clone(void* pArg)
{
    return nullptr;
}

void CPellBody::Free()
{
    __super::Free();
}
