#pragma once

#include "ImgUIBase.h"

NS_BEGIN(Engine)
class CGameObject;
class CComponent;
NS_END

NS_BEGIN(Client)
class CIMG_Component : public CImgUIBase
{
protected:
	CIMG_Component();
	virtual ~CIMG_Component() = default;

public:
	virtual HRESULT					Prototype_Initialize() override;
	virtual HRESULT					Initialize(void* pArg) override;

	virtual void					Update(_float fDeletaTime) override;
	virtual	void					Bind_Data(CGameObject* pOwner, CComponent* pComponent);

public:
	virtual CImgUIBase*				Clone(void* pArg);
	virtual void					Free() override;
};
NS_END