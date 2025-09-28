#pragma once
#include "Client_Define.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CTechnologyPointView;
class CTechnologyTitleBar;
class CTechnologyView;

class CCreateMenu final : public CUserInterface
{
private:
	CCreateMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCreateMenu(const CCreateMenu& rhs);
	virtual ~CCreateMenu() = default;

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
	//제작 UI
	CTechnologyTitleBar*					m_pTechNologyTitleBar = nullptr;
	CTechnologyView*						m_pTechnologyView = nullptr;
	CTechnologyPointView*					m_pTechNologyPointView = nullptr;

private:
	HRESULT									ADD_Childs();

public:
	static		CCreateMenu*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;

};
NS_END