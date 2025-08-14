#pragma once
#include "ImgUIBase.h"

NS_BEGIN(Client)
class CIMG_Inspector : public CImgUIBase
{
private:
	CIMG_Inspector();
	virtual ~CIMG_Inspector() = default;

public:
	virtual HRESULT					Prototype_Initialize() override;
	virtual HRESULT					Initialize(void* pArg) override;

	virtual void					Update(_float fDeletaTime) override;

public:
	static	CIMG_Inspector*			Create();
	virtual CImgUIBase*				Clone(void* pArg);
	virtual void					Free() override;

};
NS_END