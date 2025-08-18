#include "Pipeline.h"
#include "GameInstance.h"

CPipeline::CPipeline(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	m_pGameInstance(CGameInstance::GetInstance()),
	m_pDevice(pDevice),
	m_pContext(pContext)
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	auto InitMat = XMMatrixIdentity();

	XMStoreFloat4x4(&m_IdentityMat, InitMat);
	for (_uInt i = 0; i < ENUM_CLASS(MAT_STATE::END); ++i)
	{
		XMStoreFloat4x4(&m_PipeLineMat[i], InitMat);
		XMStoreFloat4x4(&m_PipeLineInvMat[i], InitMat);
	}
}

HRESULT CPipeline::Initalize()
{
	if (FAILED(SettingPipeTextures()))
		return E_FAIL;

	return S_OK;
}

void CPipeline::Update(_float fDeletaTime)
{
	
}

void CPipeline::SetMatrix(MAT_STATE eState, _float4x4 Matrix)
{
	m_PipeLineMat[ENUM_CLASS(eState)] = Matrix;
	XMStoreFloat4x4(&m_PipeLineInvMat[ENUM_CLASS(eState)], XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_PipeLineMat[ENUM_CLASS(eState)])));
}

const _float4x4& CPipeline::GetMatrix(MAT_STATE eState)
{
	return m_PipeLineMat[ENUM_CLASS(eState)];
}

const _float4x4& CPipeline::GetInvMatrix(MAT_STATE eState)
{
	return m_PipeLineInvMat[ENUM_CLASS(eState)];
}

const _float4x4& CPipeline::GetIndentityMatrix()
{
	return m_IdentityMat;
}

void CPipeline::SetPostBuffer(_uInt iIndex, ID3D11ShaderResourceView* pTex)
{
	if (0 > iIndex || m_PipeTextures.size() <= iIndex)
		return;

	m_PipeTextures[iIndex] = pTex;
}

ID3D11ShaderResourceView* CPipeline::GetPostBuffer(_uInt iIndex)
{
	if (0 > iIndex || m_PipeTextures.size() <= iIndex)
		return nullptr;

	return m_PipeTextures[iIndex];
}

HRESULT CPipeline::SettingPipeTextures()
{
	_float2  ScreenSize = m_pGameInstance->GetScreenSize();
	m_PostDesc.Width = (_uInt)ScreenSize.x;
	m_PostDesc.Height = (_uInt)ScreenSize.y;
	m_PostDesc.MipLevels = 1;
	m_PostDesc.ArraySize = 1;
	m_PostDesc.Format = DXGI_FORMAT_R32_FLOAT;

	m_PostDesc.Usage = D3D11_USAGE_DEFAULT;
	m_PostDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	m_PostDesc.CPUAccessFlags = 0;
	m_PostDesc.SampleDesc.Count = 1;

	/* 텍스처 생성 */
	m_PipeTextures.reserve(8);
	for (int i = 0; i < 8; ++i)
	{
		ID3D11Texture2D* PosTex = nullptr;
		ID3D11ShaderResourceView* pSRV = nullptr;
		if (FAILED(m_pDevice->CreateTexture2D(&m_PostDesc, nullptr, (ID3D11Texture2D**)&PosTex)))
			return E_FAIL;

		m_pDevice->CreateShaderResourceView(PosTex, nullptr, &pSRV);
		m_PipeTextures.push_back(pSRV);
	}

	return S_OK;
}

CPipeline* CPipeline::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPipeline* pPipeLine = new CPipeline(pDevice, pContext);
	if (FAILED(pPipeLine->Initalize()))
	{
		Safe_Release(pPipeLine);
		MSG_BOX("CREATE FAIL : PIPELINE");
	}

	return pPipeLine;
}

void CPipeline::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	for (auto iter : m_PipeTextures)
		Safe_Release(iter);

	m_PipeTextures.clear();
}