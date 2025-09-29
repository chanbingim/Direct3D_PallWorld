#pragma once
#include "Client_Define.h"
#include "UserInterface.h"

#include "ItemStruct.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CItemSlotIcon;

class CTechListViewSlot : public CUserInterface
{
protected:
	CTechListViewSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTechListViewSlot(const CTechListViewSlot& rhs);
	virtual ~CTechListViewSlot() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

		void								SetViewItemInfo(const ITEM_DESC* pItemDesc);

private:
	CItemSlotIcon*							m_pItemIcon = nullptr;
	CFontComponent*							m_pFontCom = nullptr;

	_wstring								m_szItemName = {};
	_float2									m_fFontPoint = {};
	_float4									m_vImageColor;

private:
	HRESULT									ADD_Components();

public:
	static			CTechListViewSlot*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END