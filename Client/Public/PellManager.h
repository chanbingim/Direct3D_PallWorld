#pragma once
#include "Client_Define.h"
#include "PellStructData.h"
#include "Base.h"

NS_BEGIN(Engine)
class CTexture;
class CGameObject;
NS_END

NS_BEGIN(Client)
class CPellManager final : public CBase
{
	DECLARE_SINGLETON(CPellManager);

private :
	CPellManager();
	virtual ~CPellManager() = default;

public :
	void									Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* szFilePath = "");
	_bool									FindPellData(_uInt iID, PELL_INFO* pPellInfo);

	void									ADD_CombatPalList(CGameObject* pPalObject);
	void									Remove_CombatPalList(CGameObject* pPalObject);
	_bool									IsCombatPal();


private :
	ID3D11Device*							m_pDevice = nullptr; 
	ID3D11DeviceContext*					m_pContext = nullptr;

	unordered_map<_uInt, CTexture*>			m_PellIcons;
	unordered_map<_uInt, PAL_NETWORK_DATA>	m_PellDatas;

	list<CGameObject*>						m_pCombatPalList;

private :
	HRESULT									LoadCSVPellData(const char* szFilePath);
	const CTexture*							CreateTexture(_uInt iID, const WCHAR* szFilePath);

public :
	virtual void							Free() override;

};
NS_END