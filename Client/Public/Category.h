#pragma once

#include "BackGround.h"

NS_BEGIN(Client)
class CSelectTri;

class CCategory final : public CBackGround
{
public :
	typedef struct CateGroy_Desc : CBackGround::GAMEOBJECT_DESC
	{
		_uInt		Type;
	}CATEGROY_DESC;

private:
	CCategory(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CCategory(const CCategory& rhs);
	virtual ~CCategory() = default;

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

	void									Bind_ClickEvent(function<void(_uInt Index)> Func);

protected :
	virtual		void						MouseHoverEnter();
	virtual		void						MouseHoverExit();

	virtual		void						MouseButtonDwon();

private :
	_uInt									m_ButtonType = {};
	_bool									m_bIsActive = false;

	function<void(_uInt Index)>				m_ClickedFunc = nullptr;

private:
	HRESULT									ADD_Components();
	

public:
	static			CCategory*				Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;
};
NS_END