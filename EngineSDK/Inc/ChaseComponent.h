#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CTransform;

class ENGINE_DLL CChaseComponent final : public CComponent
{
public :
	typedef struct ChaseInitDesc
	{
		const CTransform*			pOwnerTransform;
	}CHASE_INITIALIZE_DESC;

	typedef struct ChaseDesc
	{
		const CTransform*			pTargetTransform;
		const _float*				fChaseSpeed; 
	}CHASE_DESC;

private :
	CChaseComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChaseComponent(const CChaseComponent& rhs);
	virtual ~CChaseComponent() = default;

public :
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);

	void						ComputeLerpPoint(_float fDeletaTime, _float3& vOutDir, _float3& vOutMovePoint);

	void						SetChase(const CHASE_DESC& ChaseInfo);
	_bool						IsEndChase();

private :
	const CTransform*			m_pOwnerTransform = nullptr;
	const CTransform*			m_pTargetTransform = nullptr;

	_bool						m_bIsChase = false;
	const _float*				m_fChaseSpeed = nullptr;

public:
	static		CChaseComponent*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CComponent*					Clone(void* pArg) override;
	virtual		void						Free() override;

};
NS_END