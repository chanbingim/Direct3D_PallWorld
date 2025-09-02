#include "PlayerUpperBody.h"

CPlayerUpperBody::CPlayerUpperBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
}

CPlayerUpperBody::CPlayerUpperBody(const CPlayerUpperBody& rhs)
{
}

HRESULT CPlayerUpperBody::Initalize_Prototype()
{
    return E_NOTIMPL;
}

HRESULT CPlayerUpperBody::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

void CPlayerUpperBody::Priority_Update(_float fDeletaTime)
{
}

void CPlayerUpperBody::Update(_float fDeletaTime)
{
}

void CPlayerUpperBody::Late_Update(_float fDeletaTime)
{
}

HRESULT CPlayerUpperBody::Render()
{
    return E_NOTIMPL;
}

CPlayerUpperBody* CPlayerUpperBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    return nullptr;
}

CGameObject* CPlayerUpperBody::Clone(void* pArg)
{
    return nullptr;
}

void CPlayerUpperBody::Free()
{
}
