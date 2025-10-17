#pragma once
#include "Client_Define.h"
#include "StateLayer.h"

NS_BEGIN(Client)
class CDialogLayer : public CStateLayer
{
private:
	CDialogLayer();
	CDialogLayer(const CDialogLayer& rhs);
	virtual ~CDialogLayer() = default;

public:
	virtual		HRESULT							Initialize(void* pArg, _uInt iStateSize) override;
	virtual		void							Update(_float DeltaTime, void* pArg) override;

private:
	HRESULT										ADD_State();

public:
	static	CDialogLayer*						Create(void* pArg, _uInt iStateSize);
	virtual	void								Free() override;
};
NS_END