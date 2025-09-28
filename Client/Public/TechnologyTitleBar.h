#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CTechnologyTitleBar final : public CBackGround
{
private:
	CTechnologyTitleBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTechnologyTitleBar(const CTechnologyTitleBar& rhs);
	virtual ~CTechnologyTitleBar() = default;

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
	CFontComponent*							m_pFontCom = nullptr;
	_float2									m_fFontPoint = { };

private:
	HRESULT									ADD_Components();

public:
	static		CTechnologyTitleBar*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;

};
NS_END