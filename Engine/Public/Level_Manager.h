#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
class CLevel;

class CLevel_Manager final : public CBase
{
private :
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public :
	HRESULT			Initialize();
	HRESULT			Change_Level(class CLevel* pNewLevel);
	void			Update(_float fTimeDelta);
	HRESULT			Render();

	CLevel*			GetCurrentLevel();

private:
	CLevel*			m_pCurrentLevel = { nullptr };
	CGameInstance*	m_pGameInstance = { nullptr };

public:
	static CLevel_Manager*	Create();
	virtual void			Free() override;

};
NS_END