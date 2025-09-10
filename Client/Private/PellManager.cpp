#include "PellManager.h"

IMPLEMENT_SINGLETON(CPellManager);

CPellManager::CPellManager()
{
}

void CPellManager::Initialize(const char* szFilePath)
{
    // 파일을 읽어들여 펠 정보를 로드할지 내가 테스용으로 임의의 값으로 사용할지
    // 여기서 결정해서 하는걸로 하자
    if (!strcmp(szFilePath, ""))
    {

    }
    else
    {
        PELL_SAVE_DATA PellDesc;

        /* Sheep Ball Infomation */
        PellDesc.PellID = 0;
        PellDesc.szPellName = "Dororong";
        PellDesc.MaxHealth = 100.f;
        PellDesc.MaxHunger = 100.f;
        PellDesc.MaxStemina = 100.f;
        PellDesc.ShieldPoint = 100;
        PellDesc.eWorkType = PELL_WORK_TYPE::MAKE;
        PellDesc.ePellType = PELL_TYPE::NORMAL;
        m_PellDatas.emplace(PellDesc.PellID, PellDesc);
    }
}

const PELL_SAVE_DATA* CPellManager::FindPellData(_uInt iID)
{
    // TODO: 여기에 return 문을 삽입합니다.
    auto iter = m_PellDatas.find(iID);
    if (iter == m_PellDatas.end())
        return nullptr;

    return &iter->second;
}

void CPellManager::Free()
{
}
