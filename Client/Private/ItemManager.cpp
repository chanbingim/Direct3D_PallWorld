#include "ItemManager.h"

#include "GameInstance.h"
#include "StringHelper.h"
#include "CsvHelper.h"

IMPLEMENT_SINGLETON(CItemManager);

HRESULT CItemManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* FilePath)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);

    LoadItemData(0, "../Bin/Resources/DataFile/Item/Equip/WeaponData.csv");
    LoadItemData(1, "../Bin/Resources/DataFile/Item/consum/Item.csv");
    LoadItemData(2, "../Bin/Resources/DataFile/Item/Architecture/Architecture.csv");

    return S_OK;
}

const ITEM_DESC* CItemManager::GetItemInfo(_uInt ItemID)
{
    auto iter = m_Items.find(ItemID);
    if (iter == m_Items.end())
        return nullptr;

    return &iter->second;
}

const CTexture* CItemManager::GetItemTexture(ITEM_TEXTURE_TYPE eTextureType, _uInt ItemID)
{
    auto iter = m_ItemTextures[ENUM_CLASS(eTextureType)].find(ItemID);
    if (iter == m_ItemTextures[ENUM_CLASS(eTextureType)].end())
        return nullptr;

    return iter->second;
}

HRESULT CItemManager::LoadItemData(_uInt bFlag, const char* FilePath)
{
    vector<_string> LoadEquipData = {};
    CSV_Read<const char>(FilePath, LoadEquipData);

    HRESULT hr = {};
    switch (bFlag)
    {
    case 0 :
        hr = ParseEuipData(LoadEquipData);
        break;
    case 1:
        hr = ParseConsumeData(LoadEquipData);
        break;
    case 2:
        hr = ParseArchitecturetData(LoadEquipData);
        break;
    }
    return hr;
}

HRESULT CItemManager::ParseEuipData(vector<_string>& Data)
{
    WCHAR ConvertIvenUIName[MAX_PATH] = {};
    WCHAR ConvertWeaponUIName[MAX_PATH] = {};

    for (size_t i = 18; i < Data.size();)
    {
        ITEM_DESC Desc;
        Desc.iItemNum = atoi(Data[i++].c_str());
    
        CStringHelper::ConvertUTFToWide(Data[i++].c_str(), Desc.szItemName);
        CStringHelper::ConvertUTFToWide(Data[i++].c_str(), ConvertIvenUIName);
        CStringHelper::ConvertUTFToWide(Data[i++].c_str(), Desc.szItemModelPath);
        
        Desc.ItemType = ITEM_TYPE(atoi(Data[i++].c_str()));
        Desc.IsAnimModel = atoi(Data[i++].c_str());
        Desc.IsPlayAnimation = atoi(Data[i++].c_str());

        CStringHelper::ConvertUTFToWide(Data[i++].c_str(), ConvertWeaponUIName);
        CStringHelper::ConvertUTFToWide(Data[i++].c_str(), Desc.TypeDesc.EuqipDesc.ProjectilePrototpyeName);
        Desc.TypeDesc.EuqipDesc.iAtkPoint = atoi(Data[i++].c_str());
        Desc.TypeDesc.EuqipDesc.iDurability = atoi(Data[i++].c_str());
        Desc.TypeDesc.EuqipDesc.bIsLeftSocket = atoi(Data[i++].c_str());
        Desc.TypeDesc.EuqipDesc.bIsChargeAble = atoi(Data[i++].c_str());
        Desc.TypeDesc.EuqipDesc.iProjectileItemIndex = atoi(Data[i++].c_str());

        Desc.TypeDesc.EuqipDesc.vCenter = {  (_float)atof(Data[i++].c_str()), (_float)atof(Data[i++].c_str()), (_float)atof(Data[i++].c_str()) };
        Desc.TypeDesc.EuqipDesc.vExtents = { (_float)atof(Data[i++].c_str()), (_float)atof(Data[i++].c_str()), (_float)atof(Data[i++].c_str()) };
        Desc.TypeDesc.EuqipDesc.Equip_Type = EUQIP_TYPE(atoi(Data[i++].c_str()));
        Desc.TypeDesc.EuqipDesc.Weapon_Type = WEAPON(atoi(Data[i++].c_str()));

        m_ItemTextures[ENUM_CLASS(ITEM_TEXTURE_TYPE::INVEN)].emplace(Desc.iItemNum, CTexture::Create(m_pDevice, m_pContext, ConvertIvenUIName, 1));
       
        if(lstrcmp(ConvertWeaponUIName, TEXT("0")))
            m_ItemTextures[ENUM_CLASS(ITEM_TEXTURE_TYPE::WEAPON)].emplace(Desc.iItemNum, CTexture::Create(m_pDevice, m_pContext, ConvertWeaponUIName, 1));
        m_Items.emplace(Desc.iItemNum, Desc);
    }

    return S_OK;
}

HRESULT CItemManager::ParseConsumeData(vector<_string>& Data)
{
    WCHAR ConvertName[MAX_PATH] = {};
    for (size_t i = 10; i < Data.size();)
    {
        ITEM_DESC Desc;
        Desc.iItemNum = atoi(Data[i++].c_str());
       
        CStringHelper::ConvertUTFToWide(Data[i++].c_str(), Desc.szItemName);
        CStringHelper::ConvertUTFToWide(Data[i++].c_str(), ConvertName);
        CStringHelper::ConvertUTFToWide(Data[i++].c_str(), Desc.szItemModelPath);
     
        Desc.ItemType = ITEM_TYPE(atoi(Data[i++].c_str()));
        Desc.IsAnimModel = atoi(Data[i++].c_str());
        Desc.IsPlayAnimation = atoi(Data[i++].c_str());

        Desc.TypeDesc.ConsumDesc.bIsLeftSocket = atoi(Data[i++].c_str());
        Desc.TypeDesc.ConsumDesc.iEffectType = atoi(Data[i++].c_str());
        Desc.TypeDesc.ConsumDesc.iRecoveryPoint = atoi(Data[i++].c_str());
        
        m_ItemTextures[ENUM_CLASS(ITEM_TEXTURE_TYPE::INVEN)].emplace(Desc.iItemNum, CTexture::Create(m_pDevice, m_pContext, ConvertName, 1));
        m_Items.emplace(Desc.iItemNum, Desc);
    }
    return S_OK;
}

HRESULT CItemManager::ParseArchitecturetData(vector<_string>& Data)
{
    WCHAR ConvertName[MAX_PATH] = {};
    for (size_t i = 11; i < Data.size();)
    {
        ITEM_DESC Desc;
        Desc.iItemNum = atoi(Data[i++].c_str());

        CStringHelper::ConvertUTFToWide(Data[i++].c_str(), Desc.szItemName);
        CStringHelper::ConvertUTFToWide(Data[i++].c_str(), ConvertName);
        CStringHelper::ConvertUTFToWide(Data[i++].c_str(), Desc.szItemModelPath);

        Desc.ItemType = ITEM_TYPE(atoi(Data[i++].c_str()));
        Desc.IsAnimModel = atoi(Data[i++].c_str());
        Desc.IsPlayAnimation = atoi(Data[i++].c_str());

        CStringHelper::ConvertUTFToWide(Data[i++].c_str(), Desc.TypeDesc.ArchitectureDesc.ArchitecturePrototpyeName);
        Desc.TypeDesc.ArchitectureDesc.fCompleteTime = atoi(Data[i++].c_str());
        Desc.TypeDesc.ArchitectureDesc.iWorkPartner = atoi(Data[i++].c_str());
        Desc.TypeDesc.ArchitectureDesc.iCellTypes = atoi(Data[i++].c_str());
        m_ItemTextures[ENUM_CLASS(ITEM_TEXTURE_TYPE::INVEN)].emplace(Desc.iItemNum, CTexture::Create(m_pDevice, m_pContext, ConvertName, 1));
        m_Items.emplace(Desc.iItemNum, Desc);
    }
    return S_OK;
}

void CItemManager::Free()
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    for (_uInt i = 0; i < ENUM_CLASS(ITEM_TEXTURE_TYPE::END); ++i)
    {
        for (auto pair : m_ItemTextures[i])
            Safe_Release(pair.second);

        m_ItemTextures[i].clear();
    }
}
