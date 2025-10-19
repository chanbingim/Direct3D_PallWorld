#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CTitleUI;
class CButton;

class CFastTravelUI : public CBackGround
{
protected:
	CFastTravelUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFastTravelUI(const CFastTravelUI& rhs);
	virtual ~CFastTravelUI() = default;

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
	vector<CButton*>						m_pButtons = {};

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

	void									ButtonClickedEvent(const _wstring& MapName);

public:
	static			CFastTravelUI*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;
};
NS_END