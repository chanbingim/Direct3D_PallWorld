#pragma once
#include "ImgUIBase.h"

NS_BEGIN(Client)
class CIMG_Content : public CImgUIBase
{
private:
	CIMG_Content(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CIMG_Content() = default;

public:
	virtual HRESULT					Prototype_Initialize();
	virtual HRESULT					Initialize(void* pArg);

	virtual void					Update(_float fDeletaTime);

public:
	static		CIMG_Content*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		void				Free() override;

};
NS_END