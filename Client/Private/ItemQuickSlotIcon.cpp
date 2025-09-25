#include "ItemQuickSlotIcon.h"

#include "GameInstance.h"

CItemQuickSlotIcon::CItemQuickSlotIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CItemQuickSlotIcon::CItemQuickSlotIcon(const CItemQuickSlotIcon& rhs) :
	CBackGround(rhs)
{
}

HRESULT CItemQuickSlotIcon::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItemQuickSlotIcon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

void CItemQuickSlotIcon::Update(_float fDeletaTime)
{
}

void CItemQuickSlotIcon::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CItemQuickSlotIcon::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(0);
	m_pTextureCom->SetTexture(0, m_iWeaponTypeIcon);
	m_pVIBufferCom->Render_VIBuffer();
	return S_OK;
}

void CItemQuickSlotIcon::SetWeaponTypeIcon(WEAPON eType)
{
	m_iWeaponTypeIcon = ENUM_CLASS(eType);
}

HRESULT CItemQuickSlotIcon::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Pell_HeatlhBar_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_ProgressBar"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CItemQuickSlotIcon* CItemQuickSlotIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItemQuickSlotIcon* pQuickSlotIcon = new CItemQuickSlotIcon(pDevice, pContext);
	if (FAILED(pQuickSlotIcon->Initalize_Prototype()))
	{
		Safe_Release(pQuickSlotIcon);
		MSG_BOX("CREATE FAIL : QUICK SLOT ICON");
	}
	return pQuickSlotIcon;
}

CGameObject* CItemQuickSlotIcon::Clone(void* pArg)
{
	CItemQuickSlotIcon* pQuickSlotIcon = new CItemQuickSlotIcon(*this);
	if (FAILED(pQuickSlotIcon->Initialize(pArg)))
	{
		Safe_Release(pQuickSlotIcon);
		MSG_BOX("CLONE FAIL : QUICK SLOT ICON");
	}
	return pQuickSlotIcon;
}

void CItemQuickSlotIcon::Free()
{
	__super::Free();
}
