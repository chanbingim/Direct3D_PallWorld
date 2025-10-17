#pragma once

#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CSelectTriangle;
class CButtonFlare;

class CCategory final : public CBackGround
{
public :
	typedef struct CateGroy_Desc : CBackGround::GAMEOBJECT_DESC
	{
		const WCHAR*			szButtonName;
		_uInt					Type;
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
	virtual		HRESULT						Apply_ConstantShaderResources();

	virtual		void						MouseHoverEnter();
	virtual		void						MouseHoverExit();

	virtual		void						MouseButtonDwon();

private :
	_float									m_fAlpha = {};
	_float									m_StartU = {};

	_uInt									m_ButtonType = {};
	_bool									m_bIsActive = false;

	_float2									m_vFontPoint = {};
	_wstring								m_FontText = {};

	CSelectTriangle*						m_pSelectTri = nullptr;
	CButtonFlare*							m_pButtonFlare = nullptr;

	CFontComponent*							m_pFontCom = nullptr;
	function<void(_uInt Index)>				m_ClickedFunc = nullptr;

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	static			CCategory*				Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;
};
NS_END