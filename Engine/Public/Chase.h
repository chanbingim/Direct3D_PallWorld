#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CChase : public CComponent
{
public :
	typedef struct ChaseDesc
	{
		_float3			vStartPoint;
		_float3			vTargetPoint;
		_float			fChaseTime;
	}CHASE_DESC;

private :
	CChase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChase(const CChase& rhs);
	virtual ~CChase() = default;

public :
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);

	_float3						ComputeLerpPoint(_float fDeletaTime);

	void						SetChase(const CHASE_DESC& ChaseInfo);
	_bool						IsEndChase();

private :
	_bool						m_bIsChase = false;

	_float						m_fTickPerAcc = {};
	_float						m_fAccTime = {};

	_float3						m_vStartPoint = {};
	_float3						m_vTargetPoint = {};
	_float						m_fChaseTime = {};

public:
	static		CChase*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CComponent*		Clone(void* pArg) override;
	virtual		void			Free() override;

};
NS_END