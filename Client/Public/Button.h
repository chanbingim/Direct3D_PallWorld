#pragma once

#include "Client_Define.h"
#include "BackGround.h"

NS_BEGIN(Client)
class CButtonFrame;

class CButton : public CBackGround
{
protected:
	CButton(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CButton(const CButton& rhs);
	virtual ~CButton() = default;

public :
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetSelect(_bool flag);
	_bool									IsSelect();

protected :
	CButtonFrame*							m_pButtonFrame = nullptr;
	function<void()>						m_ClickedFunc = nullptr;

	_bool									m_bIsSelect = false;
	_wstring								m_ButtonText = {};

protected :
	virtual			void					MouseHoverEnter() override;
	virtual			void					MouseHovering() override;
	virtual			void					MouseHoverExit() override;

	virtual			void					MouseButtonDwon() override;

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;
};
NS_END