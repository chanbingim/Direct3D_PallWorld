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
	CImgUIBase();
	virtual ~CImgUIBase() = default;

public :
	virtual HRESULT					Prototype_Initialize();
	virtual HRESULT					Initialize(void* pArg);

	virtual void					Update(_float fDeletaTime);

	void							SetVisibility(VISIBILITY eType);
	const VISIBILITY&				GetVisibility();

protected :
	CGameInstance*					m_pGameInstance = nullptr;
	VISIBILITY						m_eVisibility = VISIBILITY::VISIBLE;

public :
	virtual CImgUIBase*				Clone(void* pArg);
	virtual void					Free() override;

};
NS_END