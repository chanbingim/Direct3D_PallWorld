#pragma once

#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
class CUserInterface;

class ENGINE_DLL CHeadUpDisplay : public CBase
{
protected:
	CHeadUpDisplay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CHeadUpDisplay() = default;
	
public :
	virtual HRESULT				Initialize();
	virtual void				Update(_float fDeletaTime);
	
	CUserInterface*				Find_UserInterface(const _wstring& UITag);

	void						Clear_HUD();

protected :
	unordered_map<_wstring, CUserInterface*>		m_pUserInterfaceMap;

	//기본 변수들 바꾸지마라
	CGameInstance*									m_pGameInstance = nullptr;
	ID3D11Device*									m_pDevice = nullptr;
	ID3D11DeviceContext*							m_pContext = nullptr;

protected :
	HRESULT						Add_UserInterface(_uInt LevelIndex, const _wstring& PrototypeUITag, const _wstring& UITag, void* pArg = nullptr);

public :
	static	CHeadUpDisplay*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void				Free() override;

};
NS_END