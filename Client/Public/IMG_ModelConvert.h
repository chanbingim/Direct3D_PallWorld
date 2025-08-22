#pragma once
#include "ImgUIBase.h"

NS_BEGIN(Client)
class CIMG_ModelConvert : public CImgUIBase
{
private :
	CIMG_ModelConvert();
	virtual ~CIMG_ModelConvert() = default;
	
public:
	virtual HRESULT							Prototype_Initialize();
	virtual HRESULT							Initialize(void* pArg);

	virtual void							Update(_float fDeletaTime);

public :
	static			CIMG_ModelConvert*		Create();
	virtual			void					Free() override;

};
NS_END