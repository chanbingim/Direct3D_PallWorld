#pragma once

#include "AnimMeshActor.h"

NS_BEGIN(Client)
class CItemBase : public CAnimMeshActor
{
public :
	typedef struct InitItemDesc
	{
		_uInt									iItemNum;
		ITEM_TYPE								eItemType;

	}INIT_ITEM_DESC;


protected:
	CItemBase(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CItemBase(const CItemBase& rhs);
	virtual ~CItemBase() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype(void* InitDesc);
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;

protected :
	_uInt									m_ItemNum = {};
	ITEM_TYPE								m_ItemType = { ITEM_TYPE::END };

public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END