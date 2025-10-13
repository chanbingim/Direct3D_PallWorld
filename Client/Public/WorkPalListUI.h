#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CPalBoxSlot;
class CTitleUI;

class CWorkPalListUI : public CBackGround
{
protected:
	CWorkPalListUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWorkPalListUI(const CWorkPalListUI& rhs);
	virtual ~CWorkPalListUI() = default;

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
	CTitleUI*								m_pTitleUI = {};

	_float2									m_SlotCount = {};
	vector<CPalBoxSlot *>					m_Slots = {};

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	static			CWorkPalListUI*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END