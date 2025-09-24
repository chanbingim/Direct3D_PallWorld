#include "PallSpherPercentBar.h"

#include "GameInstance.h"
#include "PalSpherUI.h"

CPallSpherPercentBar::CPallSpherPercentBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CProgressBar(pDevice, pContext)
{
}

CPallSpherPercentBar::CPallSpherPercentBar(const CPallSpherPercentBar& rhs) :
	CProgressBar(rhs)
{
}

HRESULT CPallSpherPercentBar::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CPallSpherPercentBar::Initialize(void* pArg)
{
	m_bIsWorld = true;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_eType = OBJECT_TYPE::STATIC;
	return S_OK;
}

void CPallSpherPercentBar::Update(_float fDeletaTime)
{
	CPalSpherUI* pPallSpherUI = dynamic_cast<CPalSpherUI*>(m_pParent);
	if (pPallSpherUI)
	{
		_matrix ParentMat = XMLoadFloat4x4(&pPallSpherUI->GetCombinedMatrix());
		for (_uInt i = 0; i < 3; ++i)
			ParentMat.r[i] = XMVector3Normalize(ParentMat.r[i]);

		_matrix WorldMat = XMLoadFloat4x4(&m_pTransformCom->GetWorldMat());

		XMStoreFloat4x4(&m_CombinedMat, WorldMat * ParentMat);
	}
}

void CPallSpherPercentBar::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPallSpherPercentBar::Render()
{
	Apply_ConstantShaderResources();

	m_pShaderCom->Update_Shader(4);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

HRESULT CPallSpherPercentBar::Apply_ConstantShaderResources()
{
	m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_CombinedMat));
	m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
	m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));
	 
	m_pShader_Percent->SetRawValue(&m_fPercent, 0, sizeof(_float));
	m_pShader_Color->SetFloatVector((float*)&m_vColor);

	return S_OK;
}

HRESULT CPallSpherPercentBar::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Pell_PalSpherUI_PercentBar_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_ProgressBar"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CPallSpherPercentBar* CPallSpherPercentBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPallSpherPercentBar* pSpherPercent = new CPallSpherPercentBar(pDevice, pContext);
	if (FAILED(pSpherPercent->Initalize_Prototype()))
	{
		Safe_Release(pSpherPercent);
		MSG_BOX("CREATE FAILED : PallSpherPercent");
	}
	return pSpherPercent;
}

CGameObject* CPallSpherPercentBar::Clone(void* pArg)
{
	CPallSpherPercentBar* pSpherPercent = new CPallSpherPercentBar(*this);
	if (FAILED(pSpherPercent->Initialize(pArg)))
	{
		Safe_Release(pSpherPercent);
		MSG_BOX("CLONE FAILED : PallSpherPercent");
	}
	return pSpherPercent;
}

void CPallSpherPercentBar::Free()
{
	__super::Free();
}
