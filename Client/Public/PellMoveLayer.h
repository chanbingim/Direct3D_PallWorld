#pragma once
#include "Client_Define.h"
#include "StateLayer.h"

NS_BEGIN(Engine)
class CContainerObject;
NS_END

NS_BEGIN(Client)
class CPellMoveLayer : public CStateLayer
{
public :
	typedef struct PellLayerDesc 
	{
		CContainerObject* pOwner;
	}PELL_LAYER_DESC;

private:
	CPellMoveLayer();
	CPellMoveLayer(const CPellMoveLayer& rhs);
	virtual ~CPellMoveLayer() = default;

public:
	virtual		HRESULT							Initialize(void* pArg, _uInt iStateSize) override;
	virtual		void							Update(_float DeltaTime) override;

private :
	CContainerObject*						m_pOwner = nullptr;

private:
	HRESULT										ADD_MoveState();

public:
	static	CPellMoveLayer*						Create(void* pArg, _uInt iStateSize);
	virtual	void								Free() override;

};
NS_END