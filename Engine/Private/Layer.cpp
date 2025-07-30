#include "Layer.h"
#include "GameObject.h"

HRESULT CLayer::Initialize_Layer()
{
	return S_OK;
}

void CLayer::Priority_Update(_float fDeletaTime)
{
	for (auto iter : m_pGameObjects)
		iter->Priority_Update(fDeletaTime);
}

void CLayer::Update(_float fDeletaTime)
{
	for (auto iter : m_pGameObjects)
		iter->Update(fDeletaTime);
}

void CLayer::Late_Update(_float fDeletaTime)
{
	for (auto iter : m_pGameObjects)
		iter->Late_Update(fDeletaTime);
}

HRESULT CLayer::ADD_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_pGameObjects.push_back(pGameObject);
	return S_OK;
}

CGameObject* CLayer::Find_GameObject(_uInt Index)
{
	auto iter = m_pGameObjects.begin();
	for (_uInt i = 0; i < Index; ++i)
		iter++;

	return *iter;
}

const list<CGameObject*>* CLayer::GetAllObjects()
{
	return &m_pGameObjects;
}

void CLayer::Clear_DeadObject()
{
	auto iter_end = m_pGameObjects.end();
	for (auto iter = m_pGameObjects.begin(); iter != iter_end;)
	{
		if ((*iter)->IsDead())
		{
			Safe_Release((*iter));
			iter = m_pGameObjects.erase(iter);
		}
		else
			iter++;
	}
}

void CLayer::Clear_Resource()
{
	for (auto iter : m_pGameObjects)
	{
		Safe_Release(iter);
	}
	m_pGameObjects.clear();
}

CLayer* CLayer::Create()
{
	CLayer* pLayer = new CLayer();
	if (FAILED(pLayer->Initialize_Layer()))
	{
		Safe_Release(pLayer);
		MSG_BOX("CREATE FAIL : LAYER");
	}
	return pLayer;
}

void CLayer::Free()
{
	for (auto iter : m_pGameObjects)
	{
		Safe_Release(iter);
	}
	m_pGameObjects.clear();
}
