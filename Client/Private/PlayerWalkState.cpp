#include "PlayerWalkState.h"
#include "PlayerStateMachine.h"


CPlayerWalkState::CPlayerWalkState()
{
}

void CPlayerWalkState::OnEnterState(void* pArg)
{



}

void CPlayerWalkState::PlayState(void* pArg)
{


}

void CPlayerWalkState::OnEndState(void* pArg)
{

   
}

CPlayerWalkState* CPlayerWalkState::Create()
{
    return new CPlayerWalkState();
}

void CPlayerWalkState::Free()
{
}
