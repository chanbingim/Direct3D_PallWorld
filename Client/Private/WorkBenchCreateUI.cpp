#include "WorkBenchCreateUI.h"

#include "GameInstance.h"
#include "TitleUI.h"
#include "CreateToolTipUI.h"

CWorkBenchCreateUI::CWorkBenchCreateUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CWorkBenchCreateUI::CWorkBenchCreateUI(const CWorkBenchCreateUI& rhs) :
	CBackGround(rhs)
{
}

HRESULT CWorkBenchCreateUI::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWorkBenchCreateUI::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_iZOrder = 3;
	return S_OK;
}

void CWorkBenchCreateUI::Update(_float fDeletaTime)
{
	for (auto pChild : m_pChildList)
		pChild->Update(fDeletaTime);
}

void CWorkBenchCreateUI::Late_Update(_float fDeletaTime)
{
	for (auto pChild : m_pChildList)
		pChild->Late_Update(fDeletaTime);

	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CWorkBenchCreateUI::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(1);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

HRESULT CWorkBenchCreateUI::ADD_Components()
{
	_float3 vScale = m_pTransformCom->GetScale();

	CTitleUI::TITLE_UI_DESC TitleDesc = {};
	TitleDesc.pParent = this;
	TitleDesc.vScale = { vScale.x * 0.5f, 20.f, 0.f };
	TitleDesc.vPosition = { -(vScale.x * 0.5f) + TitleDesc.vScale.x * 0.5f,
							-(vScale.y * 0.5f) + TitleDesc.vScale.y * 0.5f, 0.f};
	TitleDesc.szTitleName = TEXT("원시적인 작업대");
	m_pTitle = static_cast<CTitleUI*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Base_Title_UI"), &TitleDesc));
	m_pTitle->SetZOrder(m_iZOrder + 1);
	ADD_Child(m_pTitle);

	CCreateToolTipUI::GAMEOBJECT_DESC	CreatToolTipDesc = {};
	TitleDesc.pParent = this;
	TitleDesc.vScale = { vScale.x * 0.5f, vScale.y, 0.f };
	TitleDesc.vPosition = { (vScale.x * 0.5f) - TitleDesc.vScale.x * 0.5f,
							0.f, 0.f };
	m_pCreateToolTip = static_cast<CCreateToolTipUI*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Workbench_ToolTip_UI"), &CreatToolTipDesc));
	m_pCreateToolTip->SetZOrder(m_iZOrder + 1);
	ADD_Child(m_pCreateToolTip);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_PellInfo_Background"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CWorkBenchCreateUI* CWorkBenchCreateUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWorkBenchCreateUI* pWorkBenchUI = new CWorkBenchCreateUI(pDevice, pContext);
	if (FAILED(pWorkBenchUI->Initalize_Prototype()))
	{
		Safe_Release(pWorkBenchUI);
		MSG_BOX("CREATE FAIL : WORK BENCH");
	}
	return pWorkBenchUI;
}

CGameObject* CWorkBenchCreateUI::Clone(void* pArg)
{
	CWorkBenchCreateUI* pWorkBenchUI = new CWorkBenchCreateUI(*this);
	if (FAILED(pWorkBenchUI->Initialize(pArg)))
	{
		Safe_Release(pWorkBenchUI);
		MSG_BOX("CLONE FAIL : WORK BENCH");
	}
	return pWorkBenchUI;
}

void CWorkBenchCreateUI::Free()
{
	__super::Free();
}
