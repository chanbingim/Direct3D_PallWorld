#pragma once

#include "Client_Define.h"
#include "ProgressBar.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CHealthBar : public CProgressBar
{
protected:
	CHealthBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHealthBar(const CHealthBar& rhs);
	virtual ~CHealthBar() = default;

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
	_float2									m_vFontPos = {};
	_wstring								m_szFontText = {};

private :
	HRESULT									ADD_Components();

public:
	static		CHealthBar*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END