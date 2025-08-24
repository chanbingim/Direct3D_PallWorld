#pragma once
#include "Client_ImgDefines.h"
#include "Client_Define.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)
class CImgUIBase abstract : public CBase
{
protected :
	CImgUIBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImgUIBase() = default;

public :
	virtual HRESULT					Prototype_Initialize();
	virtual HRESULT					Initialize(void* pArg);

	virtual void					Update(_float fDeletaTime);
	virtual void					Render();

	void							SetVisibility(VISIBILITY eType);
	const VISIBILITY&				GetVisibility();

protected :
	ID3D11Device*					m_pDevice	= nullptr;
	ID3D11DeviceContext*			m_pContext = nullptr;
	CGameInstance*					m_pGameInstance = nullptr;

	VISIBILITY						m_eVisibility = VISIBILITY::VISIBLE;

public :
	virtual CImgUIBase*				Clone(void* pArg);
	virtual void					Free() override;

};
NS_END