#include "HeadUpDisplay.h"

#include "GameInstance.h"
#include "UserInterface.h"

CHeadUpDisplay::CHeadUpDisplay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	m_pDevice(pDevice),
	m_pContext(pContext),
	m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CHeadUpDisplay::Initialize()
{
	return S_OK;
}

void CHeadUpDisplay::Update(_float fDeletaTime)
{
	for (auto& pair : m_pUserInterfaceMap)
	{
		pair.second->Update(fDeletaTime);
	}
	
	for (auto& pair : m_pUserInterfaceMap)
	{
		pair.second->Late_Update(fDeletaTime);
	}
}

CUserInterface* CHeadUpDisplay::Find_UserInterface(const _wstring& UITag)
{
	auto pair = m_pUserInterfaceMap.find(UITag);
	if (pair == m_pUserInterfaceMap.end())
		return nullptr;

	return pair->second;
}

void CHeadUpDisplay::Clear_HUD()
{
	for (auto& pair : m_pUserInterfaceMap)
	{
		Safe_Release(pair.second);
	}

	m_pUserInterfaceMap.clear();
}

HRESULT CHeadUpDisplay::Add_UserInterface(_uInt LevelIndex, const _wstring& PrototypeUITag, const _wstring& UITag, void* pArg, CUserInterface** ppOut)
{
	CUserInterface* pUI = m_pGameInstance->Clone_Prototype<CUserInterface>(LevelIndex, PrototypeUITag, pArg);
	if (nullptr == pUI)
		return E_FAIL;

	auto pair = m_pUserInterfaceMap.find(UITag);
	if (pair != m_pUserInterfaceMap.end())
		return E_FAIL;

	if (ppOut)
		*ppOut = pUI;

	m_pUserInterfaceMap.emplace(UITag, pUI);
	return S_OK;
}

HRESULT CHeadUpDisplay::Remove_UserInterface(const _wstring& UITag)
{
	auto pair = m_pUserInterfaceMap.find(UITag);
	if (pair == m_pUserInterfaceMap.end())
		return E_FAIL;

	Safe_Release(pair->second);
	m_pUserInterfaceMap.erase(pair);

	return S_OK;
}

CHeadUpDisplay* CHeadUpDisplay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHeadUpDisplay* pHud = new CHeadUpDisplay(pDevice, pContext);
	if (FAILED(pHud->Initialize()))
	{
		Safe_Release(pHud);
		MSG_BOX("CREATE FAIL : HUD");
	}

	return pHud;
}

void CHeadUpDisplay::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);

	Clear_HUD();
}
