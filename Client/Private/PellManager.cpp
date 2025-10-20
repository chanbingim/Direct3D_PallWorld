#include "PellManager.h"
#include "PellSkillManager.h"

#include "GameInstance.h"
#include "StringHelper.h"
#include "CsvHelper.h"

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

    CPellSkillManager::GetInstance()->Initialize("../Bin/Resources/DataFile/Pal/PalSkill/PalSkill.csv");
    // 파일을 읽어들여 펠 정보를 로드할지 내가 테스용으로 임의의 값으로 사용할지
    // 여기서 결정해서 하는걸로 하자
    if (strcmp(szFilePath, ""))
        LoadCSVPellData(szFilePath);
    else
    {
        //// PELL_SAVE_DATA PellDesc;
        //PELL_INFO      PellDesc;
        ///* Sheep Ball Infomation */
        //PellDesc.szPellName = "Dororong";
        //PellDesc.pPellIconTexture = CreateTexture(0, TEXT("../Bin/Resources/Textures/UI/InGameUI/PellCharacter/T_SheepBall_icon_normal.png"));
        //PellDesc.MaxHealth = 100.f;
        //PellDesc.MaxHunger = 100.f;
        //PellDesc.fPellAttackRange = 100.f;
        //PellDesc.fPellActTime = 5.f;
        //PellDesc.DefaultSkill = *CPellSkillManager::GetInstance()->FindPellData(0);
        //PellDesc.fPellWalkSpeed = 1.5f;
        //PellDesc.fPellRunSpeed = 5.f;

        //PellDesc.MaxStemina = 100.f;
        //PellDesc.ShieldPoint = 100;
        //PellDesc.eWorkType = PELL_WORK_TYPE::COMMONE_WORK;
        //PellDesc.ePellType = PELL_TYPE::NORMAL;
        //m_PellDatas.emplace(0, PellDesc);

        ///* Pink Cat Infomation */
        //PellDesc.szPellName = "PinkCat";
        //PellDesc.pPellIconTexture = CreateTexture(1, TEXT("../Bin/Resources/Textures/UI/InGameUI/PellCharacter/T_PinkCat_icon_normal.png"));
        //PellDesc.MaxHealth = 150.f;
        //PellDesc.MaxHunger = 100.f;
        //PellDesc.fPellAttackRange = 50.f;
        //PellDesc.fPellActTime = 6.f;
        //PellDesc.DefaultSkill = *CPellSkillManager::GetInstance()->FindPellData(1);
        //PellDesc.fPellWalkSpeed = 1.5f;
        //PellDesc.fPellRunSpeed = 5.f;

        //PellDesc.MaxStemina = 100.f;
        //PellDesc.ShieldPoint = 100;
        //PellDesc.eWorkType = PELL_WORK_TYPE::MINING;
        //PellDesc.ePellType = PELL_TYPE::NORMAL;
        //m_PellDatas.emplace(1, PellDesc);

        ///* ElecPanda Infomation */
        //PellDesc.szPellName = "ElecPanda";
        //PellDesc.pPellIconTexture = CreateTexture(2, TEXT("../Bin/Resources/Textures/UI/InGameUI/PellCharacter/T_ElecPanda_icon_normal.png"));
        //PellDesc.MaxHealth = 200.f;
        //PellDesc.MaxHunger = 70.f;
        //PellDesc.fPellAttackRange = 50.f;
        //PellDesc.fPellActTime = 6.f;
        //PellDesc.DefaultSkill = *CPellSkillManager::GetInstance()->FindPellData(2);
        //PellDesc.fPellWalkSpeed = 1.5f;
        //PellDesc.fPellRunSpeed = 5.f;

        //PellDesc.MaxStemina = 100.f;
        //PellDesc.ShieldPoint = 100;
        //PellDesc.eWorkType = PELL_WORK_TYPE::TREE_GATHER;
        //PellDesc.ePellType = PELL_TYPE::ELETRIC;
        //m_PellDatas.emplace(2, PellDesc);
    }
}

_bool	 CPellManager::FindPellData(_uInt iID, PELL_INFO* pPellInfo)
{
    // TODO: 여기에 return 문을 삽입합니다.
    auto iter = m_PellDatas.find(iID);
    if (iter == m_PellDatas.end())
        return false;

    memcpy(pPellInfo, &iter->second, sizeof(PAL_NETWORK_DATA));
    auto SkillManager = CPellSkillManager::GetInstance();

    pPellInfo->NickName = pPellInfo->szPellName;
    pPellInfo->DefaultSkill = *SkillManager->FindPellData(pPellInfo->iDefaultSkillIndex);
    pPellInfo->pPellIconTexture = m_PellIcons.find(iID)->second;
    return true;
}

HRESULT CPellManager::LoadCSVPellData(const char* szFilePath)
{
    vector<_string> LoadData = {};
    CSV_Read<const char>(szFilePath, LoadData);

    //나중에 여기서 CSV 파서 받아서 연결할거임
    WCHAR ConvertName[MAX_PATH] = {};
    for (size_t i = 16; i < LoadData.size();)
    {
        PAL_NETWORK_DATA Desc;
        Desc.MaxHealth = atoi(LoadData[i++].c_str());
        Desc.MaxStemina = atoi(LoadData[i++].c_str());
        Desc.MaxHunger = atoi(LoadData[i++].c_str());

        Desc.ATKPoint = atoi(LoadData[i++].c_str());
        Desc.WorkSpeed = atoi(LoadData[i++].c_str());

        Desc.iPellID = atoi(LoadData[i++].c_str());
        CStringHelper::ConvertUTFToWide(LoadData[i++].c_str(), Desc.szPellName);
        CStringHelper::ConvertUTFToWide(LoadData[i++].c_str(), ConvertName);
        CStringHelper::ConvertUTFToWide(LoadData[i++].c_str(), Desc.szPrototyeName);
        
        Desc.fPellAttackRange = atoi(LoadData[i++].c_str());
        Desc.fPellActTime = atoi(LoadData[i++].c_str());
        Desc.fPellWalkSpeed = atoi(LoadData[i++].c_str());
        Desc.fPellRunSpeed = atoi(LoadData[i++].c_str());
        Desc.iDefaultSkillIndex = atoi(LoadData[i++].c_str());
        Desc.eWorkType = PELL_WORK_TYPE(atoi(LoadData[i++].c_str()));
        Desc.ePellType = PELL_TYPE(atoi(LoadData[i++].c_str()));

        m_PellIcons.emplace(Desc.iPellID, CTexture::Create(m_pDevice, m_pContext, ConvertName, 1));
        m_PellDatas.emplace(Desc.iPellID, Desc);
    }
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
