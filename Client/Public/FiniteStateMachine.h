#pragma once

#include "Client_Define.h"
#include "Base.h"

NS_BEGIN(Client)
class CState;

class CFiniteStateMachine : public CBase
{
protected :
	CFiniteStateMachine();
	CFiniteStateMachine(const CFiniteStateMachine& rhs);
	virtual ~CFiniteStateMachine() = default;

public :
	virtual		HRESULT					Initialize();
	virtual		void					Update(_float DeltaTime);

	HRESULT								AddState(_uInt iIndex, const _wstring& StateTag, CState* pState);

protected :
	_uInt								m_MaxHeirary;
	unordered_map<_wstring, CState*>*	m_States;

protected :
	CState*								FindState(_uInt iIndex, const _wstring& StateTag);

public :
	virtual	void						Free() override;

};
NS_END