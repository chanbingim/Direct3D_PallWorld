#pragma once

#include "ImgUIBase.h"

NS_BEGIN(Client)
class CIMG_LandScape : public CImgUIBase
{
private:
	CIMG_LandScape();
	virtual ~CIMG_LandScape() = default;

public:
	virtual HRESULT					Prototype_Initialize() override;
	virtual HRESULT					Initialize(void* pArg) override;

	virtual void					Update(_float fDeletaTime) override;

private :
	_uInt							m_QuadPow = {};
	_Int							m_tileCount[2] = {};

	const _char*					m_SelectQuad = "";
	_char							m_szQuadPreview[8][50] = {};

private :
	void							DrawTileCount();
	void							GenerateTerrian();
	void							CreateHeightMap();

public:
	static	CIMG_LandScape*			Create();
	virtual CImgUIBase*				Clone(void* pArg);
	virtual void					Free() override;
};
NS_END