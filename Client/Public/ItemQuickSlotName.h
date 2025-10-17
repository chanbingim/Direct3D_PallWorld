#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
class CShader;
class CVIBuffer_Rect;
class CTexture;
NS_END

NS_BEGIN(Client)
class CItemQuickSlotName : public CBackGround
{
protected:
	CItemQuickSlotName(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItemQuickSlotName(const CItemQuickSlotName& rhs);
	virtual ~CItemQuickSlotName() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;
	void									SetItemName(const WCHAR* szName);

private:
	CFontComponent*							m_pFontCom = nullptr;

	_wstring								m_szItemName = {};
	_float2									m_fFontPoint = {};

	_float4									m_vImageColor = {};
private:
	HRESULT									ADD_Components();

public:
	static			CItemQuickSlotName*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END