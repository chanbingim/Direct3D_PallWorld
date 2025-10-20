#pragma once

#include "Client_Define.h"
#include "PellSkillStruct.h"
#include "Base.h"

NS_BEGIN(Client)
class CPellSkillManager : public CBase
{
	DECLARE_SINGLETON(CPellSkillManager);

private:
	CPellSkillManager();
	virtual ~CPellSkillManager() = default;

public:
	void									Initialize(const char* szFilePath = "");
	const PAL_SKILL_NETWORK_DATA*			FindPellData(_uInt iID);

private:
	unordered_map<_uInt, PAL_SKILL_NETWORK_DATA>	m_PellSkillDatas;

private:
	HRESULT									LoadCSVPellSkillData(const char* szFilePath);

public:
	virtual void							Free() override;
};
NS_END