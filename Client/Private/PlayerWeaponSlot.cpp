#include "PlayerWeaponSlot.h"

#include "GameInstance.h"
#include "PlayerManager.h"

CPlayerWeaponSlot::CPlayerWeaponSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CPartObject(pDevice, pContext)
{
}

CPlayerWeaponSlot::CPlayerWeaponSlot(const CPlayerWeaponSlot& rhs) :
	CPartObject(rhs)
{
}

HRESULT CPlayerWeaponSlot::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerWeaponSlot::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	WEAPON_SLOT_DESC* SlotDesc = static_cast<WEAPON_SLOT_DESC*>(pArg);
	m_iSlotIndex = SlotDesc->iSlotIndex;

	return S_OK;
}

void CPlayerWeaponSlot::Priority_Update(_float fDeletaTime)
{
}

void CPlayerWeaponSlot::Update(_float fDeletaTime)
{
	if(0 < m_iSlotIndex)
		ChangeModelBuffer(CPlayerManager::GetInstance()->GetBackSlotItem(m_iSlotIndex), false);
	else
	{
		ChangeModelBuffer(CPlayerManager::GetInstance()->GetCurrentSelectItem(), false);
		m_bIsAnimWeapon = CPlayerManager::GetInstance()->GetIsAnimSelect();
	}
		
	if (nullptr == m_pVIBufferCom)
		return;

	if (m_bIsAnimWeapon)
		m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, 0.f);
}

void CPlayerWeaponSlot::Late_Update(_float fDeletaTime)
{
	UpdateCombinedMatrix();
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPlayerWeaponSlot::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	Bind_ShaderResources();

	_uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
	for (_uInt i = 0; i < iNumMeshes; ++i)
	{
		Apply_ConstantShaderResources(i);

		true == m_bIsAnimWeapon ? m_pShaderCom->Update_Shader(0) : m_pNoneAimShader->Update_Shader(0);
		m_pVIBufferCom->Render(i);
	}
	return S_OK;
}

void CPlayerWeaponSlot::ChangeModelBuffer(CModel* pModel, _bool bIsAnim)
{
	m_pVIBufferCom = pModel;
	m_bIsAnimWeapon = bIsAnim;
}

HRESULT CPlayerWeaponSlot::Bind_ShaderResources()
{
	if (m_bIsAnimWeapon)
	{
		m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
		m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
		m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
		m_pSRVEffect = m_pShaderCom->GetVariable("g_DiffuseTexture")->AsShaderResource();
		m_pBoneMatrixEffect = m_pShaderCom->GetVariable("g_BoneMatrices")->AsMatrix();
	}
	else
	{
		m_pEMVWorldMat = m_pNoneAimShader->GetVariable("g_WorldMatrix")->AsMatrix();
		m_pEMVViewMat = m_pNoneAimShader->GetVariable("g_ViewMatrix")->AsMatrix();
		m_pEMVProjMat = m_pNoneAimShader->GetVariable("g_ProjMatrix")->AsMatrix();
		m_pSRVEffect = m_pNoneAimShader->GetVariable("g_DiffuseTexture")->AsShaderResource();
	}

	return S_OK;
}

HRESULT CPlayerWeaponSlot::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
	m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_CombinedWorldMatrix));
	m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
	m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

	ID3D11ShaderResourceView* pResourceVeiw = {};
	m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
	if (pResourceVeiw)
		m_pSRVEffect->SetResource(pResourceVeiw);

	if (m_bIsAnimWeapon)
		m_pBoneMatrixEffect->SetMatrixArray(reinterpret_cast<const float*>(m_pVIBufferCom->GetBoneMatrices(iMeshIndex)), 0, m_pVIBufferCom->GetMeshNumBones(iMeshIndex));

	return S_OK;
}

HRESULT CPlayerWeaponSlot::ADD_Components()
{
	// 매번 CModel이 바뀔때마다 shader를 생성하는건 비효율적인거같은데 이거 그냥하나 만들어두자
	// 애니메이션 셰이더
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("AnimShader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// 애니메이션 아닐경우 사용할 셰이더
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pNoneAimShader)))
		return E_FAIL;

	return S_OK;
}

CPlayerWeaponSlot* CPlayerWeaponSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerWeaponSlot* pPlayerWeaponSlot = new CPlayerWeaponSlot(pDevice, pContext);
	if (FAILED(pPlayerWeaponSlot->Initalize_Prototype()))
	{
		Safe_Release(pPlayerWeaponSlot);
		MSG_BOX("CREATE FAIL : PLAYER WEAPON SLOT");
	}
	return pPlayerWeaponSlot;
}

CGameObject* CPlayerWeaponSlot::Clone(void* pArg)
{
	CPlayerWeaponSlot* pPlayerWeaponSlot = new CPlayerWeaponSlot(*this);
	if (FAILED(pPlayerWeaponSlot->Initialize(pArg)))
	{
		Safe_Release(pPlayerWeaponSlot);
		MSG_BOX("CLONE FAIL : PLAYER WEAPON SLOT");
	}
	return pPlayerWeaponSlot;
}

void CPlayerWeaponSlot::Free()
{
	CGameObject::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pNoneAimShader);
}
