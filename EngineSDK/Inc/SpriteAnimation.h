#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CAnimEntity;

class ENGINE_DLL CSpriteAnimation final : public CComponent
{
private:
	CSpriteAnimation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpriteAnimation(const CSpriteAnimation& rhs);
	virtual ~CSpriteAnimation() = default;

public:
	HRESULT						Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);

	void						Update(_uInt AnimIndex);

	void						ADD_Animation(CAnimEntity* pAnim);

	void						Reset_Animation(_uInt AnimIndex);
	const _bool&				AnimIsPlay(_uInt AnimIndex);
	const _bool&				AnimIsFinished(_uInt AnimIndex);
private:
	vector<CAnimEntity*>		m_AnimationList = {};

public:
	static		CSpriteAnimation*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CComponent*			Clone(void* pArg) override;
	virtual		void				Free() override;
};
NS_END