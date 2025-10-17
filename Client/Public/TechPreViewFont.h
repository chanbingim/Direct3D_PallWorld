#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CTechPreViewFont : public CBackGround
{
private:
	CTechPreViewFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTechPreViewFont(const CTechPreViewFont& rhs);
	virtual ~CTechPreViewFont() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;
	void									SetText(const WCHAR* szText);

private:
	CFontComponent*							m_pFontCom = nullptr;
	_wstring								m_szItemName = {};
	_float2									m_vFontPoint = {};

private:
	HRESULT									ADD_Components();

public:
	static		CTechPreViewFont*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;


};
NS_END