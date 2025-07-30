#pragma once

#include "Client_Define.h"
#include "Level.h"

NS_BEGIN(Client)
class CLogoLevel final : public CLevel
{
private :
	CLogoLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID);
	virtual ~CLogoLevel() = default;

public:
	virtual HRESULT		Initialize();
	virtual void		Update(_float fTimeDelta);
	virtual HRESULT		Render();

public :
	static	CLogoLevel*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eID);
	virtual void			Free() override;
};
NS_END