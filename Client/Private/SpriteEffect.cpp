#include "SpriteEffect.h"

CSpriteEffect::CSpriteEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CGameObject(pDevice, pContext)
{
}

CSpriteEffect::CSpriteEffect(const CSpriteEffect& rhs) :
    CGameObject(rhs)
{
}

HRESULT CSpriteEffect::Initalize_Prototype()
{


    return S_OK;
}

HRESULT CSpriteEffect::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CSpriteEffect::Priority_Update(_float fDeletaTime)
{
    for (auto pEffect : m_pEffects)
        pEffect->Priority_Update(fDeletaTime);
}

void CSpriteEffect::Update(_float fDeletaTime)
{
    for (auto pEffect : m_pEffects)
        pEffect->Update(fDeletaTime);
}

void CSpriteEffect::Late_Update(_float fDeletaTime)
{
    for (auto pEffect : m_pEffects)
        pEffect->Late_Update(fDeletaTime);
}

HRESULT CSpriteEffect::Render()
{
    return S_OK;
}

CGameObject* CSpriteEffect::Clone(void* pArg)
{
    return nullptr;
}

void CSpriteEffect::Free()
{
    __super::Free();

    for (auto pEffect : m_pEffects)
        Safe_Release(pEffect);
    m_pEffects.clear();
}
