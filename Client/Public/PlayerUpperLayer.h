#pragma once
#include "Client_Define.h"
#include "StateLayer.h"

NS_BEGIN(Client)
class CPlayerUpperLayer : public CStateLayer
{
private:
	CPlayerUpperLayer();
	CPlayerUpperLayer(const CPlayerUpperLayer& rhs);
	virtual ~CPlayerUpperLayer() = default;

public:
	virtual		HRESULT							Initialize(_uInt iStateSize) override;
	virtual		void							Update(_float DeltaTime) override;

private:
	HRESULT										ADD_UpperState();


public:
	static	CPlayerUpperLayer*					Create(_uInt iStateSize);
	virtual	void								Free() override;
};
NS_END