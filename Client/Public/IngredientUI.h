#pragma once
#include "BackGround.h"
#include "ItemStruct.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CItemSlotIcon;
class CIngredientCountView;

class CIngredientUI : public CBackGround
{
protected:
	CIngredientUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIngredientUI(const CIngredientUI& rhs);
	virtual ~CIngredientUI() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

private :
	CItemSlotIcon*							m_pItemIcon = nullptr;
	CFontComponent*							m_pFontCom = nullptr;
	CIngredientCountView*					m_pIngredientCountView = nullptr;

	_uInt									m_iItemID = {};
	const ITEM_DESC*						m_ItemData = nullptr;

	_float2									m_vFontPosition = {};
private:
	HRESULT									ADD_Components();

public:
	static			CIngredientUI*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END