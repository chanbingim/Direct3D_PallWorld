#include "ComputeShader.h"

CComputeShader::CComputeShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CComponent(pDevice, pContext)
{
}

CComputeShader::CComputeShader(const CComputeShader& rhs) :
    CComponent(rhs),
    m_pShaderCom(rhs.m_pShaderCom)
{
    Safe_AddRef(m_pShaderCom);
}

HRESULT CComputeShader::Initialize_Prototype(const WCHAR* szShaderFilePath, const char* szStartFunName)
{
    if (FAILED(LoadShader(szShaderFilePath, szStartFunName)))
        return E_FAIL;
    return S_OK;
}

HRESULT CComputeShader::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CComputeShader::Bind_UAVResource(_uInt iIndex, ID3D11UnorderedAccessView* pUAV)
{
    m_pContext->CSSetUnorderedAccessViews(iIndex, 1, &pUAV, nullptr);
    return S_OK;
}

HRESULT CComputeShader::Bind_ShaderResource(_uInt iIndex, ID3D11ShaderResourceView* pSRV)
{
    m_pContext->CSSetShaderResources(iIndex, 1, &pSRV);
    return S_OK;
}

HRESULT CComputeShader::Bind_Buffer(_uInt iIndex, ID3D11Buffer* Buffer)
{
    m_pContext->CSSetConstantBuffers(iIndex, 1, &Buffer);
    return S_OK;
}

void CComputeShader::Update_Shader(_int3 iGroupCount)
{
    m_pContext->CSSetShader(m_pShaderCom, nullptr, 0);
    m_pContext->Dispatch(iGroupCount.x, iGroupCount.y, iGroupCount.z);
}

HRESULT CComputeShader::LoadShader(const WCHAR* szShaderFilePath, const char* szStartFunName)
{
    ID3DBlob* pBlobVS = nullptr;
    ID3DBlob* pBlobCode = nullptr;

    // Debug 모드와 release 모드의 플레그를 다르게 줘야함
    UINT HLSLFlags = {};
#ifdef _DEBUG
    HLSLFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    HLSLFlags = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif // _DEBUG

    if (FAILED(D3DCompileFromFile(
        szShaderFilePath, // HLSL 파일 명
        nullptr,											 // 매크로 정의 배열 포인터
        D3D_COMPILE_STANDARD_FILE_INCLUDE,					 // 셰이더에 #inlcude 가 있을경우 설정
        szStartFunName,
        "cs_5_0",
        HLSLFlags,
        0,
        &pBlobCode,
        &pBlobVS)))		// 컴파일 중에 발생한 오류를 기록하는 포인터 
    {
        if (pBlobVS) {
            OutputDebugStringA((char*)pBlobVS->GetBufferPointer());
            pBlobVS->Release();
        }
        return E_FAIL;
    }

    if (FAILED(m_pDevice->CreateComputeShader(
        pBlobCode->GetBufferPointer(),         // 컴파일된 쉐이더 코드
        pBlobCode->GetBufferSize(),            // 코드 길이
        nullptr,                                // 클래스 인스턴스 (없음)
        &m_pShaderCom)))                          // 컴퓨트 쉐이더 객체
        return E_FAIL;

    return S_OK;
}

CComputeShader* CComputeShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const WCHAR* szShaderFilePath, const char* szStartFunName)
{
    CComputeShader* pComputeShader = new CComputeShader(pDevice, pContext);
    if (FAILED(pComputeShader->Initialize_Prototype(szShaderFilePath, szStartFunName)))
    {
        Safe_Release(pComputeShader);
        MSG_BOX("CREATE FAIL COMPUTE SHADER");
    }
    return pComputeShader;
}

CComponent* CComputeShader::Clone(void* pArg)
{
    CComputeShader* pComputeShader = new CComputeShader(*this);
    if (FAILED(pComputeShader->Initialize(pArg)))
    {
        Safe_Release(pComputeShader);
        MSG_BOX("CLONE FAIL COMPUTE SHADER");
    }
    return pComputeShader;
}

void CComputeShader::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
}
