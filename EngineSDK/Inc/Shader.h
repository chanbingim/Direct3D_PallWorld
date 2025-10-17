#pragma once

#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CShader : public CComponent
{
private :
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public :
	HRESULT						Initialize_Prototype(const D3D11_INPUT_ELEMENT_DESC* pElementDesc, const _uInt iElementCnt, const WCHAR* szShaderFilePath);
	virtual HRESULT				Initialize(void* pArg);

	ID3DX11EffectVariable*		GetVariable(const _string& ValueName);
	void						Update_Shader(_uInt ipassIndex);

private :
	vector<ID3D11InputLayout*>		m_pInputLayOutVec = {};
	LPD3D11EFFECT					m_pEffect = nullptr;
	LPD3D11EFFECTTECHNIQUE			m_pTech = nullptr;

	_uInt							m_PaseesCnt = {};

private :
	HRESULT						LoadShader(const WCHAR* szShaderFilePath);

public :
	static		CShader*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const D3D11_INPUT_ELEMENT_DESC* pElementDesc, const _uInt iElementCnt, const WCHAR* szShaderFilePath);
	virtual		CComponent*		Clone(void* pArg) override;
	virtual		void			Free() override;

};
NS_END