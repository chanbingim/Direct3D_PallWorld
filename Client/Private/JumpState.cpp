#include "JumpState.h"

CJumpState::CJumpState(const char* szStateName) :
	CState(szStateName)
{
}

void CJumpState::OnStateEnter(void* pArg)
{
	if (nullptr == pArg)
	{

	}
	else
	{
		JUMPSTATE_DESC* pDesc = static_cast<JUMPSTATE_DESC*>(pArg);
		m_fJumpSpeed = pDesc->fAnimSpeed;

		*m_fJumpSpeed = 0.5f;
	}

	m_szStateName = "JumpStart";
	m_iPhaseIndex = 0;
}

void CJumpState::OnStateExcution(_float fDeletaTime, void* pArg)
{
	// 애니메이션 진행 위치와 포물선 공식의 힘의 방향에 따라서
	// 재생할 애니메이션의 이름을 바꿔주자
	ChangeStateName();
}

void CJumpState::OnStateExit(void* pArg)
{
	*m_fJumpSpeed = 0.5f;
	m_fJumpSpeed = nullptr;
}

void CJumpState::ChangeStateName()
{
	switch (m_iPhaseIndex)
	{
	case ENUM_CLASS(ACTION_PHASE::STARTLOOP):
		m_szStateName = "JumpUpLoop";
		break;
	case ENUM_CLASS(ACTION_PHASE::ENDLOOP):
		m_szStateName = "JumpDwonLoop";
		break;
	case ENUM_CLASS(ACTION_PHASE::END):
		m_szStateName = "JumpEnd";
		break;
	}
}

CJumpState* CJumpState::Create(const char* szStateName)
{
	return new CJumpState(szStateName);
}

void CJumpState::Free()
{

}
