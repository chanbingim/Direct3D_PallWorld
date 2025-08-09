#include "Button.h"

#include "GameInstance.h"
#include "ButtonFrame.h"

CButton::CButton(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	CBackGround(pGraphic_Device, pDeviceContext)
{
}

CButton::CButton(const CButton& rhs) :
	CBackGround(rhs)
{
}

HRESULT CButton::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CButton::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Childs()))
		return E_FAIL;

	m_eType = OBJECT_TYPE::STATIC;
	m_bIsMouseEvent = true;
	m_iZOrder = 3;

	return S_OK;
}

void CButton::Update(_float fDeletaTime)
{
	__super::Update(fDeletaTime);

	if(m_bIsHover)
		m_pButtonFrame->Update(0.0f);
}

void CButton::Late_Update(_float fDeletaTime)
{
	__super::Late_Update(fDeletaTime);

	if (m_bIsHover)
		m_pButtonFrame->Late_Update(0.0f);
}

HRESULT CButton::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(2);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

void CButton::SetSelect(_bool flag)
{
	m_bIsSelect = flag;
}

_bool CButton::IsSelect()
{
	return m_bIsSelect;
}

void CButton::MouseHoverEnter()
{
	m_pButtonFrame->ResetButtonFrame();
}

void CButton::MouseHovering()
{

}

void CButton::MouseHoverExit()
{
}

void CButton::MouseButtonDwon()
{
	if(m_ClickedFunc)
		m_ClickedFunc();
}

HRESULT CButton::ADD_Components()
{

	return S_OK;
}

HRESULT CButton::ADD_Childs()
{
	CBackGround::GAMEOBJECT_DESC Desc;
	ZeroMemory(&Desc, sizeof(CBackGround::GAMEOBJECT_DESC));

	Desc.pParent = this;
	Desc.vScale = m_pTransformCom->GetScale();
	Desc.vScale.x += 3.f;
	Desc.vScale.y += 3.f;
	Desc.vPosition = {0.f , 0.f, 0.f};

	m_pButtonFrame = CButtonFrame::Create(m_pGraphic_Device, m_pDeviceContext);
	if (FAILED(m_pButtonFrame->Initialize(&Desc)))
		return E_FAIL;

	m_pButtonFrame->SetZOrder(3);
	return S_OK;
}

CGameObject* CButton::Clone(void* pArg)
{
	return nullptr;
}

void CButton::Free()
{
	__super::Free();

	Safe_Release(m_pButtonFrame);
}
