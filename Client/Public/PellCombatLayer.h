#pragma once
#include "Client_Define.h"
#include "StateLayer.h"

NS_BEGIN(Client)
class CPellCombatLayer : public CStateLayer
{
private:
	CPellCombatLayer();
	CPellCombatLayer(const CPellCombatLayer& rhs);
	virtual ~CPellCombatLayer() = default;

public:
	virtual		HRESULT							Initialize(void* pArg, _uInt iStateSize) override;
	virtual		void							Update(_float DeltaTime, void* pArg) override;

private:
	HRESULT										ADD_CombatState();

public:
	static	CPellCombatLayer*					Create(void* pArg, _uInt iStateSize);
	virtual	void								Free() override;
};
NS_END