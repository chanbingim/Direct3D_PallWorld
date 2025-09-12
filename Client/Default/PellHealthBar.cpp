#include "PellHealthBar.h"

#include "GameInstance.h"

CPellHealthBar::CPellHealthBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CProgressBar(pDevice, pContext)
{
}

CPellHealthBar::CPellHealthBar(const CPellHealthBar& rhs) :
	CProgressBar(rhs)
{
}

HRESULT CPellHealthBar::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPellHealthBar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_eType = OBJECT_TYPE::STATIC;

	return S_OK;
}

void CPellHealthBar::Update(_float fDeletaTime)
{
}

void CPellHealthBar::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CPellHealthBar::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(0);

	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->Update_Shader(1);
	m_pTextureCom->SetTexture(0, 1);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

CPellHealthBar* CPellHealthBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPellHealthBar* pHealthBar = new CPellHealthBar(pDevice, pContext);
	if (FAILED(pHealthBar->Initalize_Prototype()))
	{
		Safe_Release(pHealthBar);
		MSG_BOX("CREATE FAIL : Pell Health Bar");
	}

	return pHealthBar;
}

CGameObject* CPellHealthBar::Clone(void* pArg)
{
	CPellHealthBar* pHealthBar = new CPellHealthBar(*this);
	if (FAILED(pHealthBar->Initialize(pArg)))
	{
		Safe_Release(pHealthBar);
		MSG_BOX("CLONE FAIL : Pell Health Bar");
	}

	return pHealthBar;
}

void CPellHealthBar::Free()
{
	__super::Free();
}
