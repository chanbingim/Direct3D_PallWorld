#pragma once
#include "Client_Define.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CTechSelectView;
class CTechCategoryUI;

class CCreateMenu final : public CUserInterface
{
private:
	CCreateMenu(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
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
	vector<CTechCategoryUI*>				m_CategoryButton = {};
	CTechSelectView*						m_pCharacterView = nullptr;

private:
	HRESULT									ADD_Childs();
	HRESULT									ADD_CategoryButton();

	void									SelectCategoryEvent(_uInt iIndex);

public:
	static			CCreateMenu*			Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END