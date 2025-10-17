#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CPalSpher;
class CPallSpherPercentBar;

class CPalSpherUI : public CBackGround
{
protected:
	CPalSpherUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPalSpherUI(const CPalSpherUI& rhs);
	virtual ~CPalSpherUI() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetPalSpherUI(_float fPercent);
	const		_float4x4&					GetCombinedMatrix() const { return m_CombinedMatrix; }

private:
	_float4x4								m_CombinedMatrix = {};
	_float									m_fPercent = {};

	//폰트는 나중에 구해서 적용
	CPallSpherPercentBar*					m_pPercentBar = nullptr;

protected:
	virtual     HRESULT						Apply_ConstantShaderResources() override;

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	static			CPalSpherUI*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;
};
NS_END