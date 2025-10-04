#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CIngredientList;
class CItemSlotIcon;

class CCreateToolTipUI : public CBackGround
{
protected:
	CCreateToolTipUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCreateToolTipUI(const CCreateToolTipUI& rhs);
	virtual ~CCreateToolTipUI() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

private:
	CItemSlotIcon*							m_pItemIcon = nullptr;
	CIngredientList*						m_pIngredientList = nullptr;

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	static			CCreateToolTipUI*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END