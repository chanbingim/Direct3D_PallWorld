#include "PellSkillManager.h"

#include "CsvHelper.h"

IMPLEMENT_SINGLETON(CPellSkillManager);

CPellSkillManager::CPellSkillManager()
{
}

void CPellSkillManager::Initialize(const char* szFilePath)
{
    // 파일을 읽어들여 펠 정보를 로드할지 내가 테스용으로 임의의 값으로 사용할지
    // 여기서 결정해서 하는걸로 하자
    LoadCSVPellSkillData(szFilePath);
}

const PAL_SKILL_NETWORK_DATA* CPellSkillManager::FindPellData(_uInt iID)
{
    auto iter = m_PellSkillDatas.find(iID);
    if (iter == m_PellSkillDatas.end())
        return nullptr;

	return &iter->second;
}

HRESULT CPellSkillManager::LoadCSVPellSkillData(const char* szFilePath)
{
    vector<_string> LoadData = {};
    CSV_Read<const char>(szFilePath, LoadData);

    //나중에 여기서 CSV 파서 받아서 연결할거임
    PAL_SKILL_NETWORK_DATA Desc;

    WCHAR ConvertName[MAX_PATH] = {};
    for (size_t i = 12; i < LoadData.size();)
    {
        Desc.iSkillID = atoi(LoadData[i++].c_str());
        strcpy_s(Desc.szSkillName, LoadData[i++].c_str());

        Desc.iSkillDelay = atoi(LoadData[i++].c_str());
        Desc.iSkillDamage = atoi(LoadData[i++].c_str());
        Desc.fSkillMoveSpeed = atoi(LoadData[i++].c_str());
        Desc.fStartDurationTime = atoi(LoadData[i++].c_str());
        Desc.fSkillDurationTime = atoi(LoadData[i++].c_str());
        Desc.fAfterStateDelay = atoi(LoadData[i++].c_str());
        Desc.bIsProjectTile = atoi(LoadData[i++].c_str());
        Desc.eSkillDamageType = PELL_SKILL_DAMAGE_TYPE(atoi(LoadData[i++].c_str()));
        Desc.fTickDamageTime = atoi(LoadData[i++].c_str());
        Desc.eSkillType = PELL_SKILL_TYPE(atoi(LoadData[i++].c_str()));
        m_PellSkillDatas.emplace(Desc.iSkillID, Desc);
    }
    return S_OK;
}

void CPellSkillManager::Free()
{

}