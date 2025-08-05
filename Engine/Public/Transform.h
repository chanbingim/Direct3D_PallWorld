#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CTransform final : public CComponent
{
private :
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public :
	_vector						GetRightVector();
	_vector						GetUpVector();
	_vector						GetLookVector();

	_float3						GetPosition();
	_float3						GetScale();
	_float3						GetRotation();

	void						RotationAxis(_float3 vAxis, _float fAngle);
	void						SetPosition(_float3 vPosition);
	void						SetScale(_float3 vScale);
	void						SetRotation(_float3 vRotation);

	_float4x4&					GetWorldMat();
	_float4x4&					GetInvWorldMat();

	void						SetParentMatrix(const _float4x4& matrix);

	void						ADD_Position(_vector vAddPos);
	void						Turn(_vector vAxis, _float fTurnSpeed, _float fTimeDeleta);

	void						LookAt(_vector vAt);
private :
	_bool						m_bHasParent = false;
	_float4x4					m_ParentWorldMat;
	_float4x4					m_WorldMat;
	_float4x4					m_InvWorldMat;

	_float3						m_vRotation = {};


public :
	static		CTransform*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CComponent*		Clone(void* pArg) override;
	virtual		void			Free() override;

};
NS_END