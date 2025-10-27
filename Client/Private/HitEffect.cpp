#include "HitEffect.h"

#include "GameInstance.h"
#include "EffectContatiner.h"

CHitEffect::CHitEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CSpriteEffect(pDevice, pContext)
{
}

CHitEffect::CHitEffect(const CHitEffect& rhs) :
	CSpriteEffect(rhs)
{
}

HRESULT CHitEffect::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CHitEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	HIT_EFFECT_DESC* pHitDesc = static_cast<HIT_EFFECT_DESC*>(pArg);
	m_fLifeTime = pHitDesc->fLifeTime;
	m_fSpeed = pHitDesc->fSpeed;

	CEffectContatiner::GAMEOBJECT_DESC Desc = {};
	Desc.pParent = this;
	Desc.vScale = { 1.f, 1.f, 1.f };
	auto pGameObject = m_pGameInstance->EffectClone_Object(1, pHitDesc->szEffectName, &Desc);
	m_pEffects.push_back(pGameObject);

	return S_OK;
}

void CHitEffect::Priority_Update(_float fDeletaTime)
{
	__super::Priority_Update(fDeletaTime);
	m_fAccTime += fDeletaTime * m_fSpeed;

	/*if (m_fLifeTime <= m_fAccTime)
		SetDead(true);*/
}

void CHitEffect::Update(_float fDeletaTime)
{
	__super::Update(fDeletaTime);
}

void CHitEffect::Late_Update(_float fDeletaTime)
{
	__super::Late_Update(fDeletaTime);
}

HRESULT CHitEffect::Render()
{
	return S_OK;
}

CHitEffect* CHitEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHitEffect* pHitEffect = new CHitEffect(pDevice, pContext);
	if (FAILED(pHitEffect->Initalize_Prototype()))
	{
		Safe_Release(pHitEffect);
		MSG_BOX("CREATE FAIL : HIT EFFECT");
	}
	return pHitEffect;
}

CGameObject* CHitEffect::Clone(void* pArg)
{
	CHitEffect* pHitEffect = new CHitEffect(*this);
	if (FAILED(pHitEffect->Initialize(pArg)))
	{
		Safe_Release(pHitEffect);
		MSG_BOX("CLONE FAIL : HIT EFFECT");
	}
	return pHitEffect;
}

void CHitEffect::Free()
{
	__super::Free();
}
