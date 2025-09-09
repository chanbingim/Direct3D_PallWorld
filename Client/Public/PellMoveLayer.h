#pragma once
#include "Client_Define.h"
#include "StateLayer.h"

NS_BEGIN(Client)
class CPellMoveLayer : public CStateLayer
{
private:
	CPellMoveLayer();
	CPellMoveLayer(const CPellMoveLayer& rhs);
	virtual ~CPellMoveLayer() = default;

public:
	virtual		HRESULT							Initialize(_uInt iStateSize) override;
	virtual		void							Update(_float DeltaTime) override;

private:
	HRESULT										ADD_MoveState();

public:
	static	CPellMoveLayer*						Create(_uInt iStateSize);
	virtual	void								Free() override;

};
NS_END