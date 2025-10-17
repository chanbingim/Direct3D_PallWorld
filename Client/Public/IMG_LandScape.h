#pragma once

#include "ImgUIBase.h"
#include "NavigationStruct.h"

NS_BEGIN(Client)
class CIMG_LandScape : public CImgUIBase
{
public :
	enum class BRUSH_MODE { SELECT_TERRIAN, EDIT_ENVIORNMENT, ADD_HEGIHT, EDIT_NAVIMESH, END };
	static const char* szBrushMode[ENUM_CLASS(BRUSH_MODE::END)];

private:
	CIMG_LandScape(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CIMG_LandScape() = default;

public:
	virtual HRESULT					Prototype_Initialize() override;
	virtual HRESULT					Initialize(void* pArg) override;

	virtual void					Update(_float fDeletaTime) override;
	BRUSH_MODE						GetBurshMode() { return m_eBrushMode; }
	_uInt							GetBurshSize() { return m_iBrushSize; }
	_bool							GetbIsNaviMeshPicking();

private :
	_uInt							m_QuadPow = {};
	_Int							m_tileCount[2] = {};

	const _char*					m_SelectQuad = "";
	_char							m_szQuadPreview[8][50] = {};

	// 지형 픽킹을 위한 데이터
	_bool							m_bIsPrefabBursh = false;
	_bool							m_bPickingNavimesh = false;

	_char							m_szBushModeName[MAX_PATH] = {};
	BRUSH_MODE						m_eBrushMode = {};
	_Int							m_iBrushSize = {};
	_Int							m_iPrefabIndex = {};
	_char							m_szPrafeName[MAX_PATH] = {};
	_char							m_szLayerName[MAX_PATH] = {};

	// 네비메시 저장및 삭제 새로만들기
	_char							m_szNaviPath[MAX_PATH] = {};

private :
	void							DrawTileCount();
	void							LoadNewNaviMesh();
	void							GenerateTerrian();
	void							DrawPrefabBrush();
	void							CreateHeightMapToPng();

	void							Save_NaviMesh();
	HRESULT							ExportNaviMeshData(const char* pFilePath, list<NAVI_TRIANGLE>& SaveList);

public:
	static	CIMG_LandScape*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CImgUIBase*				Clone(void* pArg);
	virtual void					Free() override;
};
NS_END