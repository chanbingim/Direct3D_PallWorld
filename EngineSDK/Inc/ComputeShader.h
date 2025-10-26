#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CComputeShader : public CComponent
{
private:
	CComputeShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComputeShader(const CComputeShader& rhs);
	virtual ~CComputeShader() = default;

public:
	HRESULT								Initialize_Prototype(const WCHAR* szShaderFilePath, const char* szStartFunName);
	virtual HRESULT						Initialize(void* pArg);

public:
	HRESULT								Bind_UAVResource(_uInt iIndex, ID3D11UnorderedAccessView*	pUAV);
	HRESULT								Bind_ShaderResource(_uInt iIndex, ID3D11ShaderResourceView* pSRV);
	HRESULT								Bind_Buffer(_uInt iIndex, ID3D11Buffer* Buffer);

public:
	void								Update_Shader(_int3 iGroupCount);

private:
	ID3D11ComputeShader*				m_pShaderCom = nullptr;

private:
	HRESULT								LoadShader(const WCHAR* szShaderFilePath, const char* szStartFunName);

public:
	static		CComputeShader*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const WCHAR* szShaderFilePath, const char* szStartFunName);
	virtual		CComponent*				Clone(void* pArg) override;
	virtual		void					Free() override;

};
NS_END