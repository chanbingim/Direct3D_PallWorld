#include "ItemManager.h"

IMPLEMENT_SINGLETON(CItemManager);

HRESULT CItemManager::Initialize(const char* FilePath)
{
    if (!strcmp(FilePath, ""))
    {
        ITEM_DESC Desc;
        Desc.iItemNum = 1;
        Desc.ItemType = ITEM_TYPE::EQUIPMENT;
        lstrcpy(Desc.szItemName, TEXT("Pink Cat Blade"));

        // 이거 고민이네
        // 프로토타입 이름을 넘길지 실제파일의 경로를 넘길지
        lstrcpy(Desc.szItemIconPath, TEXT(""));
        lstrcpy(Desc.szItemModelPath, TEXT("Prototype_Component_VIBuffer_CatBlade"));
        Desc.TypeDesc.EuqipDesc.iAtkPoint = 10;
        Desc.TypeDesc.EuqipDesc.iGuardPoint = 0;
        Desc.TypeDesc.EuqipDesc.iHealthPoint = 0;

        Desc.TypeDesc.EuqipDesc.Equip_Type = EUQIP_TYPE::WEAPON;
        Desc.TypeDesc.EuqipDesc.Weapon_Type = WEAPON::MELEE;
        m_Items.emplace(0, Desc);
    }
    else
    {
        // 넘겨받을 녀석의 저장 파일이있다면
        // 저장된 파일의 정보를 읽어와서 파싱
        if(FAILED(LoadItemData(FilePath)))
            return E_FAIL;
    }
    return S_OK;
}

const ITEM_DESC* CItemManager::GetItemInfo(_uInt ItemID)
{
    auto iter = m_Items.find(ItemID);
    if (iter == m_Items.end())
        return nullptr;

    return &iter->second;
}

HRESULT CItemManager::LoadItemData(const char* FilePath)
{
    return S_OK;
}

void CItemManager::Free()
{

}
