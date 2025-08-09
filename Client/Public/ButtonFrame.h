#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CSpriteAnimation;
NS_END

NS_BEGIN(Client)

class CDefaultEntity;

class CButtonFrame : public CBackGround
{
protected:
	CButtonFrame(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CButtonFrame(const CButtonFrame& rhs);
	virtual ~CButtonFrame() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	
	// 랜더
	virtual		HRESULT						Render() override;

	void									ResetButtonFrame();

protected :
	virtual		HRESULT						Apply_ConstantShaderResources(_uInt iIndex);

private :
	CSpriteAnimation*						m_pAnimator = nullptr;
	_float4x4								m_pFrameCenter[4] = {};

private :
	HRESULT									ADD_Components();
	HRESULT									ADD_Animations();
	void									SetFrameCenterPos(_uInt iIndex, _float4 vPos);
	void									SetInitFrameMatrix();

public:
	static		CButtonFrame*				Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;
};
NS_END