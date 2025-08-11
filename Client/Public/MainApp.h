#pragma once

#ifdef _DEBUG
#include "Client_ImgDefines.h"
#endif // _DEBUG

#include "Client_Define.h"
#include "Base.h"

NS_BEGIN(Engine)
	class CGameInstance;
NS_END

NS_BEGIN(Client)
class CImgManager;
class CHeadUpDisplay;

class CMainApp final : public CBase
{
private :
	CMainApp();
	virtual ~CMainApp() = default;

public :
	HRESULT Initialize_MainApp();
	void	Run(_float fDeletaTime);

private :
	CGameInstance*				m_pGameInstance = nullptr;
	ID3D11Device*				m_pGraphic_Device = nullptr;
	ID3D11DeviceContext*		m_pDevice_Context = nullptr;
	_float						m_fDefalutColor[4] = { 0.f, 0.f, 1.f, 1.f };

	CHeadUpDisplay*				m_pHeadUpDisPlay = nullptr;
	ID3D11RasterizerState*		m_pRasterState = nullptr;

	CImgManager*				m_pImgManager = nullptr;

private :
	HRESULT		SetUp_DefaultSetting();
	HRESULT		SetUp_StartLevel(LEVEL eLevelID);
	HRESULT		SetUp_StaticComponents();
	HRESULT		SetUp_CameraSetting();
	HRESULT		SetUp_MouseTexture();

	HRESULT		SetUp_ImgManager();

	void		Update(_float fDeletaTime);
	void		Render();

public :
	static		CMainApp*	Create();
	virtual		void		Free() override;

};
NS_END