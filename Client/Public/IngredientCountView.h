#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CIngredientCountView : public CBackGround
{
protected:
	CIngredientCountView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIngredientCountView(const CIngredientCountView& rhs);
	virtual ~CIngredientCountView() = default;

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

	_wstring								m_szText = {};
	_float2									m_vTitleFontPos = {};

private:
	HRESULT									ADD_Components();

public:
	static			CIngredientCountView*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END