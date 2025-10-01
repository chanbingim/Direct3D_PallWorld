#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CTechListSlotFont : public CBackGround
{
protected:
	CTechListSlotFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTechListSlotFont(const CBackGround& rhs);
	virtual ~CTechListSlotFont() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetText(const WCHAR* szItemName);


protected:
	CFontComponent*							m_pFontCom = nullptr;
	_wstring								m_szSelectItemName = {};
	_float2									m_vFontPos = {};

private :
	HRESULT									ADD_Components();

public:
	static			CTechListSlotFont*		Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END