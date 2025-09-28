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

    return S_OK;
}

const ITEM_DESC* CItemManager::GetItemInfo(_uInt ItemID)
{
    auto iter = m_Items.find(ItemID);
    if (iter == m_Items.end())
        return nullptr;

    return &iter->second;
}

const CTexture* CItemManager::GetItemTexture(_uInt ItemID)
{
    auto iter = m_ItemTextures.find(ItemID);
    if (iter == m_ItemTextures.end())
        return nullptr;

    return iter->second;
}

HRESULT CItemManager::LoadItemData(_bool bFlag, const char* FilePath)
{
    vector<_string> LoadEquipData = {};
    CSV_Read<const char>(FilePath, LoadEquipData);

    HRESULT hr = {};
    if (bFlag)
        hr =  ParseEuipData(LoadEquipData);
    else
        hr = ParseConsumeData(LoadEquipData);

    return hr;
}

HRESULT CItemManager::ParseEuipData(vector<_string>& Data)
{
    WCHAR ConvertName[MAX_PATH] = {};
    for (size_t i = 17; i < Data.size();)
    {
        ITEM_DESC Desc;
        Desc.iItemNum = atoi(Data[i++].c_str());
    
        CStringHelper::ConvertUTFToWide(Data[i++].c_str(), Desc.szItemName);
        CStringHelper::ConvertUTFToWide(Data[i++].c_str(), ConvertName);
        CStringHelper::ConvertUTFToWide(Data[i++].c_str(), Desc.szItemModelPath);
        
        Desc.ItemType = ITEM_TYPE(atoi(Data[i++].c_str()));
        Desc.IsAnimModel = atoi(Data[i++].c_str());
        Desc.IsPlayAnimation = atoi(Data[i++].c_str());

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

        m_ItemTextures.emplace(Desc.iItemNum, CTexture::Create(m_pDevice, m_pContext, ConvertName, 1));
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
        m_ItemTextures.emplace(Desc.iItemNum, CTexture::Create(m_pDevice, m_pContext, ConvertName, 1));
        m_Items.emplace(Desc.iItemNum, Desc);
    }
    return S_OK;
}

void CItemManager::Free()
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    for (auto pair : m_ItemTextures)
        Safe_Release(pair.second);

    m_ItemTextures.clear();
}
