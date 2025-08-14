#include "GameObject.h"

#include "GameInstance.h"
#include "Component.h"

CGameObject::CGameObject(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	m_pGraphic_Device(pGraphic_Device),
	m_pDeviceContext(pDeviceContext),
	m_pGameInstance(CGameInstance::GetInstance())

{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pDeviceContext);
}

CGameObject::CGameObject(const CGameObject& rhs) :
	m_pGraphic_Device(rhs.m_pGraphic_Device),
	m_pDeviceContext(rhs.m_pDeviceContext),
	m_pGameInstance(rhs.m_pGameInstance)
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device); 
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CGameObject::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	m_pTransformCom = CTransform::Create(m_pGraphic_Device, m_pDeviceContext);
	if (pArg)
	{
		GAMEOBJECT_DESC* pObjectDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

		if(pObjectDesc->pParent)
			SetParent(pObjectDesc->pParent);
		SetLocation(pObjectDesc->vPosition);
		SetRotation(pObjectDesc->vRotation);
		SetScale(pObjectDesc->vScale);
	}

	Safe_AddRef(m_pTransformCom);
	m_pComponentMap.emplace(TEXT("Transform_Com"), m_pTransformCom);

	return S_OK;
}

void CGameObject::Priority_Update(_float fDeletaTime)
{
}

void CGameObject::Update(_float fDeletaTime)
{
}

//랜더에 넣는다거나 Update 이후에 동작을 처리하기위한 행위
//부모를 가지고있을경우 부모의 이동상태에따라 캐싱해서 부모의 이동행렬을 가져온다.
//Initaize에서 부모를 세팅한 경우 부모의 월드 행렬을 가져온다.
void CGameObject::Late_Update(_float fDeletaTime)
{
	if (m_IsMoveParentTransform)
	{
		for (auto iter : m_pChildList)
			iter->GetTransform()->SetParentMatrix(GetTransform()->GetWorldMat());

		m_IsMoveParentTransform = false;
	}
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

HRESULT CGameObject::Bind_ShaderResources()
{
	return S_OK;
}

HRESULT CGameObject::Apply_ConstantShaderResources()
{
	return S_OK;
}

HRESULT CGameObject::Add_Component(_uInt iLevelIndex, const _wstring& _PrototypeTag, const _wstring& ComponentTag, CComponent** ppOut, void* pArg)
{
	auto pair = m_pComponentMap.find(ComponentTag);

	if (pair == m_pComponentMap.end())
	{
		auto pComponent = m_pGameInstance->Clone_Prototype<CComponent>(iLevelIndex, _PrototypeTag, pArg);
		m_pComponentMap.emplace(ComponentTag, pComponent);
		*ppOut = pComponent;
		Safe_AddRef(*ppOut);
	}
	else
		return E_FAIL;

	return S_OK;
}

CComponent* CGameObject::Find_Component(const _wstring& ComponentTag)
{
	auto pair = m_pComponentMap.find(ComponentTag);

	if (pair == m_pComponentMap.end())
		return nullptr;

	return pair->second;
}

void CGameObject::SetDead(_bool bflag)
{
	m_IsDead = bflag;
	for (auto& iter : m_pChildList)
	{
		iter->SetDead(true);
	}
}

void CGameObject::SetParent(CGameObject* pParent)
{
	m_pParent = pParent;
	m_pParent->Update_ParentMatrix();
}

void CGameObject::ADD_Child(CGameObject* pChild)
{
	m_pChildList.push_back(pChild);
	Safe_AddRef(pChild);
}

void CGameObject::Remove_Child(CGameObject* pChild)
{
	auto iter = find(m_pChildList.begin(), m_pChildList.end(), pChild);
	if (iter == m_pChildList.end())
		return;

	Safe_Release(*iter);
	m_pChildList.erase(iter);
}

const list<CGameObject*>* CGameObject::GetChildObject()
{
	return  &m_pChildList;
}

void CGameObject::SetLocation(_float3 vPosition)
{
	if (nullptr == m_pTransformCom)
		return;

	m_pTransformCom->SetPosition(vPosition);
	Update_ParentMatrix();
}

void CGameObject::SetRotation(_float3 vRotation)
{
	if (nullptr == m_pTransformCom)
		return;

	m_pTransformCom->SetRotation(vRotation);
	Update_ParentMatrix();
}

void CGameObject::SetScale(_float3 vScale)
{
	if (nullptr == m_pTransformCom)
		return;

	m_pTransformCom->SetScale(vScale);
	Update_ParentMatrix();
}

void CGameObject::ADDPosition(_vector vAddPos)
{
	if (nullptr == m_pTransformCom)
		return;

	m_pTransformCom->ADD_Position(vAddPos);
	Update_ParentMatrix();
}

void CGameObject::ADDRotation(_vector vAxis, _float fTurnSpeed, _float fTimeDeleta)
{
	if (nullptr == m_pTransformCom)
		return;

	m_pTransformCom->Turn(vAxis, fTurnSpeed, fTimeDeleta);
	Update_ParentMatrix();
}

void CGameObject::SetObjectTag(const WCHAR* szTag)
{
	m_ObejctTag = szTag;
}

const _wstring& CGameObject::GetObjectTag()
{
	return m_ObejctTag;
}

CGameObject* CGameObject::Clone(void* pArg)
{
	return nullptr;
}

void CGameObject::Free()
{
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pTransformCom);

	for (auto& pair : m_pComponentMap)
	{
		Safe_Release(pair.second);
	}
	m_pComponentMap.clear();

	for (auto& iter : m_pChildList)
	{
		Safe_Release(iter);
	}
	m_pChildList.clear();
}

void CGameObject::Update_ParentMatrix()
{
	if (0 < m_pChildList.size())
		return;

	m_IsMoveParentTransform = true;
	for (auto& iter : m_pChildList)
	{
		iter->Update_ParentMatrix();
	}
}