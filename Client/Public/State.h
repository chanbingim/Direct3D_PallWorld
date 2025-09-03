#pragma once

#include "Client_Define.h"
#include "Base.h"

NS_BEGIN(Client)
class CState abstract : public CBase
{
protected :
	CState();
	virtual ~CState() = default;

public :
	virtual void OnEnterState(void* pArg = nullptr) = 0;
	virtual void PlayState(void* pArg = nullptr) = 0;
	virtual void OnEndState(void* pArg = nullptr) = 0;

public :
	virtual void	Free() override;

};
NS_END