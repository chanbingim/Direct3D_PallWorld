#pragma once
#include "Client_Define.h"
#include "StateLayer.h"

NS_BEGIN(Client)

class CPlayerNoneCombatLayer : public CStateLayer
{
private:
	CPlayerNoneCombatLayer();
	CPlayerNoneCombatLayer(const CPlayerNoneCombatLayer& rhs);
	virtual ~CPlayerNoneCombatLayer() = default;

public:
	virtual		HRESULT							Initialize(void* pArg, _uInt iStateSize) override;
	virtual		void							Update(_float DeltaTime) override;

private:
	HRESULT										ADD_CombatState();

public:
	static	CPlayerNoneCombatLayer*				Create(void* pArg, _uInt iStateSize);
	virtual	void								Free() override;
};
NS_END