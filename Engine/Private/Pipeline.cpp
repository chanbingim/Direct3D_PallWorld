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

void CPipeline::SetPlayerWorldMatrix(_float4x4 Matrix)
{
	m_PlayerWorldMat = Matrix;
}

ID3D11ShaderResourceView* CPipeline::GetPostBuffer(_uInt iIndex)
{
	if (0 > iIndex || m_PipeTextures.size() <= iIndex)
		return nullptr;
	return m_PipeTextures[iIndex];
}

_vector CPipeline::GetCameraState(WORLDSTATE eType)
{
	_matrix CameraWolrdMat = XMLoadFloat4x4(&m_PipeLineInvMat[ENUM_CLASS(MAT_STATE::VIEW)]);
	return CameraWolrdMat.r[ENUM_CLASS(eType)];
}

_vector CPipeline::GetPlayerState(WORLDSTATE eType)
{
	_matrix PlayerWolrdMat = XMLoadFloat4x4(&m_PlayerWorldMat);
	return PlayerWolrdMat.r[ENUM_CLASS(eType)];
}

HRESULT CPipeline::SettingPipeTextures()
{
	//백버퍼의 정보를 가져와서 전역으로 사용할 용도로 보관할 텍스처들을 세팅한다.
	ID3D11Texture2D* pBackBuffer = nullptr;
	m_pGameInstance->GetBackBuffer(&pBackBuffer);
	pBackBuffer->GetDesc(&m_PostDesc);

	//셰이더에 넘겨 값을 설정하는 용도 및
	//backbuffer가 랜더타겟용도로 사용되었기때문에 둘이 같은 값으로 세팅해준다.
	//나중에 이데이터를 랜더타겟으로 사용할지도 모름
	m_PostDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

	/* 텍스처 생성 */
	m_PipeTextures.reserve(8);
	for (int i = 0; i < 8; ++i)
	{
		ID3D11Texture2D* PosTex = nullptr;
		ID3D11ShaderResourceView* pSRV = nullptr;
		if (FAILED(m_pDevice->CreateTexture2D(&m_PostDesc, nullptr, &PosTex)))
			return E_FAIL;

		m_pDevice->CreateShaderResourceView(PosTex, nullptr, &pSRV);
		m_PipeTextures.push_back(pSRV);

		//텍스처2D를 이용해서 리소스뷰를 만들었기때문에 메모리 해제
		//안하면 누수남
		Safe_Release(PosTex);
	}
	// 백버퍼에 대한 정보를 전부다 사용했기때문에 Ref해제
	Safe_Release(pBackBuffer);

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