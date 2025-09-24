#include "NeturalPellInfo.h"

#include "NeturalTypeIcon.h"
#include "PellHealthBar.h"

#include "PellBase.h"
#include "GameInstance.h"

CNeturalPellInfo::CNeturalPellInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CNeturalPellInfo::CNeturalPellInfo(const CNeturalPellInfo& rhs) :
	CBackGround(rhs)
{
}

HRESULT CNeturalPellInfo::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CNeturalPellInfo::Initialize(void* pArg)
{
	m_bIsWorld = true;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(ADD_Childs()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	NETURAL_PELL_DESC* Desc = static_cast<NETURAL_PELL_DESC *>(pArg);
	if (Desc)
		m_pOwner = Desc->pOwner;

	return S_OK;
}

void CNeturalPellInfo::Update(_float fDeletaTime)
{
	_float3 vScale = m_pTransformCom->GetScale();
	_float3 vPos = m_pTransformCom->GetPosition();
	_matrix WorldMat = XMLoadFloat4x4(&m_pGameInstance->GetInvMatrix(MAT_STATE::VIEW));
	for (_uInt i = 0; i < 3; ++i)
		WorldMat.r[i] = XMVector3Normalize(WorldMat.r[i])* XMLoadFloat3(&vScale).m128_f32[i];

	WorldMat.r[3] = XMLoadFloat3(&vPos);
	if (m_pOwner)
	{
		vPos = m_pOwner->GetTransform()->GetPosition();
		WorldMat.r[3] += XMLoadFloat3(&vPos);
		WorldMat.r[3].m128_f32[3] = 1;
		XMStoreFloat4x4(&m_CombinedMatrix, WorldMat);
	}

	for (auto pChild : m_pChildList)
		pChild->Update(fDeletaTime);
}

void CNeturalPellInfo::Late_Update(_float fDeletaTime)
{
	SetUpPellInfoData();

	for (auto pChild : m_pChildList)
		pChild->Late_Update(fDeletaTime);

	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CNeturalPellInfo::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(3);

	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}


HRESULT CNeturalPellInfo::Apply_ConstantShaderResources()
{
	m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_CombinedMatrix));
	m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
	m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));


	return S_OK;
}

HRESULT CNeturalPellInfo::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_PellInfo_BackGround_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNeturalPellInfo::ADD_Childs()
{
	CBackGround::GAMEOBJECT_DESC Desc = {};
	Desc.pParent = this;
	_float3 vParentScale = m_pTransformCom->GetScale();
	Desc.vScale = { vParentScale.x - 0.16f , 0.5f, 0.f };

	//Pell Health Bar
	Desc.vPosition = { 0, -(vParentScale.y * 0.25f), -0.01f };
	m_pHealthBar = static_cast<CPellHealthBar *>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Netrual_Pell_Health_Bar"), &Desc));
	ADD_Child(m_pHealthBar);

	//Pell Type
	Desc.vScale = { 0.5f, 0.5f, 0.f };
	Desc.vPosition = { -vParentScale.x * 0.3f, vParentScale.y * 0.4f, -0.02f };
	m_pTypeIcon = static_cast<CNeturalTypeIcon*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Netrual_Pell_Type_UI"), &Desc));
	ADD_Child(m_pTypeIcon);

	return S_OK;
}

void CNeturalPellInfo::SetUpPellInfoData()
{
	const PELL_INFO& PellData = m_pOwner->GetPellInfo();

	m_pHealthBar->SetPercent(PellData.CurHealth / PellData.MaxHealth);
	m_pTypeIcon->SetData(ENUM_CLASS(PellData.ePellType));
}

CNeturalPellInfo* CNeturalPellInfo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNeturalPellInfo* pNeturalPellInfo = new CNeturalPellInfo(pDevice, pContext);
	if (FAILED(pNeturalPellInfo->Initalize_Prototype()))
	{
		Safe_Release(pNeturalPellInfo);
		MSG_BOX("CREATE FAIL : NETURAL PELL INFO");
	}
	return pNeturalPellInfo;
}

CGameObject* CNeturalPellInfo::Clone(void* pArg)
{
	CNeturalPellInfo* pNeturalPellInfo = new CNeturalPellInfo(*this);
	if (FAILED(pNeturalPellInfo->Initialize(pArg)))
	{
		Safe_Release(pNeturalPellInfo);
		MSG_BOX("CLONE FAIL : NETURAL PELL INFO");
	}
	return pNeturalPellInfo;
}

void CNeturalPellInfo::Free()
{
	__super::Free();

	Safe_Release(m_pHealthBar);
	Safe_Release(m_pTypeIcon);
}
