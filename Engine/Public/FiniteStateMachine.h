#pragma once

#include "Base.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CActor;
class CStateLayer;

class ENGINE_DLL CFiniteStateMachine : public CBase
{
public :
	typedef struct FSM_Desc
	{
		CActor*						pOwner;
		_uInt						iLayerSize;
	}FSM_DESC;

protected :
	CFiniteStateMachine();
	CFiniteStateMachine(const CFiniteStateMachine& rhs);
	virtual ~CFiniteStateMachine() = default;

public :
	virtual		HRESULT							Initialize(void* pArg = nullptr);
	virtual		void							Update(_float DeltaTime, void* pArg = nullptr);

	HRESULT										AddLayer(const _wstring& LayerTag, CStateLayer* pStateLayer);
	_Int										GetNumLayer(const _wstring& LayerTag);
	void										ResetLayer(const _wstring& LayerTag);
	
	_uInt										NextStatePhase(const _wstring& LayerTag);
	_uInt										GetStatePhase(const _wstring& LayerTag);
	const _bool									GetLayerAnimLoop(const _wstring& LayerTag);

protected :
	CActor*										m_pOwner = nullptr;
	unordered_map<_wstring, _uInt>				m_LayerHashMap;
	vector<CStateLayer*>						m_StateLayers;

protected :
	CStateLayer*								FindLayer(const _wstring& LayerTag);

public :
	virtual	void								Free() override;

};
NS_END