#pragma once
#include "Client_Define.h"
#include "Component.h"

NS_BEGIN(Client)
class CDropComponent final : public CComponent
{
public :
	typedef struct DropItemDesc
	{
		_uInt			iDropItemIndex;
		_uInt			iDropItemCount;
	}DROP_ITEM_DESC;

private :
	CDropComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDropComponent(const CDropComponent& Prototype);
	virtual ~CDropComponent() = default;

public :
	void							GetDropItemDesc(_bool bOverlapItem, list<DROP_ITEM_DESC>* pItemDesc);

	HRESULT							Insert_ItemIndex(_uInt iIndex, _float fPercent);
	void							Remove_ItemIndex(_uInt iIndex, _float fPercent);

private :
	list<pair<_uInt, _float>>		m_pDorpItems = {};

public :
	static CDropComponent*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;

};
NS_END