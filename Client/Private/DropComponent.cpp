#include "DropComponent.h"

#include "GameInstance.h"

CDropComponent::CDropComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CComponent(pDevice, pContext)
{
}

CDropComponent::CDropComponent(const CDropComponent& Prototype) :
	CComponent(Prototype)
{
}

void CDropComponent::GetDropItemDesc(_bool bOverlapItem, list<DROP_ITEM_DESC>* pItemDesc)
{
	_float fAccPercent = {};
	_float fWeightNum = m_pGameInstance->Random(0.f, 100.f);

	DROP_ITEM_DESC DropItemDesc = {};
	for (auto pair : m_pDorpItems)
	{
		_bool bAddFlag = false;
		fAccPercent += pair.second;
		if (fAccPercent >= fWeightNum)
			fAccPercent = true;

		if (fAccPercent)
		{
			DropItemDesc.iDropItemCount = m_pGameInstance->Random(1.f, 10.f);
			DropItemDesc.iDropItemIndex = pair.first;
			pItemDesc->push_back(DropItemDesc);

			if (false == bOverlapItem)
				break;
		}
	}
}

HRESULT CDropComponent::Insert_ItemIndex(_uInt iIndex, _float fPercent)
{
	auto iter = find_if(m_pDorpItems.begin(), m_pDorpItems.end(), [&](auto& Pair)
		{
			return iIndex == Pair.first;
		});

	if (iter == m_pDorpItems.end())
	{
		m_pDorpItems.emplace_back(make_pair(iIndex, fPercent));
		return S_OK;
	}

	return E_FAIL;
}

void CDropComponent::Remove_ItemIndex(_uInt iIndex, _float fPercent)
{
	auto iter = find_if(m_pDorpItems.begin(), m_pDorpItems.end(), [&](auto& Pair)
		{
			return iIndex == Pair.first;
		});

	if (iter == m_pDorpItems.end())
		return;
	else
		iter = m_pDorpItems.erase(iter);
}

CDropComponent* CDropComponent::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDropComponent* pDropCompoent = new CDropComponent(pDevice, pContext);
	if (FAILED(pDropCompoent->Initialize_Prototype()))
	{
		Safe_Release(pDropCompoent);
		MSG_BOX("CREATE FAIL : DROP COMPONENT");
	}
	return pDropCompoent;
}

CComponent* CDropComponent::Clone(void* pArg)
{
	CDropComponent* pDropCompoent = new CDropComponent(*this);
	if (FAILED(pDropCompoent->Initialize(pArg)))
	{
		Safe_Release(pDropCompoent);
		MSG_BOX("CLONE FAIL : DROP COMPONENT");
	}
	return pDropCompoent;
}

void CDropComponent::Free()
{
	__super::Free();
}
