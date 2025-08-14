#pragma once

#include "ImgUIBase.h"
class CIMG_Transform final : public CImgUIBase
{
private:
	CIMG_Transform();
	virtual ~CIMG_Transform() = default;

public:
	virtual HRESULT					Prototype_Initialize() override;
	virtual HRESULT					Initialize(void* pArg) override;

	virtual void					Update(_float fDeletaTime) override;


private :
	_float3							m_vPostion;
	_float3							m_vRotation;
	_float3							m_vSacle;

private :
	void							DrawTransformUI();


public:
	static	CIMG_Transform*			Create();
	virtual CImgUIBase*				Clone(void* pArg);
	virtual void					Free() override;
};