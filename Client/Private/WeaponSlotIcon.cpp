#include "WeaponSlotIcon.h"

#include "GameInstance.h"

CWeaponSlotIcon::CWeaponSlotIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CWeaponSlotIcon::CWeaponSlotIcon(const CWeaponSlotIcon& rhs) :
	CBackGround(rhs)
{
}

HRESULT CWeaponSlotIcon::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeaponSlotIcon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_iZOrder = 1;
	return S_OK;
}

void CWeaponSlotIcon::Update(_float fDeletaTime)
{
}

void CWeaponSlotIcon::Late_Update(_float fDeletaTime)
{
	if (m_pItemIcon)
		m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CWeaponSlotIcon::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(2);

	if(m_pItemIcon)
		m_pItemIcon->SetTexture(0, 0);

	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

void CWeaponSlotIcon::SetIconTexture(const CTexture* pIconTexture)
{
	m_pItemIcon = pIconTexture;
}

HRESULT CWeaponSlotIcon::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	//Áö¿À¸áÆ®¸® ¼ÎÀÌ´õ ±¸ÇöÇØ¼­ ¹Ù²Ü¿¹Á¤
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CWeaponSlotIcon* CWeaponSlotIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeaponSlotIcon* pWeaponquickSlotIcon = new CWeaponSlotIcon(pDevice, pContext);
	if (FAILED(pWeaponquickSlotIcon->Initalize_Prototype()))
	{
		Safe_Release(pWeaponquickSlotIcon);
		MSG_BOX("CREATE FAIL : WEAPON QUICK SLOT ICON");
	}

	return pWeaponquickSlotIcon;
}

CGameObject* CWeaponSlotIcon::Clone(void* pArg)
{
	CGameObject* pWeaponquickSlotIcon = new CWeaponSlotIcon(*this);
	if (FAILED(pWeaponquickSlotIcon->Initialize(pArg)))
	{
		Safe_Release(pWeaponquickSlotIcon);
		MSG_BOX("CLONE FAIL : WEAPON QUICK SLOT ICON");
	}

	return pWeaponquickSlotIcon;
}

void CWeaponSlotIcon::Free()
{
	__super::Free();
}
