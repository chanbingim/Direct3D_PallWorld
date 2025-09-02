#include "PlayerLowerBody.h"

CPlayerLowerBody::CPlayerLowerBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
}

CPlayerLowerBody::CPlayerLowerBody(const CPlayerLowerBody& rhs)
{
}

HRESULT CPlayerLowerBody::Initalize_Prototype()
{
    return E_NOTIMPL;
}

HRESULT CPlayerLowerBody::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

void CPlayerLowerBody::Priority_Update(_float fDeletaTime)
{
}

void CPlayerLowerBody::Update(_float fDeletaTime)
{
}

void CPlayerLowerBody::Late_Update(_float fDeletaTime)
{
}

HRESULT CPlayerLowerBody::Render()
{
    return E_NOTIMPL;
}

CPlayerLowerBody* CPlayerLowerBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    return nullptr;
}

CGameObject* CPlayerLowerBody::Clone(void* pArg)
{
    return nullptr;
}

void CPlayerLowerBody::Free()
{
}
