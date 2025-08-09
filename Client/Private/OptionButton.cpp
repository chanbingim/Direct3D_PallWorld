#include "OptionButton.h"

#include "GameInstance.h"

COptionButton::COptionButton(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	CButton(pGraphic_Device, pDeviceContext)
{
}

COptionButton::COptionButton(const COptionButton& rhs) :
	CButton(rhs)
{
}

HRESULT COptionButton::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT COptionButton::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	
	return S_OK;
}

void COptionButton::Update(_float fDeletaTime)
{
	__super::Update(fDeletaTime);
}

void COptionButton::Late_Update(_float fDeletaTime)
{
	__super::Late_Update(fDeletaTime);
}

HRESULT COptionButton::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(2);

	if (m_bIsHover || m_bIsSelect)
		m_pTextureCom->SetTexture(0, 1);
	else
		m_pTextureCom->SetTexture(0, 0);

	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

void COptionButton::Bind_ClickEvent(function<void()> Func)
{
	m_ClickedFunc = Func;
}

HRESULT COptionButton::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_InGameMenu_OptionBut"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

COptionButton* COptionButton::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
	COptionButton* pOtionButton = new COptionButton(pGraphic_Device, pDeviceContext);
	if (FAILED(pOtionButton->Initalize_Prototype()))
	{
		Safe_Release(pOtionButton);
		MSG_BOX("CREATE FAIL : OPTION BUTTON");
	}
	return pOtionButton;
}

CGameObject* COptionButton::Clone(void* pArg)
{
	COptionButton* pOtionButton = new COptionButton(*this);
	if (FAILED(pOtionButton->Initialize(pArg)))
	{
		Safe_Release(pOtionButton);
		MSG_BOX("CLONE FAIL : OPTION BUTTON");
	}
	return pOtionButton;
}

void COptionButton::Free()
{
	__super::Free();
}
