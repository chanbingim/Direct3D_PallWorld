#pragma once

#include "BaseCamera.h"

NS_BEGIN(Engine)
class ENGINE_DLL CEditorCamera : public CBaseCamera
{
private:
	CEditorCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEditorCamera(const CEditorCamera& rhs);
	virtual ~CEditorCamera() = default;

public:
	//초기화
	virtual		HRESULT				Initalize_Prototype() override;
	virtual		HRESULT				Initialize(void* pArg) override;

	virtual			void			Priority_Update(_float fDeletaTime)override;
	virtual			void			Update(_float fDeletaTime)override;
	virtual			void			Late_Update(_float fDeletaTime)override;

	// 랜더
	virtual			HRESULT			Render()override;

private:
	_float							m_fSpeed = 10.f;
	_float							m_fRotSpeed = 5.f;

private:
	void							Input_KeyBoard(_float fDeletaTime);

public:
	static	CEditorCamera*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
NS_END