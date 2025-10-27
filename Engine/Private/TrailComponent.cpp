#include "TrailComponent.h"

CTrailComponent::CTrailComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CComponent(pDevice, pContext)
{
}

CTrailComponent::CTrailComponent(const CTrailComponent& rhs) :
    CComponent(rhs),
    m_pIB(rhs.m_pIB),
    m_iNumVertices(rhs.m_iNumVertices),
    m_iNumIndices(rhs.m_iNumIndices)
{
    Safe_AddRef(m_pIB);
}

HRESULT CTrailComponent::Initialize_Prototype()
{
	m_iNumVertices = 144;
	m_iNumIndices = ((m_iNumVertices / 2) - 1) * 6;

#pragma region IDX_BUFFER
	D3D11_BUFFER_DESC IBDesc{};
	IBDesc.ByteWidth = sizeof(_uInt) * m_iNumIndices;
	IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;
	IBDesc.StructureByteStride = sizeof(_uInt);

	_uInt* pIndices = new _uInt[m_iNumIndices]{};
	_uInt iIndex = 0;
	for (_uInt iStartIndex = 0; iStartIndex < (m_iNumVertices / 2) - 1; ++iStartIndex)
	{
		//시작 인덱스
		_uInt i0 = iStartIndex * 2;
		_uInt i1 = i0 + 1;
		_uInt i2 = i0 + 2;
		_uInt i3 = i0 + 3;

		pIndices[iIndex++] = i0;
		pIndices[iIndex++] = i1;
		pIndices[iIndex++] = i2;

		pIndices[iIndex++] = i2;
		pIndices[iIndex++] = i1;
		pIndices[iIndex++] = i3;
	}

	D3D11_SUBRESOURCE_DATA InitialIBData{};
	InitialIBData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &InitialIBData, &m_pIB))) {
		return E_FAIL;
	}
	Safe_Delete_Array(pIndices);
#pragma endregion

    return S_OK;
}

HRESULT CTrailComponent::Initialize(void* pArg)
{


#pragma region VTX_BUFFER
	D3D11_BUFFER_DESC VBDesc{};
	VBDesc.ByteWidth = sizeof(VTX_TEX) * m_iNumVertices;
	VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = sizeof(VTX_TEX);

	m_pVTXPOSTEXs = new VTX_TEX[m_iNumVertices]{};
	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, nullptr, &m_pVB))) {
		return E_FAIL;
	}
#pragma endregion

	Initialize_Trail();



	return S_OK;
}

void CTrailComponent::Initialize_Trail()
{
	m_iNumPresent = 0;
	m_iEndIndex = 0;
	memset(m_pVTXPOSTEXs, 0, sizeof(VTX_TEX) * m_iNumVertices);
}

void CTrailComponent::Update_Trail(_matrix matCurrentWorld, _bool bMakeTrail)
{
	if (false == bMakeTrail) {
		if (0 != m_iNumPresent) {
			m_iNumPresent -= 2;
			memmove(m_pVTXPOSTEXs, m_pVTXPOSTEXs + 2, sizeof(VTX_TEX) * m_iNumPresent);
		}
		return;
	}

	memmove(m_vPreHighPositions, m_vPreHighPositions + 1, sizeof(_float4) * 2);
	memmove(m_vPreLowPositions, m_vPreLowPositions + 1, sizeof(_float4) * 2);

	XMStoreFloat4(&m_vPreHighPositions[2], XMVector3TransformCoord(XMLoadFloat4(&m_vHigh), matCurrentWorld));
	XMStoreFloat4(&m_vPreLowPositions[2], XMVector3TransformCoord(XMLoadFloat4(&m_vLow), matCurrentWorld));

	_uInt iNumActivatedPairs = m_iNumPresent >> 1; // [1, (m_iNumVertices - 2) >> 1]
	if (iNumActivatedPairs < 2) {

		if (m_iNumPresent + 2 >= m_iNumVertices) {
			m_iNumPresent -= 2;
			memmove(m_pVTXPOSTEXs, m_pVTXPOSTEXs + 2, sizeof(VTX_TEX) * m_iNumPresent);
		}

		XMStoreFloat3(&m_pVTXPOSTEXs[m_iNumPresent + 1].vPosition, XMLoadFloat4(&m_vPreHighPositions[2]));
		XMStoreFloat3(&m_pVTXPOSTEXs[m_iNumPresent].vPosition, XMLoadFloat4(&m_vPreLowPositions[2]));

		m_iNumPresent += 2;

		return; // 시작한지 얼마 안돼서 정점 갯수가 4 미만인 경우 얼리 리턴
	}

	{
		_vector vHighPositions[4]{};
		_vector vLowPositions[4]{};
		_float fValue = {};
		for (int i = 0; i < 3; ++i) {
			vHighPositions[i] = XMLoadFloat4(&m_vPreHighPositions[i]);
			vLowPositions[i] = XMLoadFloat4(&m_vPreLowPositions[i]);
		}

		for (int i = 0; i < 4; ++i) {
			fValue = (_float)i / 3;

			vHighPositions[3] = XMVectorCatmullRom(vHighPositions[0], vHighPositions[1], vHighPositions[2], vHighPositions[2] + (vHighPositions[2] - vHighPositions[1]), fValue);
			vLowPositions[3] = XMVectorCatmullRom(vLowPositions[0], vLowPositions[1], vLowPositions[2], vLowPositions[2] + (vLowPositions[2] - vLowPositions[1]), fValue);

			if (m_iNumPresent + 2 >= m_iNumVertices) {
				m_iNumPresent -= 2;
				memmove(m_pVTXPOSTEXs, m_pVTXPOSTEXs + 2, sizeof(VTX_TEX) * m_iNumPresent);
			}

			XMStoreFloat3(&m_pVTXPOSTEXs[m_iNumPresent + 1].vPosition, vHighPositions[3]);
			XMStoreFloat3(&m_pVTXPOSTEXs[m_iNumPresent].vPosition, vLowPositions[3]);

			m_iNumPresent += 2;
		}
	}

	{ // uv 매핑
		_uInt iIndexLow;
		_uInt iIndexHigh;
		for (_uInt iIndex = 0; iIndex < iNumActivatedPairs; ++iIndex)
		{
			_float u = (_float)iIndex / (_float)(iNumActivatedPairs - 1);
			// 전체 UV에서 현재 노트가 위치한 u구하기 ( uv 늘이기 )

			iIndexLow = iIndex << 1;
			iIndexHigh = iIndexLow + 1;

			m_pVTXPOSTEXs[iIndexHigh].vTexcoord = { u, 0.f };
			m_pVTXPOSTEXs[iIndexLow].vTexcoord = { u, 1.f };
		}
	}

	{ // Lock UnLock
		D3D11_MAPPED_SUBRESOURCE SubResource{};
		m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

		VTX_TEX* pVertices = static_cast<VTX_TEX*>(SubResource.pData);
		for (_uInt i = 0; i < m_iNumPresent; ++i) {
			pVertices[i] = m_pVTXPOSTEXs[i];
		}
		m_pContext->Unmap(m_pVB, 0);
	}
}

HRESULT CTrailComponent::Render()
{
	if (m_iNumPresent < 4)
	{ 
		return S_OK; 
	}

	ID3D11Buffer* VertexBuffers[] = { m_pVB };
	_uInt			VertexStrides[] = { sizeof(VTX_TEX) };
	_uInt			Offsets[] = { 0 };

	m_pContext->IASetVertexBuffers(0, 1, VertexBuffers, VertexStrides, Offsets);
	m_pContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);
	m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	_uInt iNumPair = m_iNumPresent >> 1;
	_uInt iActiveIndices = (iNumPair - 1) * 6;

	m_pContext->DrawIndexed(iActiveIndices, 0, 0);
	return S_OK;
}

_bool CTrailComponent::IsRenderable()
{
	return (m_iNumPresent < 4);
}

CTrailComponent* CTrailComponent::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTrailComponent* pInstance = new CTrailComponent(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTrail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTrailComponent::Clone(void* pArg)
{
	CTrailComponent* pInstance = new CTrailComponent(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTrail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrailComponent::Free()
{
	__super::Free();

	if (true == m_isCloned) {
		Safe_Delete_Array(m_pVTXPOSTEXs);
	}

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}