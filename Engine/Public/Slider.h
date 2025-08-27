#pragma once
#include "UserInterface.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
class CSliderButton;

class ENGINE_DLL CSlider : public CUserInterface
{
public :
	typedef struct SliderDesc : GAMEOBJECT_DESC
	{
		AXIS					eType;
	}SLIDER_DESC;

protected:
	CSlider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSlider(const CSlider& rhs);
	virtual ~CSlider() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	_float									GetCurPercent() { return m_fSliderCurPercent; }
	_float3									GetMouseMoveVecDir() { return m_MoveDir; }
	_float									GetMouseMoveDir();

protected :
	virtual			void					MouseButtonDwon() override;
	virtual			void					MouseButtonPressed() override;
	virtual			void					MouseButtonUp() override;

	void									UpdateSliderBar();

protected :
	CVIBuffer_Rect*							m_pVIBufferCom = nullptr;
	CTexture*								m_pTextureCom = nullptr;
	CShader*								m_pShaderCom = nullptr;
	CSliderButton*							m_pSliderBut = nullptr;

	_float									m_fSliderCurPercent = {};

	//x 성분 : 최소  y성분 : 최대
	_float2									m_fSliderMaxMin = {};
	AXIS									m_eSliderType = {AXIS::HORIZONTAL};

	//어느방향으로 이동했는지를 알려주는 변수들
	POINT									m_ClickPos = {};
	_float3									m_MoveDir = {};

public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END