#include "GameOption.h"

#include "GameInstance.h"
#include "OptionButton.h"

CGameOption::CGameOption(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	CBackGround(pGraphic_Device, pDeviceContext)
{
}

CGameOption::CGameOption(const CGameOption& rhs) :
	CBackGround(rhs)
{
}

HRESULT CGameOption::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameOption::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(ADD_Childs()))
		return E_FAIL;

	m_eType = OBJECT_TYPE::STATIC;
	m_iZOrder = 3;

	return S_OK;
}

void CGameOption::Update(_float fDeletaTime)
{
	if (!m_bIsActive)
		return;

	for (auto& iter : m_pChildList)
		iter->Update(fDeletaTime);
}

void CGameOption::Late_Update(_float fDeletaTime)
{
	if (!m_bIsActive)
		return;

	for (auto& iter : m_pChildList)
		iter->Late_Update(fDeletaTime);
}

HRESULT CGameOption::Render()
{
	return S_OK;
}

void CGameOption::SetActive(_bool flag)
{
	m_bIsActive = flag;
}

_bool CGameOption::IsActive()
{
	return m_bIsActive;
}

HRESULT CGameOption::ADD_Components()
{
	return S_OK;
}

HRESULT CGameOption::ADD_Childs()
{
	COptionButton* pOptionbut = nullptr;

	// 버튼 만들기
	CBackGround::GAMEOBJECT_DESC Desc = {};
	Desc.pParent = this;
	Desc.vScale = { 200.f, 50.f, 0.f };

	_float ButHalfX = Desc.vScale.x * 0.5f + 3.f;
	_float CenterX = m_pTransformCom->GetScale().x * 0.5f - ButHalfX;

	/* CharacterInfo Button */
	Desc.vPosition = { 0, 0.f, 0.f };
	pOptionbut = COptionButton::Create(m_pGraphic_Device, m_pDeviceContext);
	if (FAILED(pOptionbut->Initialize(&Desc)))
		return E_FAIL;
	m_OptionButton.push_back(pOptionbut);
	ADD_Child(pOptionbut);

	/* Option Button */
	Desc.vPosition = { 0, 100.f, 0.f };
	pOptionbut = COptionButton::Create(m_pGraphic_Device, m_pDeviceContext);
	if (FAILED(pOptionbut->Initialize(&Desc)))
		return E_FAIL;
	pOptionbut->Bind_ClickEvent([&]() { GameQuit(); });
	m_OptionButton.push_back(pOptionbut);
	ADD_Child(pOptionbut);

	for (size_t i = 0; i < m_OptionButton.size(); ++i)
	{
		//m_OptionButton[i]->Bind_ClickEvent([&]() {SelectCategoryEvent(iIndex); });
	}
	return S_OK;
}

void CGameOption::GameQuit()
{
	DestroyWindow(g_hWnd);
}

CGameOption* CGameOption::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
	CGameOption* pGameOption = new CGameOption(pGraphic_Device, pDeviceContext);
	if (FAILED(pGameOption->Initalize_Prototype()))
	{
		Safe_Release(pGameOption);
		MSG_BOX("CREATE FAIL : GAME OPTION");
	}

	return pGameOption;
}

CGameObject* CGameOption::Clone(void* pArg)
{
	CGameOption* pGameOption = new CGameOption(*this);
	if (FAILED(pGameOption->Initialize(pArg)))
	{
		Safe_Release(pGameOption);
		MSG_BOX("CLONE FAIL : GAME OPTION");
	}

	return pGameOption;
}

void CGameOption::Free()
{
	__super::Free();

	for (auto& iter : m_OptionButton)
		Safe_Release(iter);
}
