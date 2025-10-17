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
	virtual		HRESULT							Initialize(void* pArg, _uInt iStateSize) override;
	virtual		void							Update(_float DeltaTime, void* pArg) override;

private:
	HRESULT										ADD_UpperState();


public:
	static	CPlayerUpperLayer*					Create(void* pArg, _uInt iStateSize);
	virtual	void								Free() override;
};
NS_END