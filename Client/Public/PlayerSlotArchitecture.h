#pragma once
#include "Client_Define.h"
#include "PartObject.h"
#include "ItemStruct.h"

NS_BEGIN(Engine)
class CNavigation;
NS_END

NS_BEGIN(Client)
class CPlayerSlotArchitecture : public CPartObject
{
public :
	typedef struct PlayerSlotArchitectureDesc : public PARTOBJECT_DESC
	{
		_uInt					iParentCellIndex;

	}PLAYTER_SLOT_ARCHITECTURE_DESC;

protected:
	CPlayerSlotArchitecture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerSlotArchitecture(const CPlayerSlotArchitecture& rhs);
	virtual ~CPlayerSlotArchitecture() = default;

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

	void									SetItemDesc(const ITEM_DESC* pDesc);
	
	_bool									IsPreView();
	void									CreateSlotObject();

protected:
	virtual     HRESULT						Bind_ShaderResources();
	virtual     HRESULT						Apply_ConstantShaderResources(_uInt iMeshIndex);

private :
	const		ITEM_DESC*					m_pItemDesc = nullptr;
	CNavigation*							m_pNavigation = nullptr;
	CModel*									m_pItemModel = nullptr;

	_float4									m_vPreViewColor = {};
	_float3									m_vMoveAmount = {};

	_bool									m_bIsCreateAble = false;
	

private :
	HRESULT									ADD_Components(PLAYTER_SLOT_ARCHITECTURE_DESC* pDesc);

public:
	static		CPlayerSlotArchitecture*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END