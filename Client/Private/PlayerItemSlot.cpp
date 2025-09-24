#include "PlayerItemSlot.h"

#include "GameInstance.h"
#include "PlayerManager.h"
#include "ItemBase.h"
#include "Player.h"

CPlayerItemSlot::CPlayerItemSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CPartObject(pDevice, pContext)
{
}

CPlayerItemSlot::CPlayerItemSlot(const CPlayerItemSlot& rhs) :
	CPartObject(rhs)
{
}

HRESULT CPlayerItemSlot::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerItemSlot::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	ITEM_SLOT_DESC* SlotDesc = static_cast<ITEM_SLOT_DESC*>(pArg);
	m_iSlotIndex = SlotDesc->iSlotIndex;
	m_eState = WEAPON_STATE::IDLE;

	return S_OK;
}

void CPlayerItemSlot::Priority_Update(_float fDeletaTime)
{

}

void CPlayerItemSlot::Update(_float fDeletaTime)
{
	m_CurrentEuipItemInfo = CPlayerManager::GetInstance()->GetSlotItemData(m_iSlotIndex);
	ChangeModelBuffer(CPlayerManager::GetInstance()->GetBackSlotItem(m_iSlotIndex), false);

	if (nullptr == m_pVIBufferCom || nullptr == m_CurrentEuipItemInfo)
		return;

	const ITEM_DESC& ItemData = m_CurrentEuipItemInfo->GetItemData();
	m_bIsAnimWeapon = ItemData.IsAnimModel;
	if (m_bIsAnimWeapon)
	{
		if (ItemData.IsPlayAnimation)
			m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime);
		else
			m_pVIBufferCom->PlayAnimation(0, 0, 0);
	}
}

void CPlayerItemSlot::Late_Update(_float fDeletaTime)
{
	UpdateCombinedMatrix();

	if (nullptr != m_pVIBufferCom)
		m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPlayerItemSlot::Render()
{
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

void CPlayerItemSlot::ChangeModelBuffer(CModel* pModel, _bool bIsAnim)
{
	m_pVIBufferCom = pModel;
}

void CPlayerItemSlot::ChangeWeaponState(WEAPON_STATE eWeaponState, _bool bIsAnimLoop)
{
	m_eState = eWeaponState;
	m_bIsAnimLoop = bIsAnimLoop;
}

HRESULT CPlayerItemSlot::Bind_ShaderResources()
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

HRESULT CPlayerItemSlot::Apply_ConstantShaderResources(_uInt iMeshIndex)
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

HRESULT CPlayerItemSlot::ADD_Components()
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

void CPlayerItemSlot::ChangeAnimWaponAnimationIndex()
{
	switch (m_eState)
	{
	case Client::CPlayerItemSlot::WEAPON_STATE::CHARGE:
		m_bIsAnimLoop = false;
		m_iAnimIndex = 1;
		break;
	case Client::CPlayerItemSlot::WEAPON_STATE::CHARGE_LOOP:
		m_bIsAnimLoop = true;
		m_iAnimIndex = 2;
		break;
	case Client::CPlayerItemSlot::WEAPON_STATE::ATTACK:
		m_iAnimIndex = 0;
		break;
	case Client::CPlayerItemSlot::WEAPON_STATE::IDLE:
		m_iAnimIndex = 3;
		break;
	}

}


CPlayerItemSlot* CPlayerItemSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerItemSlot* pPlayerItemSlot = new CPlayerItemSlot(pDevice, pContext);
	if (FAILED(pPlayerItemSlot->Initalize_Prototype()))
	{
		Safe_Release(pPlayerItemSlot);
		MSG_BOX("CREATE FAIL : PLAYER ITEM SLOT");
	}
	return pPlayerItemSlot;
}

CGameObject* CPlayerItemSlot::Clone(void* pArg)
{
	CPlayerItemSlot* pPlayerItemSlot = new CPlayerItemSlot(*this);
	if (FAILED(pPlayerItemSlot->Initialize(pArg)))
	{
		Safe_Release(pPlayerItemSlot);
		MSG_BOX("CLONE FAIL : PLAYER ITEM SLOT");
	}
	return pPlayerItemSlot;
}

void CPlayerItemSlot::Free()
{
	CGameObject::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pNoneAimShader);
}