#pragma once
#include "NoneAnimMesh.h"
#include "ItemStruct.h"

NS_BEGIN(Engine)
class CCollision;
NS_END

NS_BEGIN(Client)
class CItemInfoUI;

class CItemObject final : public CNoneAnimMesh
{
public :
	typedef struct ItemObjectDesc : GAMEOBJECT_DESC
	{
		_uInt			iItemIndex;
		_uInt			iItemCount;
	}ITEM_OBJECT_DESC;

private:
	CItemObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItemObject(const CItemObject& rhs);
	virtual ~CItemObject() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;
	virtual		void						Damage(void* pArg, CActor* pDamagedActor);


private :
	const		ITEM_DESC*					m_ItemDesc;
	CCollision*								m_pCollision = nullptr;

	CItemInfoUI*							m_pItemUI = nullptr;
	_uInt									m_iItemCount = {};

#ifdef _DEBUG
	_bool									m_bIsRayHit = false;
#endif // _DEBUG

private :
	HRESULT									ADD_Components();
	
public :
	static		CItemObject*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END