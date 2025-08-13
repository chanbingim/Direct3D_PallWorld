#pragma once
#include "ImgUIBase.h"

NS_BEGIN(Client)
class CIMG_Profiler final : public CImgUIBase
{
private:
	CIMG_Profiler();
	virtual ~CIMG_Profiler() = default;

public:
	virtual HRESULT					Prototype_Initialize() override;
	virtual HRESULT					Initialize(void* pArg) override;

	virtual void					Update(_float fDeletaTime) override;

private :
	char							m_szPercent[MAX_PATH];

	//시간을 가져와서 IMG 세팅
	_float							m_fDurTime[5];

private :
	const char*					GetProgressName(_uInt iIndex);


public:
	static	CIMG_Profiler*			Create();
	virtual CImgUIBase*				Clone(void* pArg);
	virtual void					Free() override;

};
NS_END