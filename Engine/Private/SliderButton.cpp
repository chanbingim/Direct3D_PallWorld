#include "SliderButton.h"

#include "GameInstance.h"
CSliderButton::CSliderButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUserInterface(pDevice, pContext)
{
}

CSliderButton::CSliderButton(const CSliderButton& rhs) :
	CUserInterface(rhs)
{
}

HRESULT CSliderButton::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CSliderButton::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_bIsMouseEvent = true;
	return S_OK;
}

void CSliderButton::Update(_float fDeletaTime)
{
	__super::Update(fDeletaTime);
}

void CSliderButton::Late_Update(_float fDeletaTime)
{
}

HRESULT CSliderButton::Render()
{
	return S_OK;
}

void CSliderButton::MouseHoverEnter()
{
}

void CSliderButton::MouseHovering()
{
}

void CSliderButton::MouseHoverExit()
{
}

void CSliderButton::MouseButtonDown()
{
	m_SliderButClick = true;
}

void CSliderButton::MouseButtonUp()
{
	m_SliderButClick = false;
}

CGameObject* CSliderButton::Clone(void* pArg)
{
	return nullptr;
}

void CSliderButton::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
