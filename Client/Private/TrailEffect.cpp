#include "TrailEffect.h"

CTrailEffect::CTrailEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CSpriteEffect(pDevice, pContext)
{
}

CTrailEffect::CTrailEffect(const CTrailEffect& rhs) :
    CSpriteEffect(rhs)
{
}

HRESULT CTrailEffect::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CTrailEffect::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CTrailEffect::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
}

void CTrailEffect::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CTrailEffect::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CTrailEffect::Render()
{
    return S_OK;
}

CTrailEffect* CTrailEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTrailEffect* pTrailEffect = new CTrailEffect(pDevice, pContext);
    if (FAILED(pTrailEffect->Initalize_Prototype()))
    {
        Safe_Release(pTrailEffect);
        MSG_BOX("CREATE FAIL : TRAIL EFFECT");
    }
    return pTrailEffect;
}

CGameObject* CTrailEffect::Clone(void* pArg)
{
    CTrailEffect* pTrailEffect = new CTrailEffect(*this);
    if (FAILED(pTrailEffect->Initialize(pArg)))
    {
        Safe_Release(pTrailEffect);
        MSG_BOX("CLONE FAIL : TRAIL EFFECT");
    }
    return pTrailEffect;
}

void CTrailEffect::Free()
{
    __super::Free();
}
