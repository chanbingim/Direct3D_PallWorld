#include "CombatComponent.h"

#include "GameObject.h"

#include "EffectContatiner.h"
#include "Transform.h"

CCombatComponent::CCombatComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CComponent(pDevice, pContext)
{
}

CCombatComponent::CCombatComponent(const CCombatComponent& rhs) :
	CComponent(rhs)
{
}

HRESULT CCombatComponent::Initialize(void* pArg)
{
	COMBAT_COMPONENT_DESC* pDesc = static_cast<COMBAT_COMPONENT_DESC*>(pArg);
	if (pDesc)
	{
		m_pOwner = pDesc->pOwner;
		m_fLostTargetTime = pDesc->fLostTargetTime;
		m_fChangeTargetDistance = pDesc->fChangeTargetDistance;
		m_fLostTargetDistance = pDesc->fLostTargetDistance;
		m_BindCombatFunc = pDesc->CallBackFunction;
	}

	return S_OK;
}

void CCombatComponent::Update(_float fDeletaTime)
{
	if (nullptr != m_BindCombatFunc)
	{
		if (m_pTargetObject)
			m_BindCombatFunc(fDeletaTime, m_pTargetObject);
	}
}

void CCombatComponent::ResetCombatComponent()
{
	m_pTargetList.clear();
	m_pTargetObject = nullptr;
}

void CCombatComponent::ADD_TargetObject(CGameObject* pTarget)
{
	if (nullptr != dynamic_cast<CEffectContatiner*>(pTarget))
		return;

	auto iter = find(m_pTargetList.begin(), m_pTargetList.end(), pTarget);
	if (iter == m_pTargetList.end())
		m_pTargetList.push_back(pTarget);
}

void CCombatComponent::Remove_TargetObject(CGameObject* pTarget)
{
	if (nullptr == pTarget)
		return;

	auto iter = find(m_pTargetList.begin(), m_pTargetList.end(), pTarget);
	if (iter != m_pTargetList.end())
		m_pTargetList.erase(iter);
}

void CCombatComponent::UpdateTarget()
{
	if (m_pTargetList.empty())
		m_pTargetObject = nullptr;

	_float3 vOwnerPos = m_pOwner->GetTransform()->GetPosition();
	_vector vCalculationOwnerPos = XMLoadFloat3(&vOwnerPos);
	for(auto pTargetIter = m_pTargetList.begin(); pTargetIter != m_pTargetList.end();)
	{
		if (nullptr == m_pTargetObject)
		{
			m_pTargetObject = *pTargetIter;
			break;
		}

		if ((*pTargetIter)->IsDead())
		{
			pTargetIter = m_pTargetList.erase(pTargetIter);
		}
		else
		{
			_float3 newTargetPos = (*pTargetIter)->GetTransform()->GetPosition();
			_vector newCalculationTargetPos = XMLoadFloat3(&newTargetPos);
			_float fDistance = XMVectorGetX(XMVector3Length(newCalculationTargetPos - vCalculationOwnerPos));

			if (fDistance <= m_fChangeTargetDistance)
			{
				_float3 OldTargetPos = m_pTargetObject->GetTransform()->GetPosition();
				_vector OldCalculationTargetPos = XMLoadFloat3(&OldTargetPos);
				_float fOldDistance = XMVectorGetX(XMVector3Length(OldCalculationTargetPos - vCalculationOwnerPos));

				if (fDistance < fOldDistance)
					m_pTargetObject = (*pTargetIter);
			}
			
			if (fDistance >= m_fLostTargetDistance)
			{
				pTargetIter = m_pTargetList.erase(pTargetIter);
			}
			else
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
	if (FAILED(pCombatComponent->Initialize(pArg)))
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
