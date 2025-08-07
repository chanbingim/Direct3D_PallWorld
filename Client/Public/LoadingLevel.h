#pragma once

#include "Client_Define.h"
#include "Level.h"

NS_BEGIN(Client)
class CLoader;

class CLoadingLevel final : public CLevel
{
private:
	CLoadingLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID, LEVEL eNextLevel);
	virtual ~CLoadingLevel() = default;

public:
	virtual HRESULT			Initialize();
	virtual void			Update(_float fTimeDelta);
	virtual HRESULT			Render();

	_bool					IsFinishedLoader();

private:
	LEVEL					m_eNextLevelID = { LEVEL::END };
	CLoader*				m_pLoader = { nullptr };

private :
	HRESULT					Load_Data();

public:
	static	CLoadingLevel*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eID, LEVEL eNextLevel);
	virtual void			Free() override;
};
NS_END