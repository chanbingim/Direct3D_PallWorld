#include "Shader.h"

#include "GameInstance.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CComponent(pDevice, pContext)
{

}

CShader::CShader(const CShader& rhs) :
	CComponent(rhs),
	m_pInputLayOut(rhs.m_pInputLayOut),
	m_pEffect(rhs.m_pEffect),
	m_pTech(rhs.m_pTech)
{
	Safe_AddRef(m_pInputLayOut);
	Safe_AddRef(m_pEffect);
	Safe_AddRef(m_pTech);
}

HRESULT CShader::Initialize_Prototype(D3D11_INPUT_ELEMENT_DESC* pElementDesc, _uInt iElementCnt, const WCHAR* szShaderFilePath)
{
	if(FAILED(LoadShader(szShaderFilePath)))
		return E_FAIL;

	D3DX11_PASS_DESC	PassDesc = {};
	ZeroMemory(&PassDesc, sizeof(D3DX11_PASS_DESC));

	m_pTech = m_pEffect->GetTechniqueByName("Tech");

	m_pTech->GetPassByIndex(0)->GetDesc(&PassDesc);
	m_pDevice->CreateInputLayout(pElementDesc, iElementCnt, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pInputLayOut);

	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
	return S_OK;
}

ID3DX11EffectVariable* CShader::GetVariable(const _string& ValueName)
{
	return m_pEffect->GetVariableByName(ValueName.c_str());
}

// 셰이더를 업데이트 함과 동시에 상수 버퍼 및 데이터 갱신
void CShader::Update_Shader(_uInt ipassIndex)
{
	/*D3DX11_TECHNIQUE_DESC TechDesc;

	m_pTech->GetDesc(&TechDesc);
	for (UINT i = 0; i < TechDesc.Passes; ++i)
	{
		m_pTech->GetPassByIndex(i)->Apply(0, m_pContext);
	}*/

	m_pContext->IASetInputLayout(m_pInputLayOut);
	m_pTech->GetPassByIndex(ipassIndex)->Apply(0, m_pContext);
}

HRESULT CShader::LoadShader(const WCHAR* szShaderFilePath)
{
	ID3DBlob* pError = nullptr;

	if (FAILED(D3DX11CompileEffectFromFile(szShaderFilePath, // HLSL 파일 명
		nullptr,											 // 매크로 정의 배열 포인터
		D3D_COMPILE_STANDARD_FILE_INCLUDE,					 // 셰이더에 #inlcude 가 있을경우 설정
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,     // HLSL이 컴파일 하는 방법 지정
		0,              // 효과 파일이 아니라면 Flag 설정은 무시 사용하지않으면 0
		m_pDevice,      // 셰이더를 생성하기때문에 Device가 필요함
		&m_pEffect,		// 생성된 셰이더 객체 정보를 받을 포인터
		&pError)))		// 컴파일 중에 발생한 오류를 기록하는 포인터 
	{
		if (pError) {
			OutputDebugStringA((char*)pError->GetBufferPointer());
			pError->Release();
		}

		return E_FAIL;
	}
	return S_OK;
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, D3D11_INPUT_ELEMENT_DESC* pElementDesc, _uInt iElementCnt, const WCHAR* szShaderFilePath)
{
	CShader* pShader = new CShader(pDevice, pContext);
	if (FAILED(pShader->Initialize_Prototype(pElementDesc, iElementCnt, szShaderFilePath)))
	{
		Safe_Release(pShader);
		MSG_BOX("CREATE FAIL : SHADER");
	}

	return pShader;
}

CComponent* CShader::Clone(void* pArg)
{
	CShader* pShader = new CShader(*this);
	if (FAILED(pShader->Initialize(pArg)))
	{
		Safe_Release(pShader);
		MSG_BOX("CLONE FAIL : SHADER");
	}

	return pShader;
}

void CShader::Free()
{
	__super::Free();

	Safe_Release(m_pEffect);
	//Safe_Release(m_pTech);
	Safe_Release(m_pInputLayOut);
}
