#include "PellSkillManager.h"

IMPLEMENT_SINGLETON(CPellSkillManager);

CPellSkillManager::CPellSkillManager()
{
}

void CPellSkillManager::Initialize(const char* szFilePath)
{
    // 파일을 읽어들여 펠 정보를 로드할지 내가 테스용으로 임의의 값으로 사용할지
    // 여기서 결정해서 하는걸로 하자
    if (strcmp(szFilePath, ""))
        LoadCSVPellSkillData(szFilePath);
    else
    {
        // PELL_SAVE_DATA PellDesc;
        PELL_SKILL_DATA      PellSkillDesc;

        /* Sheep Ball Infomation */
        PellSkillDesc.SkillName = "Korogaru";
        PellSkillDesc.fAfterStateDelay = 0.5f;
        PellSkillDesc.iSkillDamage = 8.f;
        PellSkillDesc.fSkillMoveSpeed = 7.f;
        PellSkillDesc.fStartDurationTime = 0.f;
        PellSkillDesc.fSkillDurationTime = 3.f;
        PellSkillDesc.eSkillType = PELL_SKILL_TYPE::NORAML;

        m_PellSkillDatas.emplace(0, PellSkillDesc);
    }
}

const Pell_Skill_Data* CPellSkillManager::FindPellData(_uInt iID)
{
    auto iter = m_PellSkillDatas.find(iID);
    if (iter == m_PellSkillDatas.end())
        return nullptr;

	return &iter->second;
}

HRESULT CPellSkillManager::LoadCSVPellSkillData(const char* szFilePath)
{
	return S_OK;
}

void CPellSkillManager::Free()
{

}