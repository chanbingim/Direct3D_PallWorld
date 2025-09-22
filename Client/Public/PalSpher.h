#pragma once

#include "ProjectileObject.h"

NS_BEGIN(Client)

class CPellBase;

class CPalSpher : public CProjectileObject
{
private :
	CPalSpher(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPalSpher(const CPalSpher& rhs);
	virtual ~CPalSpher() = default;
	
public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;

private :
	/* 펠과 충돌 됐을때 사용할 변수 목록 */
	CPellBase*								m_pHitPell = nullptr;
	_float									m_fAccTime = {};

	_float									m_fCurComputeTime = {};
	_uInt									m_iAccCount = {};
	_float									m_fAccPercent = {};

	_float3									m_HitStartPoint = {};
	_float3									m_ReflectPoint = {};
	_float3									m_ReflectDir = {};
private :
	HRESULT									ADD_Components();
	void									BeginOverlapEvent(_float3 vDir, CGameObject* pTarget);

	void									ComputeCatchPellSuccess(_float fDeletaTime);
	void									ViewPellCatchUI();

public :
	static		CPalSpher*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;
};
NS_END