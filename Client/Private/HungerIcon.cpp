#include "HungerIcon.h"

#include "GameInstance.h"

CHungerIcon::CHungerIcon(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	CBackGround(pGraphic_Device, pDeviceContext)
{
}

CHungerIcon::CHungerIcon(const CHungerIcon& rhs) :
	CBackGround(rhs)
{
}

HRESULT CHungerIcon::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHungerIcon::Initialize(void* pArg)
{
	if (FAILED(CUserInterface::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_eType = OBJECT_TYPE::STATIC;
	return S_OK;
}

void CHungerIcon::Update(_float fDeletaTime)
{
	__super::Update(fDeletaTime);
}

void CHungerIcon::Late_Update(_float fDeletaTime)
{
	__super::Late_Update(fDeletaTime);
}

HRESULT CHungerIcon::Render()
{
	Apply_ConstantShaderResources();

	m_pShaderCom->Update_Shader(1);
	m_pTextureCom->SetTexture(0, 0);

	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

HRESULT CHungerIcon::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_HungerIcon_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CHungerIcon* CHungerIcon::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
	CHungerIcon* pHungerIcon = new CHungerIcon(pGraphic_Device, pDeviceContext);
	if (FAILED(pHungerIcon->Initalize_Prototype()))
	{
		Safe_Release(pHungerIcon);
		MSG_BOX("CREATE FAIL : HUNGER ICON");
	}
	return pHungerIcon;
}

CGameObject* CHungerIcon::Clone(void* pArg)
{
	CHungerIcon* pHungerIcon = new CHungerIcon(*this);
	if (FAILED(pHungerIcon->Initialize(pArg)))
	{
		Safe_Release(pHungerIcon);
		MSG_BOX("CLONE FAIL : HUNGER ICON");
	}
	return pHungerIcon;
}

void CHungerIcon::Free()
{
	__super::Free();
}
