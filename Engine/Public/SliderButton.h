#pragma once
#include "UserInterface.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;

class ENGINE_DLL CSliderButton : public CUserInterface
{
protected:
	CSliderButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSliderButton(const CSliderButton& rhs);
	virtual ~CSliderButton() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	const _bool								GetSliderButtonClick() { return m_SliderButClick; }

protected:
	virtual			void					MouseHoverEnter();
	virtual			void					MouseHovering();
	virtual			void					MouseHoverExit();

	virtual			void					MouseButtonDown();
	virtual			void					MouseButtonUp();

protected :
	CVIBuffer_Rect*							m_pVIBufferCom = nullptr;
	CTexture*								m_pTextureCom = nullptr;
	CShader*								m_pShaderCom = nullptr;

	_bool									m_SliderButClick = false;

public:
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END