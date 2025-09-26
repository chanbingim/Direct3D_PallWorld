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

    if (!strcmp(FilePath, ""))
    {
        //이거도 나중에 테스트 끝나고 정상화되면 다 데이터 파일로 바꿔서 보관할거
        ITEM_DESC Desc;
        Desc.iItemNum = 1;
        Desc.ItemType = ITEM_TYPE::EQUIPMENT;
        lstrcpy(Desc.szItemName, TEXT("Pink Cat Blade"));

        // 이거 고민이네
        // 프로토타입 이름을 넘길지 실제파일의 경로를 넘길지
        
        m_ItemTextures.emplace(Desc.iItemNum, CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/WeaponUI/WeaponIcon/T_icon_YakushimaBlade_UI.png"), 1));
        lstrcpy(Desc.szItemModelPath, TEXT("Prototype_Component_VIBuffer_CatBlade"));
        Desc.IsAnimModel = false;

        lstrcpy(Desc.TypeDesc.EuqipDesc.ProjectilePrototpyeName, TEXT(""));
        Desc.TypeDesc.EuqipDesc.iAtkPoint = 10;
        Desc.TypeDesc.EuqipDesc.iDurability = 100;
        Desc.TypeDesc.EuqipDesc.bIsLeftSocket = false;
        Desc.TypeDesc.EuqipDesc.bIsChargeAble = false;
        Desc.TypeDesc.EuqipDesc.vCenter = {0.f, 0.5f, 0.f};
        Desc.TypeDesc.EuqipDesc.vExtents = {0.2f, 0.5f, 0.2f};
        Desc.TypeDesc.EuqipDesc.Equip_Type = EUQIP_TYPE::WEAPON;
        Desc.TypeDesc.EuqipDesc.Weapon_Type = WEAPON::MELEE;
        m_Items.emplace(1, Desc);

        Desc.iItemNum = 2;
        Desc.ItemType = ITEM_TYPE::EQUIPMENT;
        lstrcpy(Desc.szItemName, TEXT("Pell Spher"));

        // 이거 고민이네
        // 프로토타입 이름을 넘길지 실제파일의 경로를 넘길지
        m_ItemTextures.emplace(Desc.iItemNum, CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/WeaponUI/WeaponIcon/T_icon_pal_sphere.png"), 1));
        lstrcpy(Desc.szItemModelPath, TEXT("Prototype_Component_VIBuffer_PalSpher"));
        Desc.IsAnimModel = true;

        lstrcpy(Desc.TypeDesc.EuqipDesc.ProjectilePrototpyeName, TEXT("Prototype_GameObject_PalSpher"));
        Desc.TypeDesc.EuqipDesc.iAtkPoint = 10;
        Desc.TypeDesc.EuqipDesc.iDurability = 100;
        Desc.TypeDesc.EuqipDesc.bIsLeftSocket = false;
        Desc.TypeDesc.EuqipDesc.bIsChargeAble = false;
        Desc.TypeDesc.EuqipDesc.vCenter = {  };
        Desc.TypeDesc.EuqipDesc.vExtents = { };
        Desc.TypeDesc.EuqipDesc.Equip_Type = EUQIP_TYPE::WEAPON;
        Desc.TypeDesc.EuqipDesc.Weapon_Type = WEAPON::THROW;
        m_Items.emplace(2, Desc);

        Desc.iItemNum = 3;
        Desc.ItemType = ITEM_TYPE::EQUIPMENT;
        lstrcpy(Desc.szItemName, TEXT("Bow"));

        // 이거 고민이네
        // 프로토타입 이름을 넘길지 실제파일의 경로를 넘길지
        m_ItemTextures.emplace(Desc.iItemNum, CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/WeaponUI/WeaponIcon/T_icon_Bow_UI.png"), 1));
        lstrcpy(Desc.szItemModelPath, TEXT("Prototype_Component_VIBuffer_Bow"));
        Desc.IsAnimModel = true;
        Desc.IsPlayAnimation = true;

        lstrcpy(Desc.TypeDesc.EuqipDesc.ProjectilePrototpyeName, TEXT("Prototype_GameObject_ArrowProjectile"));
        Desc.TypeDesc.EuqipDesc.iAtkPoint = 10;
        Desc.TypeDesc.EuqipDesc.iDurability = 100;
        Desc.TypeDesc.EuqipDesc.bIsLeftSocket = true;
        Desc.TypeDesc.EuqipDesc.bIsChargeAble = true;
        Desc.TypeDesc.EuqipDesc.iProjectileItemIndex = 10;
        Desc.TypeDesc.EuqipDesc.vCenter = {  };
        Desc.TypeDesc.EuqipDesc.vExtents = {  };
        Desc.TypeDesc.EuqipDesc.Equip_Type = EUQIP_TYPE::WEAPON;
        Desc.TypeDesc.EuqipDesc.Weapon_Type = WEAPON::BOW;
        m_Items.emplace(3, Desc);

        ZeroMemory(&Desc, sizeof(ITEM_DESC));
        Desc.iItemNum = 10;
        Desc.ItemType = ITEM_TYPE::CONSUM;
        lstrcpy(Desc.szItemName, TEXT("Arrow"));

        // 이거 고민이네
        // 프로토타입 이름을 넘길지 실제파일의 경로를 넘길지
        lstrcpy(Desc.szItemIconPath, TEXT(""));
        lstrcpy(Desc.szItemModelPath, TEXT("Prototype_Component_VIBuffer_Arrow"));
        Desc.IsAnimModel = false;
        Desc.IsPlayAnimation = false;
        Desc.TypeDesc.ConsumDesc.bIsLeftSocket = false;
        m_Items.emplace(10, Desc);
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
