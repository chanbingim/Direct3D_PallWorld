#include "ProgressBar.h"

#include "GameInstance.h"

CProgressBar::CProgressBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUserInterface(pDevice, pContext)
{
}

CProgressBar::CProgressBar(const CProgressBar& rhs) :
	CUserInterface(rhs)
{
}

HRESULT CProgressBar::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CProgressBar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	PROGRESS_DESC* Desc = static_cast<PROGRESS_DESC*>(pArg);
	if (Desc)
	{
		m_vColor = Desc->vColor;
	}

	return S_OK;
}

void CProgressBar::Update(_float fDeletaTime)
{
	__super::Update(fDeletaTime);
}

void CProgressBar::Late_Update(_float fDeletaTime)
{
	__super::Late_Update(fDeletaTime);
}

HRESULT CProgressBar::Render()
{


	return S_OK;
}

void CProgressBar::SetPercent(_float fPercent)
{
	m_fPercent = fPercent;
}

_float CProgressBar::GetPercent()
{
	return m_fPercent;
}

void CProgressBar::LerpTimeReset()
{
	m_fLerpAccTime = 1.f;
}

HRESULT CProgressBar::Bind_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
	m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
	m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();

	m_pShader_Percent = m_pShaderCom->GetVariable("g_Percent");
	m_pShader_Color   = m_pShaderCom->GetVariable("g_Color")->AsVector();

	return S_OK;
}

HRESULT CProgressBar::Apply_ConstantShaderResources()
{
	__super::Apply_ConstantShaderResources();

	m_pShader_Percent->SetRawValue(&m_fPercent, 0, sizeof(_float));
	m_pShader_Color->SetFloatVector((float *)&m_vColor);
	return S_OK;
}

void CProgressBar::LerpAnimation(_float fTimeDeleta)
{
	if (m_fPercent != m_fPrePercent)
	{
		m_fLerpAccTime -= 0.05f;

		m_fLerpAccTime = Clamp<_float>(m_fLerpAccTime, 0.f, 1.f);
		m_fPrePercent = Lerp<_float>(m_fPrePercent, m_fPercent, 1 - m_fLerpAccTime);
	}
}

CGameObject* CProgressBar::Clone(void* pArg)
{
	return nullptr;
}

void CProgressBar::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
