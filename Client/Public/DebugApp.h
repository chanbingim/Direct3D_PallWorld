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

class CDebugApp : public CBase
{
private:
	CDebugApp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CDebugApp() = default;

public:
	HRESULT Initialize_MainApp();
	void	Run(_float fDeletaTime);

private:
	CGameInstance*				m_pGameInstance = nullptr;
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;

	CImgManager*				m_pImgManager = nullptr;

private:
	HRESULT		SetUp_DefaultSetting();
	HRESULT		SetUp_ImgManager();

	void		Update(_float fDeletaTime);
	void		Render();

public:
	static		CDebugApp*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		void		Free() override;

};

NS_END