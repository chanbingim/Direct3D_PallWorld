#pragma once

#include "ImgUIBase.h"

NS_BEGIN(Client)
class CIMG_LandScape : public CImgUIBase
{
private:
	CIMG_LandScape(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
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

	// 지형 픽킹을 위한 데이터
	_bool							m_bIsPrefabBursh = false;
	_Int							m_iBrushSize = {};
	_Int							m_iPrefabIndex = {};
	_char							m_szPrafeName[MAX_PATH] = {};
	_char							m_szLayerName[MAX_PATH] = {};

private :
	void							DrawTileCount();
	void							GenerateTerrian();
	void							DrawPrefabBrush();
	void							CreateHeightMapToPng();

public:
	static	CIMG_LandScape*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CImgUIBase*				Clone(void* pArg);
	virtual void					Free() override;
};
NS_END