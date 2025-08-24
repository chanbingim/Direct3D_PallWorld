#pragma once

#include "IMG_Component.h"

NS_BEGIN(Client)
class CIMG_Transform final : public CIMG_Component
{
private:
	CIMG_Transform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CIMG_Transform() = default;

public:
	virtual HRESULT					Prototype_Initialize() override;
	virtual HRESULT					Initialize(void* pArg) override;

	virtual void					Update(_float fDeletaTime) override;
	virtual	void					Bind_Data(CGameObject* pOwner, CComponent* pComponent);

private :
	CGameObject*					m_pOwner = nullptr;

	_float3							m_vPostion = {};
	_float3							m_vRotation = {};
	_float3							m_vSacle = {};

private :
	void							DrawTransformUI();


public:
	static	CIMG_Transform*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CImgUIBase*				Clone(void* pArg);
	virtual void					Free() override;
};
NS_END