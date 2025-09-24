#include "PellAttackState.h"
#include "PellBase.h"

CPellAttackState::CPellAttackState(const char* szStateName) :
    CState(szStateName)
{
    m_iLastPhase = 4;
}

void CPellAttackState::OnStateEnter(void* pArg)
{
    PELL_ATTACK_STATE_DESC*  Desc = static_cast<PELL_ATTACK_STATE_DESC*>(pArg);

    m_IsSpaceOut = Desc->IsSpaceOut;
    m_pActPell = Desc->ActPell;
    m_AttackData = *Desc->AttackData;

    if (nullptr != Desc->fSkillMoveSpeed)
    {
        m_fSkillMovePtr = Desc->fSkillMoveSpeed;
        *m_fSkillMovePtr = 0.f;
    }

    m_bStateAnimLoop = false;
    m_iPhaseIndex = 0;
    m_fAccTime = 0.f;
}

void CPellAttackState::OnStateExcution(_float fDeletaTime, void* pArg)
{
    if (m_IsSpaceOut)
    {
        // 여기서 거리를 벌리는 행위를 한다.



    }
    else
    {
        m_szStateName = m_AttackData.SkillName;
        SkillMotionChange(fDeletaTime);

        switch (m_iPhaseIndex)
        {
        case 0 :
            m_szStateName += "_Start";
            break;
        case 1:
            m_szStateName += "_StartLoop";
            break;
        case 2:
            if (PELL_SKILL_TYPE::NORAML == m_AttackData.eSkillType)
            {
                m_szStateName += "_Loop";
                *m_fSkillMovePtr = m_AttackData.fSkillMoveSpeed;
            }
            else
                m_szStateName += "_Action";
            break;
        case 3:
        {
            m_szStateName += "_ActionLoop";
            *m_fSkillMovePtr = m_AttackData.fSkillMoveSpeed;
        }
            break;
        case 4:
            m_szStateName += "_End";
            *m_fSkillMovePtr = 0;
            break;
        }
    }
}

void CPellAttackState::OnStateExit(void* pArg)
{
    m_pActPell = nullptr;
    m_fSkillMovePtr = nullptr;
}

void CPellAttackState::SkillMotionChange(_float fTimeDeleta)
{
    if (false == m_bStateAnimLoop && m_pActPell->GetFinisehdAnimation())
    {
        if (PELL_SKILL_TYPE::SPECIAL == m_AttackData.eSkillType)
        {
            m_iPhaseIndex++;
            if (1 == m_iPhaseIndex || 3 == m_iPhaseIndex)
            {
                m_bStateAnimLoop = true;
                if (1 == m_iPhaseIndex)
                    m_DurationTime = m_AttackData.fStartDurationTime;
                else if (3 == m_iPhaseIndex)
                    m_DurationTime = m_AttackData.fSkillDurationTime;
            }
        }
        else
        {
            m_iPhaseIndex += 2;
            m_bStateAnimLoop = true;
            m_DurationTime = m_AttackData.fSkillDurationTime;
        }
    }

    if(m_bStateAnimLoop)
    {
        m_fAccTime += fTimeDeleta;
        if (m_fAccTime >= m_DurationTime)
        {
            if (PELL_SKILL_TYPE::SPECIAL == m_AttackData.eSkillType)
                m_iPhaseIndex++;
            else
                m_iPhaseIndex += 2;

            m_fAccTime = 0.f;
            m_DurationTime = 0.0f;
            m_bStateAnimLoop = false;
        }
    }
}

CPellAttackState* CPellAttackState::Create(const char* szStateName)
{
    return new CPellAttackState(szStateName);
}

void CPellAttackState::Free()
{
    __super::Free();
}
