#pragma once
#include "Client_Define.h"
#include "StateLayer.h"

NS_BEGIN(Client)
class CPlayerCombatLayer : public CStateLayer
{
private:
	CPlayerCombatLayer();
	CPlayerCombatLayer(const CPlayerCombatLayer& rhs);
	virtual ~CPlayerCombatLayer() = default;

public:
	virtual		HRESULT							Initialize(_uInt iStateSize) override;
	virtual		void							Update(_float DeltaTime) override;

private:
	HRESULT										ADD_CombatState();

public:
	static	CPlayerCombatLayer*						Create(_uInt iStateSize);
	virtual	void								Free() override;
};
NS_END