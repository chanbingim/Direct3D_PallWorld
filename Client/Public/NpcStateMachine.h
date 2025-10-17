#pragma once
#include "Client_Define.h"
#include "FiniteStateMachine.h"

NS_BEGIN(Client)
class CNpc;

class CNpcStateMachine final : public CFiniteStateMachine
{
public:
	enum class NPC_DIALOG_ACTION { IDLE, TALK, LISTEN, END };

	typedef	struct	Npc_State
	{
		NPC_DIALOG_ACTION	eNpc_DialogState;
	}NPC_STATE;

protected:
	CNpcStateMachine();
	CNpcStateMachine(const CNpcStateMachine& rhs);
	virtual ~CNpcStateMachine() = default;

public:
	virtual		HRESULT					Initialize(void* pArg = nullptr) override;
	virtual		void					Update(_float DeltaTime, void* pArg = nullptr) override;

	const NPC_STATE&					GetState() const { return m_StateData; }
	_bool								ChangeState(const _wstring& LayerTag, const _wstring& StateTag, void* pArg = nullptr);

	_string								GetStateFullName();
	_bool								GetLayerLastPhase(const _wstring& LayerTag);

	void								NpcStateReset(const _wstring& LayerTag);

private:
	NPC_STATE							m_StateData = {};

private:
	HRESULT								ADD_NpcLayer();
	void								SettingNpcState(void* pArg = nullptr);

public:
	static		CNpcStateMachine*		Create();
	virtual		void					Free() override;
};
NS_END