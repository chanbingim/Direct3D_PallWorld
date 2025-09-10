#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CState;

class ENGINE_DLL CStateLayer : public CBase
{
protected:
	CStateLayer();
	CStateLayer(const CStateLayer& rhs);
	virtual ~CStateLayer() = default;

public:
	virtual		HRESULT							Initialize(void* pArg, _uInt iStateSize);
	virtual		void							Update(_float DeltaTime);

	HRESULT										AddState(const _wstring& StateTag, CState* pNewState);
	_Int										GetStateNum(const _wstring& StateTag);
	
	_Int										GetCurrentStateNum();
	const char*									GetCurStateName();

	HRESULT										ChangeState(const _wstring& StateTag);
	_uInt										StateNextPhase();
	_uInt										GetCurStatePhase();

	void										ResetLayer();

protected:
	unordered_map<_wstring, _uInt>				m_StatesHasMap;
	vector<CState*>								m_States;

	_uInt										m_iCurStateIndex = {};
	CState*										m_pCurState = nullptr;

protected:
	pair<CState*, _Int>							FindState(const _wstring& StateTag);

public:
	virtual	void								Free() override;
};
NS_END