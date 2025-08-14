#pragma once
#include "ImgUIBase.h"

NS_BEGIN(Client)
class CIMG_Create : public CImgUIBase
{
private:
	CIMG_Create();
	virtual ~CIMG_Create() = default;

public:
	virtual HRESULT					Prototype_Initialize() override;
	virtual HRESULT					Initialize(void* pArg) override;

	virtual void					Update(_float fDeletaTime) override;

public:
	static	CIMG_Create*			Create();
	virtual CImgUIBase*				Clone(void* pArg);
	virtual void					Free() override;
};
NS_END