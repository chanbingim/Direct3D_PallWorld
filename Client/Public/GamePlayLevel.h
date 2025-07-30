#pragma once

#include "Client_Define.h"
#include "Level.h"

NS_BEGIN(Client)
class CGamePlayLevel final : public CLevel
{
private:
	CGamePlayLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID);
	virtual ~CGamePlayLevel() = default;

public:
	virtual HRESULT			Initialize();
	virtual void			Update(_float fTimeDelta);
	virtual HRESULT			Render();

public:
	static	CGamePlayLevel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eID);
	virtual void			Free() override;
};
NS_END