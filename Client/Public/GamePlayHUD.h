#pragma once

#include "Client_Define.h"
#include "HeadUpDisplay.h"

NS_BEGIN(Client)
class CGamePlayHUD final : public CHeadUpDisplay
{
protected:
	CGamePlayHUD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CGamePlayHUD() = default;

public:
	virtual HRESULT				Initialize() override;
	virtual void				Update(_float fDeletaTime) override;

private:
	HRESULT						ADD_UserInterface();

public:
	static	CGamePlayHUD*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void				Free() override;
};
NS_END