#include "PreviewModelWeapon.h"

#include "GameInstance.h"
#include "PlayerManager.h"

#include "ItemBase.h"
#include "PlayerStateMachine.h"

#include "PellBase.h"
#include "Player.h"

CPreviewModelWeapon::CPreviewModelWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CPlayerItemSlot(pDevice, pContext)
{
}

CPreviewModelWeapon::CPreviewModelWeapon(const CPreviewModelWeapon& rhs) :
	CPlayerItemSlot(rhs)
{
}

HRESULT CPreviewModelWeapon::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CPreviewModelWeapon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	WEAPON_SLOT_DESC* SlotDesc = static_cast<WEAPON_SLOT_DESC*>(pArg);
	m_pLeftSocket = SlotDesc->pLeftSocket;

	return S_OK;;
}

void CPreviewModelWeapon::Priority_Update(_float fDeletaTime)
{
}

void CPreviewModelWeapon::Update(_float fDeletaTime)
{
	m_CurrentEuipItemInfo = CPlayerManager::GetInstance()->GetCurrentSlotItemInfo(EUQIP_TYPE::WEAPON);
	ChangeModelBuffer(CPlayerManager::GetInstance()->GetCurrentSlotItemModel(EUQIP_TYPE::WEAPON), false);

	if (m_CurrentEuipItemInfo)
	{
		const ITEM_DESC& ItemData = m_CurrentEuipItemInfo->GetItemData();
		if (ITEM_TYPE::EQUIPMENT == ItemData.ItemType)
			m_LeftFlag = ItemData.TypeDesc.EuqipDesc.bIsLeftSocket;

		m_bIsAnimWeapon = ItemData.IsAnimModel;
		if (m_bIsAnimWeapon)
		{
			if (ITEM_TYPE::EQUIPMENT == ItemData.ItemType)
			{
				if (WEAPON_STATE::CHARGE == m_eState && m_bIsAnimFinished)
					m_eState = WEAPON_STATE::CHARGE_LOOP;

				if (ItemData.TypeDesc.EuqipDesc.bIsChargeAble)
					ChangeAnimWaponAnimationIndex();
			}

			if (ItemData.IsPlayAnimation)
				m_bIsAnimFinished = m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime, 10.f, m_bIsAnimLoop);
			else
				m_pVIBufferCom->PlayAnimation(0, 0, 0);
		}
	}
}

void CPreviewModelWeapon::Late_Update(_float fDeletaTime)
{
	if (nullptr == m_pVIBufferCom)
	{
		UpdateCombinedMatrix(m_pLeftSocket);
		UpdateCombinedMatrix();
	}
	else
	{
		if (m_LeftFlag)
			UpdateCombinedMatrix(m_pLeftSocket);
		else
			UpdateCombinedMatrix();
	}
}

HRESULT CPreviewModelWeapon::Render()
{
	if (nullptr != m_pVIBufferCom)
	{
		Bind_ShaderResources();

		_uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
		for (_uInt i = 0; i < iNumMeshes; ++i)
		{
			Apply_ConstantShaderResources(i);

			true == m_bIsAnimWeapon ? m_pShaderCom->Update_Shader(2) : m_pNoneAimShader->Update_Shader(4);
			m_pVIBufferCom->Render(i);
		}
	}
	return S_OK;
}

CPreviewModelWeapon* CPreviewModelWeapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPreviewModelWeapon* pPreviewModel = new CPreviewModelWeapon(pDevice, pContext);
	if (FAILED(pPreviewModel->Initalize_Prototype()))
	{
		Safe_Release(pPreviewModel);
		MSG_BOX("CREAT FAIL : PREVEIW MODEL");
	}
	return pPreviewModel;
}

CGameObject* CPreviewModelWeapon::Clone(void* pArg)
{
	CPreviewModelWeapon* pPreviewModel = new CPreviewModelWeapon(*this);
	if (FAILED(pPreviewModel->Initialize(pArg)))
	{
		Safe_Release(pPreviewModel);
		MSG_BOX("CLONE FAIL : PREVEIW MODEL");
	}
	return pPreviewModel;
}

void CPreviewModelWeapon::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
}
