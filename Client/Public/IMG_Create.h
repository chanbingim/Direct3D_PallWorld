#pragma once
#include "ImgUIBase.h"

NS_BEGIN(Engine)
class CGameObject;
NS_END

NS_BEGIN(Client)
class CIMG_Create : public CImgUIBase
{
private:
	CIMG_Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CIMG_Create() = default;

public:
	virtual HRESULT					Prototype_Initialize() override;
	virtual HRESULT					Initialize(void* pArg) override;

	virtual void					Update(_float fDeletaTime) override;

private :
	char							m_szSelectTag[MAX_PATH] = {};
	char							m_szSelectPrototype[MAX_PATH] = {};
	const char*						m_SzSelectTagList[2] = { "STATIC", "CURRENT LEVEL" };

	list<pair<_wstring, CGameObject*>>			m_Prototypes;

	_uInt							m_SelectLevel = {};

	char							m_szObjectName[MAX_PATH] = {};
	char							m_szLayerName[MAX_PATH] = {};
	_Int							m_iCreateCnt = {};

	_Int							m_iEnvIndex = {};
	_bool							m_bCrateEnv = {};

private :
	void							DrawCreateCategory();
	void							DrawPrototypes();

public:
	static	CIMG_Create*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CImgUIBase*				Clone(void* pArg);
	virtual void					Free() override;
};
NS_END