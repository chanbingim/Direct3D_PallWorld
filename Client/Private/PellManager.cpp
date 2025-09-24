#include "PellManager.h"
#include "PellSkillManager.h"

#include "GameInstance.h"

IMPLEMENT_SINGLETON(CPellManager);

CPellManager::CPellManager()
{
}

void CPellManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* szFilePath)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);

    CPellSkillManager::GetInstance()->Initialize("");
    // 파일을 읽어들여 펠 정보를 로드할지 내가 테스용으로 임의의 값으로 사용할지
    // 여기서 결정해서 하는걸로 하자
    if (strcmp(szFilePath, ""))
    {
        LoadCSVPellData(szFilePath);
    }
    else
    {
        // PELL_SAVE_DATA PellDesc;
        PELL_INFO      PellDesc;

        /* Sheep Ball Infomation */
        PellDesc.szPellName = "Dororong";
        PellDesc.pPellIconTexture = CreateTexture(0, TEXT("../Bin/Resources/Textures/UI/InGameUI/PellCharacter/T_SheepBall_icon_normal.png"));
        PellDesc.MaxHealth = 100.f;
        PellDesc.MaxHunger = 100.f;
        PellDesc.fPellAttackRange = 100.f;
        PellDesc.fPellActTime = 5.f;
        PellDesc.DefaultSkill = *CPellSkillManager::GetInstance()->FindPellData(0);
        PellDesc.fPellWalkSpeed = 2.5f;
        PellDesc.fPellRunSpeed = 10.f;

        PellDesc.MaxStemina = 100.f;
        PellDesc.ShieldPoint = 100;
        PellDesc.eWorkType = PELL_WORK_TYPE::MAKE;
        PellDesc.ePellType = PELL_TYPE::NORMAL;
        m_PellDatas.emplace(0, PellDesc);

        /* Pink Cat Infomation */
        PellDesc.szPellName = "PinkCat";
        PellDesc.pPellIconTexture = CreateTexture(1, TEXT("../Bin/Resources/Textures/UI/InGameUI/PellCharacter/T_PinkCat_icon_normal.png"));
        PellDesc.MaxHealth = 150.f;
        PellDesc.MaxHunger = 100.f;
        PellDesc.fPellAttackRange = 50.f;
        PellDesc.fPellActTime = 6.f;
        PellDesc.DefaultSkill = *CPellSkillManager::GetInstance()->FindPellData(1);
        PellDesc.fPellWalkSpeed = 2.5f;
        PellDesc.fPellRunSpeed = 10.f;

        PellDesc.MaxStemina = 100.f;
        PellDesc.ShieldPoint = 100;
        PellDesc.eWorkType = PELL_WORK_TYPE::MINING;
        PellDesc.ePellType = PELL_TYPE::NORMAL;
        m_PellDatas.emplace(1, PellDesc);
    }
}

const PELL_INFO* CPellManager::FindPellData(_uInt iID)
{
    // TODO: 여기에 return 문을 삽입합니다.
    auto iter = m_PellDatas.find(iID);
    if (iter == m_PellDatas.end())
        return nullptr;

    return &iter->second;
}

HRESULT CPellManager::LoadCSVPellData(const char* szFilePath)
{
    //나중에 여기서 CSV 파서 받아서 연결할거임

    return S_OK;
}

const CTexture* CPellManager::CreateTexture(_uInt iID, const WCHAR* szFilePath)
{
    auto pTexture = CTexture::Create(m_pDevice, m_pContext, szFilePath, 1);
    if (nullptr == pTexture)
        return nullptr;

    m_PellIcons.emplace(iID, pTexture);
    return pTexture;
}

void CPellManager::Free()
{
    for (auto& pair : m_PellIcons)
        Safe_Release(pair.second);

    m_PellIcons.clear();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
