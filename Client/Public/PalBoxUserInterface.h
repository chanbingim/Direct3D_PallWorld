#pragma once
#include "Client_Define.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CWorkPalListUI;
class CPalBoxListUI;
class CPalInvenListUI;
class CPalBox;

class CPalBoxUserInterface : public CUserInterface
{
protected:
	CPalBoxUserInterface(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPalBoxUserInterface(const CPalBoxUserInterface& rhs);
	virtual ~CPalBoxUserInterface() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetViewPalBox(CPalBox* pPalBox);

private:
	CWorkPalListUI*							m_pWorkPalList = nullptr;
	CPalBoxListUI*							m_pPalBoxList = nullptr;
	CPalInvenListUI*						m_pPalInvenList = nullptr;

private:
	HRESULT									ADD_Childs();

public:
	static			CPalBoxUserInterface*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END