#pragma once

#include "Client_Define.h"
#include "Actor.h"

NS_BEGIN(Client)
class CCamera : public CActor
{
public :
	typedef struct CameraDesc : public GAMEOBJECT_DESC
	{
		_float fWidth;
		_float fHegiht;

		_float ffov;
		_float fFar;
		_float fNear;
	}CAMERA_DESC;

private :
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public :
	//√ ±‚»≠
	virtual		HRESULT				Initalize_Prototype() override;
	virtual		HRESULT				Initialize(void* pArg) override;
	void							Update(_float fDeletaTime) override;

	const _float4x4&				GetWorldMatrix();
	const _float4x4&				GetViewMatrix();
	const _float4x4&				GetProjectionMatrix();

private :
	CAMERA_DESC						m_CameraInfo;
	_float							m_fSpeed = 5.f;

	_float4x4						m_ViewMatrix = {};
	_float4x4						m_ProjectionMatrix = {};

private :
	void							Input_KeyBoard(_float fDeletaTime);


public :
	static	CCamera*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
NS_END