#pragma once
#include "Client_Define.h"
#include "BaseCamera.h"

NS_BEGIN(Client)
class CPlayerCamera : public CBaseCamera
{
private:
	CPlayerCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerCamera(const CPlayerCamera& rhs);
	virtual ~CPlayerCamera() = default;

public:
	//초기화
	virtual		HRESULT				Initalize_Prototype() override;
	virtual		HRESULT				Initialize(void* pArg) override;

	virtual			void			Priority_Update(_float fDeletaTime)override;
	virtual			void			Update(_float fDeletaTime)override;
	virtual			void			Late_Update(_float fDeletaTime)override;

	// 랜더
	virtual			HRESULT			Render()override;
	void							AttachCamera(_vector BonesTransformation);

private:
	_float							m_fRotSpeed = 5.f;
	_float4x4						m_CombinedMatrix = {};
	_float4x4						m_InvCombinedMatrix = {};
public:
	static	CPlayerCamera*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
NS_END