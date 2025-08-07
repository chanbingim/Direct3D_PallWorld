#pragma once

#include "BackGround.h"

NS_BEGIN(Client)
class CCategory final : public CBackGround
{
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

protected :
	virtual		void						MouseHoverEnter();
	virtual		void						MouseHoverExit();

	virtual		void						MouseButtonDwon();
	
	void									SetActive(_bool flag);
	_bool									IsActive();

private :
	_uInt									m_ButtonType = {};
	_bool									m_bIsActive = false;

private:
	HRESULT									ADD_Components();

public:
	static			CCategory*				Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;
};
NS_END