#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CTechSlotTitleBar final : public CBackGround
{
private:
	CTechSlotTitleBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTechSlotTitleBar(const CTechSlotTitleBar& rhs);
	virtual ~CTechSlotTitleBar() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetTehcItemTypeName(const WCHAR* pTechTypeName);


private:
	CFontComponent*							m_pFontCom = nullptr;
	_float2									m_fFontPoint = { };

	_wstring								m_szTechTypeName = {};

private:
	HRESULT									ADD_Components();

public:
	static		CTechSlotTitleBar*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;
};
NS_END