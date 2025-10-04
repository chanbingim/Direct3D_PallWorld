#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CTitleUI : public CBackGround
{
public :
	typedef	struct TitleUIDesc : public GAMEOBJECT_DESC
	{
		const WCHAR*		szTitleName;

	}TITLE_UI_DESC;

protected:
	CTitleUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTitleUI(const CTitleUI& rhs);
	virtual ~CTitleUI() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

private :
	CFontComponent*							m_pFontCom = nullptr;

	_wstring								m_szTitle = {};
	_float2									m_vTitleFontPos = {};

private:
	HRESULT									ADD_Components();

public:
	static			CTitleUI*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END