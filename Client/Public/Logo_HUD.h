#pragma once
#include "Client_Define.h"
#include "HeadUpDisplay.h"

NS_BEGIN(Client)
class CLogo_HUD final : public CHeadUpDisplay
{
protected:
	CLogo_HUD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLogo_HUD() = default;

public:
	virtual HRESULT				Initialize() override;
	virtual void				Update(_float fDeletaTime) override;

private :
	HRESULT						ADD_UserInterface();

public:
	static	CLogo_HUD*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void				Free() override;
};
NS_END