#include "ContainerObject.h"

#include "GameInstance.h"
#include "PartObject.h"

CContainerObject::CContainerObject(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CActor(pGraphic_Device, pDeviceContext)
{
}

CContainerObject::CContainerObject(const CContainerObject& rhs) :
     CActor(rhs)
{
}

HRESULT CContainerObject::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CContainerObject::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CContainerObject::Priority_Update(_float fDeletaTime)
{
    for (auto& Pair : m_PartObjects)
        Pair.second->Priority_Update(fDeletaTime);
}

void CContainerObject::Update(_float fDeletaTime)
{
    for (auto& Pair : m_PartObjects)
        Pair.second->Update(fDeletaTime);
}

void CContainerObject::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    for (auto& Pair : m_PartObjects)
        Pair.second->Late_Update(fDeletaTime);
}

HRESULT CContainerObject::Render()
{

    return S_OK;
}

CPartObject* CContainerObject::FindPartObject(const _wstring& PartObjectTag)
{
    auto pair = m_PartObjects.find(PartObjectTag);
    if (pair == m_PartObjects.end())
        nullptr;

    return pair->second;
}

HRESULT CContainerObject::AddPartObject(_uInt iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strPartTag, void* pArg)
{
    if (nullptr != FindPartObject(strPartTag))
        return E_FAIL;

    CPartObject* pPartObject = m_pGameInstance->Clone_Prototype<CPartObject>(iPrototypeLevelIndex, strPrototypeTag, pArg);

    if (nullptr == pPartObject)
        return E_FAIL;

    m_PartObjects.emplace(strPartTag, pPartObject);
    return S_OK;
}

CGameObject* CContainerObject::Clone(void* pArg)
{
    return nullptr;
}

void CContainerObject::Free()
{
    __super::Free();

    for (auto& Pair : m_PartObjects)
        Safe_Release(Pair.second);

    m_PartObjects.clear();

}
