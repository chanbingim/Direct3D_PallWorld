#pragma once
#include "Client_Define.h"
#include "StateLayer.h"

NS_BEGIN(Client)
class CPlayerLowerLayer : public CStateLayer
{
private :
	CPlayerLowerLayer();
	CPlayerLowerLayer(const CPlayerLowerLayer& rhs);
	virtual ~CPlayerLowerLayer() = default;

public:
	virtual		HRESULT							Initialize(_uInt iStateSize) override;
	virtual		void							Update(_float DeltaTime) override;

private :
	HRESULT										ADD_LowerState();

public:
	static	CPlayerLowerLayer*					Create(_uInt iStateSize);
	virtual	void								Free() override;
};
NS_END