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

	m_vColor[0] = { 1.f, 1.f, 1.f, 0.8f };
	m_vColor[1] = { 0.f, 132.f / 255.f, 1.0f, 0.8f };
	m_StartU = 0.2f;
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
	m_pShaderCom->Update_Shader(1);
	m_pVIBufferCom->Render_VIBuffer();

	m_pFontCom->Render(m_FontText.c_str(), {1.f, 1.f, 1.f, 1.f});
	return S_OK;
}

void COptionButton::SetText(const WCHAR* szText)
{
	m_FontText = szText;
	m_vFontPosition = GetScreenPos();
}

void COptionButton::Bind_ClickEvent(function<void()> Func)
{
	m_ClickedFunc = Func;
}

HRESULT COptionButton::Apply_ConstantShaderResources()
{
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->GetWorldMat());
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pShaderCom->Bind_SRV("g_Texture", m_pTextureCom->GetTexture(0));
	m_pShaderCom->Bind_SRV("g_EftTexture", m_pTextureCom->GetTexture(1));


	true == m_bIsHover ? m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor[1], sizeof(_float4)) :
			m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor[0], sizeof(_float4));


	m_pShaderCom->Bind_RawValue("g_fStartU", &m_StartU, sizeof(_float));

	return S_OK;
}

HRESULT COptionButton::ADD_Components()
{
	// 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
	CFontComponent::FONT_DESC FontDesc = {};
	m_vFontPosition = GetScreenPos();
	FontDesc.pPoint = &m_vFontPosition;
	FontDesc.szUseFontName = TEXT("HanSanFont_16");
	FontDesc.vFontSize = { 50, 100 };

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_InGameMenu_OptionBut"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Button"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
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

	Safe_Release(m_pFontCom);
}
