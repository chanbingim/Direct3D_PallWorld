#pragma once
#include "ImgUIBase.h"

NS_BEGIN(Engine)
class CGameObject;
class CEditorCamera;
NS_END

NS_BEGIN(Client)


class CIMG_ModelConvert : public CImgUIBase
{
private :
	CIMG_ModelConvert(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CIMG_ModelConvert() = default;
	
public:
	virtual HRESULT							Prototype_Initialize();
	virtual HRESULT							Initialize(void* pArg);

	virtual void							Update(_float fDeletaTime);
	virtual void							Render();
	
private :
	void									UpdateCurrentModelList();
	void									DrawConvertUI();
	void									UpdateSelect();


private :
	list<CGameObject*>						m_ShowGameObject = {};
	CGameObject*							m_pSelectObejct = nullptr;
	CEditorCamera*							m_pModelCamera = nullptr;

	char									m_SelectObejctName[MAX_PATH] = {};
	char									m_showObejctName[MAX_PATH] = {};
	char									m_SaveFilePath[MAX_PATH] = {};

private :
	HRESULT									ADD_ModelViewCameraLayer();

public :
	static			CIMG_ModelConvert*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			void					Free() override;

};
NS_END