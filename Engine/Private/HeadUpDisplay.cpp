#include "HeadUpDisplay.h"

#include "UserInterface.h"

CHeadUpDisplay::CHeadUpDisplay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	m_pDevice(pDevice),
	m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
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

	Clear_HUD();

}
