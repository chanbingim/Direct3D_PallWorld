#include "CombatComponent.h"

#include "GameObject.h"
#include "Transform.h"

CCombatComponent::CCombatComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CComponent(pDevice, pContext)
{
}

HRESULT CCombatComponent::Initialzie(void* pArg)
{
	COMBAT_COMPONENT_DESC* pDesc = static_cast<COMBAT_COMPONENT_DESC*>(pArg);
	if (pDesc)
	{
		m_pOwner = pDesc->pOwner;
		m_fLostTargetTime = pDesc->fLostTargetTime;
		m_fChangeTargetDistance = pDesc->fChangeTargetDistance;
		m_BindCombatFunc = pDesc->CallBackFunction;
	}

	return S_OK;
}

void CCombatComponent::Update(_float fDeletaTime)
{
	if (nullptr != m_BindCombatFunc)
		m_BindCombatFunc(m_pTargetObject);
}

void CCombatComponent::ADD_TargetObject(CGameObject* pTarget)
{
	auto iter = find(m_pTargetList.begin(), m_pTargetList.end(), pTarget);
	if (iter == m_pTargetList.end())
		m_pTargetList.push_back(pTarget);
}

void CCombatComponent::UpdateTarget()
{
	if (m_pTargetList.empty())
		m_pTargetObject = nullptr;

	_vector OwnerPos = XMLoadFloat3(&m_pOwner->GetTransform()->GetPosition());
	for(auto pTargetIter = m_pTargetList.begin(); pTargetIter != m_pTargetList.end();)
	{
		if ((*pTargetIter)->IsDead())
		{
			pTargetIter = m_pTargetList.erase(pTargetIter);
		}
		else
		{
			_vector newTargetPos = XMLoadFloat3(&(*pTargetIter)->GetTransform()->GetPosition());
			_float fDistance = XMVectorGetX(XMVector3Length(newTargetPos - OwnerPos));

			if (fDistance <= m_fChangeTargetDistance)
			{
				_vector OldTargetPos = XMLoadFloat3(&m_pTargetObject->GetTransform()->GetPosition());
				_float fOldDistance = XMVectorGetX(XMVector3Length(OldTargetPos - OwnerPos));

				if (fDistance < fOldDistance)
					m_pTargetObject = (*pTargetIter);
			}
			pTargetIter++;
		}
	}
}

CCombatComponent* CCombatComponent::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CCombatComponent(pDevice, pContext);
}

CComponent* CCombatComponent::Clone(void* pArg)
{
	CComponent*	pCombatComponent = new  CCombatComponent(*this);
	if (FAILED(pCombatComponent->Clone(pArg)))
	{
		Safe_Release(pCombatComponent);
		MSG_BOX("CREATE FAIL : Combat Component");
	}

	return pCombatComponent;
}

void CCombatComponent::Free()
{
	__super::Free();

	m_pTargetList.clear();
	m_pTargetObject = nullptr;
}
