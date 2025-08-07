#pragma once

#include "BackGround.h"

NS_BEGIN(Client)
class CCompassPoint;

class CCompass final : public CBackGround
{
protected:
	CCompass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCompass(const CCompass& rhs);
	virtual ~CCompass() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetCompass(_float fRadius);

protected :
	HRESULT									Bind_ShaderResources() override;
	HRESULT									Apply_ConstantShaderResources() override;

private :
	CCompassPoint*							m_CompassPoint = nullptr;
	ID3DX11EffectVariable*					m_pUvPercent = nullptr;

	_float									m_fRadius = {};

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	static		CCompass*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;

};
NS_END