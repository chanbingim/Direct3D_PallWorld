#pragma once
#include "ImgUIBase.h"
#include "EffectStruct.h"

NS_BEGIN(Engine)
class CGameObject;
class CEffectPartObject;
class CEffectContatiner;
class CEditorCamera;
NS_END

NS_BEGIN(Client)
class CIMG_EffectTool : public CImgUIBase
{
public :
	enum class IMG_EFFECT_SHOW_TYPE { EFFECT_PART_OBJECT, EFFECT_CONTATINER, END };
	enum class IMG_EFFECT_TEXTURE_TYPE { DIFFUSE, NORMAL, DISTOTION, MASK,END };

	static const char*		szShowType[ENUM_CLASS(IMG_EFFECT_SHOW_TYPE::END)];
	static const char*		szEffectTexType[ENUM_CLASS(IMG_EFFECT_TEXTURE_TYPE::END)];
	static const char*		szEffectType[ENUM_CLASS(EFFECT_TYPE::END)];
	static const char*		szBlendType[ENUM_CLASS(EFFECT_BELND_MODE::END)];
	static const char*		szDistotionType[ENUM_CLASS(EFFECT_DISTOTION_TYPE::END)];
	static const char*		szMaskType[ENUM_CLASS(EFFECT_MASK_TYPE::END)];

private:
	CIMG_EffectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CIMG_EffectTool() = default;

public:
	virtual HRESULT							Prototype_Initialize();
	virtual HRESULT							Initialize(void* pArg);

	virtual void							Update(_float fDeletaTime);
	virtual void							Render();

private:
	CEditorCamera*								m_pModelCamera = nullptr;
	IMG_EFFECT_SHOW_TYPE						m_eShowType = {};
	char										m_CreateEffectName[MAX_PATH] = {};
	char										m_SaveFilePath[MAX_PATH] = {};
	char										m_PreVeiwEffectObject[MAX_PATH] = {};
	char										m_SelectShowType[24] = {};

	list<pair<_wstring, CEffectContatiner*>>	m_ShowEffectObject = {};
	list<pair<_wstring, CEffectPartObject*>>	m_ShowEffectPartObject = {};
	CGameObject*								m_pSelectObject = nullptr;

	char										m_SelectAddPartEffect[MAX_PATH] = {};
	char										m_SelectObejctName[MAX_PATH] = {};
	char										m_showObejctName[MAX_PATH] = {};

	EFFECT_NETWORK_DESC							m_EffectDesc = {};
	IMG_EFFECT_TEXTURE_TYPE						m_eEffectTexType = {};
	char										m_SelectTextureType[24] = {};

	char										m_SelectType[24] = {};
	char										m_ShaderSelect[MAX_PATH] = {};
	char										m_ModelSelect[MAX_PATH] = {};

	char										m_SelectBlendMode[24] = {};
	char										m_SelectDistotionMode[24] = {};
	char										m_SelectMaskMode[24] = {};

	char										m_ConvertChar[MAX_PATH] = {};

	_float										m_UVRatetime[2] = {};
	_float										m_vColor[4] = {};

	_float										m_vPosition[3] = {};
	_float										m_vRotation[3] = {};
	_float										m_vScale[3] = {};

	_float										m_vLerpEndPosition[3] = {};
	_float										m_vLerpEndRotation[3] = {};
	_float										m_vLerpEndScale[3] = {};


private:
	HRESULT										ADD_ModelViewCameraLayer();

	void										EffectSelect();
	void										EffectViewer();

	void										EffectTextureViewer();
	void										EffectDataEditor();

public:
	static			CIMG_EffectTool*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			void						Free() override;

};
NS_END