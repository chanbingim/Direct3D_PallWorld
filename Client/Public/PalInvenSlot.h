#pragma once
#include "PalBoxSlot.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CHealthBar;
class CHungerBar;

class CPalInvenSlot : public  CPalBoxSlot
{
private:
	CPalInvenSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPalInvenSlot(const CPalInvenSlot& rhs);
	virtual ~CPalInvenSlot() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

protected :
	virtual		HRESULT						Bind_ShaderResources() override;
	HRESULT									Bind_ButtonShader();

private :
	CFontComponent*							m_pFontCom = nullptr;
	CShader*								m_pFrameShaderCom = nullptr;
	CTexture*								m_pFrameTex = nullptr;

	CHealthBar*								m_pHealthBar = nullptr;
	CHungerBar*								m_pHungerBar = nullptr;
	
	_float2									m_vNickNameFontPos = {};
	_wstring								m_PrintText = {};

	_float									m_fAlpha = {};
	_float									m_fStartU = {};

private:
	HRESULT									ADD_Components();

public:
	static		CPalInvenSlot*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;

};
NS_END