#include "NeturalPellInfo.h"

#include "NeturalTypeIcon.h"
#include "PellHealthBar.h"

#include "PellBase.h"
#include "PellBody.h"
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
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

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
	{
		m_pOwner = Desc->pOwner;
		auto PellBody = static_cast<CPellBody*>(m_pOwner->FindPartObject(TEXT("Part_Body")));
		if(PellBody)
			m_pSpineMatrix = PellBody->GetBoneMatrix(Desc->szAttachSocketName);
	}

	return S_OK;
}

void CNeturalPellInfo::Update(_float fDeletaTime)
{
	if (m_pSpineMatrix)
	{
		_matrix WorldMat = XMLoadFloat4x4(m_pSpineMatrix) * XMLoadFloat4x4(&m_pOwner->GetTransform()->GetWorldMat());
		_float3 CombindPoisition = {};
		_float2	vNDCPos = {};

		auto MatVP = XMLoadFloat4x4(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)) * XMLoadFloat4x4(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION));
		XMStoreFloat3(&CombindPoisition, WorldMat.r[3]);

		XMStoreFloat2(&vNDCPos, XMVector3TransformCoord(XMLoadFloat3(&CombindPoisition), MatVP));

		// 투영 스페이스에 있는 위치
		// X : -1 ~ 1  -> 0 ~ 1 로변경해서 스크린 위치를 구한다.
		// Y : 1 ~ -1 -> 0 ~ 1 로 변경
		m_pTransformCom->SetPosition({ vNDCPos.x * g_iHalfWinSizeX + m_pTransformCom->GetScale().x * 0.5f,
									   vNDCPos.y * g_iHalfWinSizeY, 0.f });

	}
	
	for (auto pChild : m_pChildList)
		pChild->Update(fDeletaTime);
}

void CNeturalPellInfo::Late_Update(_float fDeletaTime)
{
	SetUpPellInfoData();

	for (auto pChild : m_pChildList)
		pChild->Late_Update(fDeletaTime);

	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CNeturalPellInfo::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(1);

	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}


HRESULT CNeturalPellInfo::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Point"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_PellInfo_BackGround_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_WorldUI"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNeturalPellInfo::ADD_Childs()
{
	CBackGround::GAMEOBJECT_DESC Desc = {};
	Desc.pParent = this;
	_float3 vParentScale = m_pTransformCom->GetScale();
	Desc.vScale = { 100.f , 15.f, 0.f };

	//Pell Health Bar
	Desc.vPosition = { 10.f, -Desc.vScale.y * 0.4f, 0.f };
	m_pHealthBar = static_cast<CPellHealthBar *>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Netrual_Pell_Health_Bar"), &Desc));
	m_pHealthBar->SetZOrder(m_iZOrder + 1);
	ADD_Child(m_pHealthBar);

	//Pell Type
	Desc.vScale = { 30.f, 30.f, 0.f };
	Desc.vPosition = { -vParentScale.x * 0.5f, vParentScale.y * 0.4f, 0.f };
	m_pTypeIcon = static_cast<CNeturalTypeIcon*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Netrual_Pell_Type_UI"), &Desc));
	m_pTypeIcon->SetZOrder(m_iZOrder + 1);
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
