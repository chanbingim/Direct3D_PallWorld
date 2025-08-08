#include "SpriteAnimation.h"

#include "GameInstance.h"
#include "AnimEntity.h"

CSpriteAnimation::CSpriteAnimation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CComponent(pDevice, pContext)
{
}

CSpriteAnimation::CSpriteAnimation(const CSpriteAnimation& rhs) :
	CComponent(rhs)
{
}

HRESULT CSpriteAnimation::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpriteAnimation::Initialize(void* pArg)
{
	return S_OK;
}

void CSpriteAnimation::Update(_uInt AnimIndex)
{
	if (m_AnimationList.size() < AnimIndex || 0 > AnimIndex)
		return;

	m_AnimationList[AnimIndex]->Update();
}

void CSpriteAnimation::ADD_Animation(CAnimEntity* pAnim)
{
	m_AnimationList.push_back(pAnim);
}

void CSpriteAnimation::Reset_Animation(_uInt AnimIndex)
{
	if (m_AnimationList.size() < AnimIndex || 0 > AnimIndex)
		return;

	m_AnimationList[AnimIndex]->ReplayAnimation();
}

const _bool& CSpriteAnimation::AnimIsPlay(_uInt AnimIndex)
{
	if (m_AnimationList.size() < AnimIndex || 0 > AnimIndex)
		return false;

	return m_AnimationList[AnimIndex]->IsPlay();
}

const _bool& CSpriteAnimation::AnimIsFinished(_uInt AnimIndex)
{
	if (m_AnimationList.size() < AnimIndex || 0 > AnimIndex)
		return false;

	return m_AnimationList[AnimIndex]->IsFinished();
}

CSpriteAnimation* CSpriteAnimation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSpriteAnimation* pSpriteAnimCom = new CSpriteAnimation(pDevice, pContext);
	if (FAILED(pSpriteAnimCom->Initialize_Prototype()))
	{
		Safe_Release(pSpriteAnimCom);
		MSG_BOX("CREATE FAIL : SPRITE ANIM");
	}

	return pSpriteAnimCom;
}

CComponent* CSpriteAnimation::Clone(void* pArg)
{
	CSpriteAnimation* pSpriteAnimCom = new CSpriteAnimation(*this);
	if (FAILED(pSpriteAnimCom->Initialize(pArg)))
	{
		Safe_Release(pSpriteAnimCom);
		MSG_BOX("CLONE FAIL : SPRITE ANIM");
	}

	return pSpriteAnimCom;
}

void CSpriteAnimation::Free()
{
	for (auto& iter : m_AnimationList)
		Safe_Release(iter);

	m_AnimationList.clear();
}
