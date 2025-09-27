#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)

class CItemObject;

class CItemInfoUI : public CBackGround
{
public:
	typedef struct ItemInfoDesc : public GAMEOBJECT_DESC
	{
		const WCHAR* pItemName;
	}ITEM_INFO_DESC;

protected:
	CItemInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItemInfoUI(const CItemInfoUI& rhs);
	virtual ~CItemInfoUI() = default;

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
	CItemObject* m_pOwner = nullptr;
	CFontComponent* m_pFontCom = nullptr;

	_float2									m_vFontPosition = {};
	_float3									m_vAwayPoisition = {};
	_wstring								m_szItemName = {};
	ID3DX11EffectVariable*					m_pEVCamPos = nullptr;
 
protected:
	virtual		HRESULT						Bind_ShaderResources() override;
	virtual     HRESULT						Apply_ConstantShaderResources() override;

private:
	HRESULT									ADD_Components();

public:
	static			CItemInfoUI*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;




};
NS_END