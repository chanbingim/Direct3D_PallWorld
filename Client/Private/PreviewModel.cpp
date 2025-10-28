#include "PreviewModel.h"

#include "GameInstance.h"
#include "PreivewModelBody.h"

#include "PlayerManager.h"
#include "ItemBase.h"

CPreviewModel::CPreviewModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CContainerObject(pDevice, pContext)
{
}

CPreviewModel::CPreviewModel(const CPreviewModel& rhs) :
	CContainerObject(rhs)
{
}

HRESULT CPreviewModel::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CPreviewModel::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CPreviewModel::Priority_Update(_float fDeletaTime)
{
	__super::Priority_Update(fDeletaTime);
	
	_string m_szAnimName = "Idle";
	m_szAnimName += GetWeaponName();

	m_pAnimator->SetAnimIndex(m_pAnimator->GetAnimIndex(m_szAnimName.c_str()), 1.f, true);
}

void CPreviewModel::Update(_float fDeletaTime)
{
	__super::Update(fDeletaTime);
}

void CPreviewModel::Late_Update(_float fDeletaTime)
{
	__super::Late_Update(fDeletaTime);
}

HRESULT CPreviewModel::Render()
{
	for (auto& Pair : m_PartObjects)
		Pair.second->Render();

	return S_OK;
}

HRESULT CPreviewModel::ADD_PartObjects()
{
	CPartObject::PARTOBJECT_DESC Desc;
	ZeroMemory(&Desc, sizeof(CPartObject::PARTOBJECT_DESC));

	Desc.pParent = this;
	Desc.vScale = { 1.f, 1.f, 1.f };

	lstrcpy(Desc.ObjectTag, L"Player_Animator");
	if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PreviewModelBody"), TEXT("Player_Animator"), &Desc)))
		return E_FAIL;

	m_pAnimator = static_cast<CPreivewModelBody*>(FindPartObject(TEXT("Player_Animator")));
	return S_OK;
}

const char* CPreviewModel::GetWeaponName()
{
	auto pPlayerManager = CPlayerManager::GetInstance();
	auto ItemBaseData = pPlayerManager->GetSlotItemInfo(EUQIP_TYPE::WEAPON, 0);
	if(nullptr == ItemBaseData)
		return "_None";

	//무기 타입에 의해 네이밍
	switch (ItemBaseData->GetItemData().TypeDesc.EuqipDesc.Weapon_Type)
	{
	case WEAPON::MELEE:
		return "_Melee";
	case WEAPON::BOW:
		return "_Bow";
	case WEAPON::THROW:
		return "_Throw";
	}

	return "_None";
}
CPreviewModel* CPreviewModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPreviewModel* pPreivewModel = new CPreviewModel(pDevice, pContext);
	if (FAILED(pPreivewModel->Initalize_Prototype()))
	{
		Safe_Release(pPreivewModel);
		MSG_BOX("CREATE FAIL : PREIVEW MODEL");
	}
	return pPreivewModel;
}

CGameObject* CPreviewModel::Clone(void* pArg)
{
	CPreviewModel* pPreivewModel = new CPreviewModel(*this);
	if (FAILED(pPreivewModel->Initialize(pArg)))
	{
		Safe_Release(pPreivewModel);
		MSG_BOX("CLONE FAIL : PREIVEW MODEL");
	}
	return pPreivewModel;
}

void CPreviewModel::Free()
{
	__super::Free();
}
