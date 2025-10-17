#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CLevelIcon : public CBackGround
{
protected:
	CLevelIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLevelIcon(const CLevelIcon& rhs);
	virtual ~CLevelIcon() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetLevel(const WCHAR*	szLevel);

private:
	CFontComponent*							m_pFontCom = nullptr;
	_float2									m_fFontPoint = {};
	_wstring								m_szLevel = {};

private:
	HRESULT									ADD_Components();

public:
	static		CLevelIcon*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END