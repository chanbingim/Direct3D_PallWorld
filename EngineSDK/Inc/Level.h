#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
class CHeadUpDisplay;

class ENGINE_DLL CLevel : public CBase
{
protected :
	CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID);
	virtual ~CLevel() = default;

public :
	virtual HRESULT		Initialize();
	virtual void		Update(_float fTimeDelta);
	virtual HRESULT		Render();


	const _uInt			GetLevelID() { return m_ID; }
	CHeadUpDisplay*		GetHUD() { return m_pHUD; }

protected :
	_uInt					m_ID = {};
	CGameInstance*			m_pGameInstance = nullptr;
	ID3D11Device*			m_pGraphic_Device = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	CHeadUpDisplay*			m_pHUD = nullptr;

protected :
	void					SetHUD(CHeadUpDisplay* pHUD);

protected :
	virtual void Free() override;
};
NS_END