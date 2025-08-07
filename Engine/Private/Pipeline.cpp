#include "Pipeline.h"

CPipeline::CPipeline()
{
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

CPipeline* CPipeline::Create()
{
	CPipeline* pPipeLine = new CPipeline();
	if (nullptr == pPipeLine)
		MSG_BOX("CREATE FAIL : PIPELINE");

	return pPipeLine;
}

void CPipeline::Free()
{
	__super::Free();
}