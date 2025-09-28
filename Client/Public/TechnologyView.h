#pragma once
#include "BackGround.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CTechSlotLine;

class CTechnologyView : public CUserInterface
{
protected:
	CTechnologyView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTechnologyView(const CTechnologyView& rhs);
	virtual ~CTechnologyView() = default;

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
	vector<CTechSlotLine*>					m_pTechSlotLine = {};

private:
	HRESULT									ADD_Childs();

public:
	static		CTechnologyView*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;

};
NS_END