#include "ItemManager.h"
#include "GameInstance.h"

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
        Desc.TypeDesc.EuqipDesc.iGuardPoint = 0;
        Desc.TypeDesc.EuqipDesc.iHealthPoint = 0;
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
        Desc.TypeDesc.EuqipDesc.iGuardPoint = 0;
        Desc.TypeDesc.EuqipDesc.iHealthPoint = 0;
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
        Desc.TypeDesc.EuqipDesc.iGuardPoint = 0;
        Desc.TypeDesc.EuqipDesc.iHealthPoint = 0;
        Desc.TypeDesc.EuqipDesc.bIsLeftSocket = true;
        Desc.TypeDesc.EuqipDesc.bIsChargeAble = true;
        Desc.TypeDesc.EuqipDesc.iProjectileIndex = 10;
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

const CTexture* CItemManager::GetItemTexture(_uInt ItemID)
{
    auto iter = m_ItemTextures.find(ItemID);
    if (iter == m_ItemTextures.end())
        return nullptr;

    return iter->second;
}

HRESULT CItemManager::LoadItemData(const char* FilePath)
{
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
