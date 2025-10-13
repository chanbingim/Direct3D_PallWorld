#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CPalBoxSlot;

class CPalBoxListUI : public CBackGround
{
protected:
	CPalBoxListUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPalBoxListUI(const CPalBoxListUI& rhs);
	virtual ~CPalBoxListUI() = default;

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
	vector<CPalBoxSlot>						m_Slots = {};

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	static			CPalBoxListUI*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;
};
NS_END