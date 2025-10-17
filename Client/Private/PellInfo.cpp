#include "PellInfo.h"

#include "GameInstance.h"
#include "GamePlayHUD.h"

CPellInfo::CPellInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : 
	CBackGround(pDevice, pContext)
{
}

CPellInfo::CPellInfo(const CPellInfo& rhs) :
	CBackGround(rhs)
{
}

HRESULT CPellInfo::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPellInfo::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(ADD_Childs()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_eType = OBJECT_TYPE::STATIC;
	return S_OK;
}

void CPellInfo::Update(_float fDeletaTime)
{
}

void CPellInfo::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CPellInfo::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(2);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

HRESULT CPellInfo::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_PellInfo_Background"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPellInfo::ADD_Childs()
{
	auto pDefaultHUD = m_pGameInstance->GetCurrentHUD();
	if (nullptr == pDefaultHUD)
		return E_FAIL;

	auto pInGame_HUD = dynamic_cast<CGamePlayHUD*>(pDefaultHUD);
	if (pInGame_HUD)
	{
		CBackGround::GAMEOBJECT_DESC Desc = {};
		Desc.pParent = this;
		Desc.vScale = { 30.f, 30.f, 0.f };

		//Type Icon
		Desc.vPosition = { -50.f, 0.f, 0.f };
		if (FAILED(pInGame_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Battle_Pell_TypeUI"), TEXT("TpyeIcon"), &Desc)))
			return E_FAIL;
	}

	return S_OK;
}

CPellInfo* CPellInfo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPellInfo* pPellInfo = new CPellInfo(pDevice, pContext);
	if (FAILED(pPellInfo->Initalize_Prototype()))
	{
		Safe_Release(pPellInfo);
		MSG_BOX("CREATE FAIL : Pell Info UI");
	}

	return pPellInfo;
}

CGameObject* CPellInfo::Clone(void* pArg)
{
	CPellInfo* pPellInfo = new CPellInfo(*this);
	if (FAILED(pPellInfo->Initialize(pArg)))
	{
		Safe_Release(pPellInfo);
		MSG_BOX("CLONE FAIL : Pell Info UI");
	}

	return pPellInfo;
}

void CPellInfo::Free()
{
	__super::Free();
}
