#include "PellHealthBar.h"

#include "GameInstance.h"
#include "NeturalPellInfo.h"

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

void CPellHealthBar::Update(_float fDeletaTime)
{
	CNeturalPellInfo* pNeturalPell = dynamic_cast<CNeturalPellInfo*>(m_pParent);
	if (pNeturalPell)
	{
		_matrix ParentMat = XMLoadFloat4x4(&pNeturalPell->GetCombinedMatrix());
		for (_uInt i = 0; i < 3; ++i)
			ParentMat.r[i] = XMVector3Normalize(ParentMat.r[i]);

		_matrix WorldMat = XMLoadFloat4x4(&m_pTransformCom->GetWorldMat());

		XMStoreFloat4x4(&m_CombinedMat, WorldMat * ParentMat);
	}
}

void CPellHealthBar::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPellHealthBar::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(0);

	m_pShaderCom->Update_Shader(1);
	m_pTextureCom->SetTexture(0, 1);
	m_pVIBufferCom->Render_VIBuffer();

	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();



	return S_OK;
}

HRESULT CPellHealthBar::Apply_ConstantShaderResources()
{
	m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_CombinedMat));
	m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
	m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

	return S_OK;
}

HRESULT CPellHealthBar::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Pell_PalSpher_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_ProgressBar"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

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
