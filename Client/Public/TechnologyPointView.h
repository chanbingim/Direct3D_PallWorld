#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CPlayerManager;

class CTechnologyPointView final : public CBackGround
{
private:
	CTechnologyPointView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTechnologyPointView(const CTechnologyPointView& rhs);
	virtual ~CTechnologyPointView() = default;

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
	CFontComponent*							m_pPointFontCom = nullptr;
	CPlayerManager*							m_pPlayerManager = nullptr;

	_float2									m_fFontPoint = { };
	_float2									m_fSkillFontPoint = { };

	_wstring								m_szTechPoint = {};

private:
	HRESULT									ADD_Components();

public:
	static		CTechnologyPointView*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;
};
NS_END