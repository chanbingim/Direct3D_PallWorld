#pragma once

#include "BackGround.h"

NS_BEGIN(Engine)
class CSpriteAnimation;
NS_END

NS_BEGIN(Client)
class CCategory;
class CCharacterView;
class CGameOption;

class CInGameMenu final : public CBackGround
{
private:
	CInGameMenu(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CInGameMenu(const CInGameMenu& rhs);
	virtual ~CInGameMenu() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetActive(_bool flag);
	_bool									IsActive();

private :
	_bool									m_bIsActive = false;
	vector<CCategory*>						m_CategoryButton = {};

	CUserInterface*							m_pSelectWidget = nullptr;
	CGameOption*							m_pGameOptionUI = nullptr;
	CCharacterView*							m_pCharacterView = nullptr;

private:
	HRESULT									ADD_Childs();
	HRESULT									ADD_CategoryButton();
	HRESULT									ADD_Widgets();

	HRESULT									ADD_Components();

	void									SelectCategoryEvent(_uInt iIndex);

public:
	static			CInGameMenu*			Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END