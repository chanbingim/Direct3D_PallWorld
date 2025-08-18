#include "VIBuffer_Terrian.h"

CVIBuffer_Terrian::CVIBuffer_Terrian(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Terrian::CVIBuffer_Terrian(const CVIBuffer_Terrian& rhs) :
    CVIBuffer(rhs),
	m_iNumVertex(rhs.m_iNumVertex)
{
}

HRESULT CVIBuffer_Terrian::Initialize_Prototype(_uInt TileCnt)
{
	m_iNumVertex.x = (_float)TileCnt;
	m_iNumVertex.y = (_float)TileCnt;
	m_iNumVertices = (_uInt)(m_iNumVertex.x * m_iNumVertex.y);

	m_iNumVertexBuffers = 1;
	m_iVertexStride = sizeof(VTX_NORTEX);

	m_iNumIndices = _uInt(m_iNumVertex.x - 1) * _uInt(m_iNumVertex.y - 1) * 2 * 3;
	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = _uInt(m_iVertexStride * m_iNumVertices);
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	VTX_NORTEX* pVertices = new VTX_NORTEX[(_uInt)m_iNumVertices];
	ZeroMemory(pVertices, size_t(sizeof(VTX_NORTEX) * m_iNumVertices));

	m_pVertices = new _float3[(_uInt)m_iNumVertices];
	ZeroMemory(m_pVertices, size_t(sizeof(_float3) * m_iNumVertices));

	for (_float i = 0; i < m_iNumVertex.y; i++)
	{
		for (_float j = 0; j < m_iNumVertex.x; j++)
		{
			_uInt		iIndex = _uInt(i * m_iNumVertex.x + j);

			m_pVertices[iIndex] = pVertices[iIndex].vPosition = _float3(j, 0.f, i);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVertex.x - 1.f), i / (m_iNumVertex.y - 1.f));
		}
	}

	D3D11_SUBRESOURCE_DATA	InitialVBData{};
	InitialVBData.pSysMem = pVertices;
#pragma endregion

#pragma region INDEX_BUFFER
	D3D11_BUFFER_DESC		IBDesc{};
	IBDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.StructureByteStride = m_iIndexStride;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;

	_uInt* pIndices = new _uInt[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uInt) * m_iNumIndices);
	_uInt		iNumIndices = {};

	for (_float i = 0; i < m_iNumVertex.y - 1; i++)
	{
		for (_float j = 0; j < m_iNumVertex.x - 1; j++)
		{
			_uInt		iIndex = _uInt(i * m_iNumVertex.x + j);

			_uInt		iIndices[4] = {
				iIndex + (_uInt)m_iNumVertex.x,
				iIndex + (_uInt)m_iNumVertex.x + 1,
				iIndex + 1,
				iIndex
			};

			_vector		vSourDir, vDestDir, vNormal;

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			vSourDir = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[1]].vNormal, XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

			vSourDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[3]].vNormal, XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal);
		}
	}

	for (size_t i = 0; i < m_iNumVertices; i++)
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));

	D3D11_SUBRESOURCE_DATA	InitialIBData{};
	InitialIBData.pSysMem = pIndices;
#pragma endregion

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitialVBData, &m_pVertexBuffer)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &InitialIBData, &m_pIndexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Terrian::Initialize_Prototype(const WCHAR* pHegithFilePath)
{
	_ulong			dwByte = {};
	HANDLE			hFile = CreateFile(pHegithFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	BITMAPFILEHEADER			fh{};
	BITMAPINFOHEADER			ih{};

	if (!ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr))
		return E_FAIL;

	if (!ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr))
		return E_FAIL;

	m_iNumVertex.x = (_float)ih.biWidth;
	m_iNumVertex.y = (_float)ih.biHeight;
	m_iNumVertices = m_iNumVertex.x * m_iNumVertex.y;

	_uInt* pPixels = new _uInt[(_uInt)m_iNumVertices];
	ZeroMemory(pPixels, size_t(sizeof(_uInt) * m_iNumVertices));

	if (!ReadFile(hFile, pPixels, size_t(sizeof(_uInt) * m_iNumVertices), &dwByte, nullptr))
		return E_FAIL;

	m_iNumVertexBuffers = 1;

	m_iVertexStride = sizeof(VTX_NORTEX);

	m_iNumIndices = _uInt(m_iNumVertex.x - 1) * _uInt(m_iNumVertex.y - 1) * 2 * 3;
	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = _uInt(m_iVertexStride * m_iNumVertices);
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	VTX_NORTEX* pVertices = new VTX_NORTEX[(_uInt)m_iNumVertices];
	ZeroMemory(pVertices, size_t(sizeof(VTX_NORTEX) * m_iNumVertices));

	m_pVertices = new _float3[(_uInt)m_iNumVertices];
	ZeroMemory(m_pVertices, size_t(sizeof(_float3) * m_iNumVertices));

	for (_float i = 0; i < m_iNumVertex.y; i++)
	{
		for (_float j = 0; j < m_iNumVertex.x; j++)
		{
			_uInt		iIndex = _uInt(i * m_iNumVertex.x + j);

			m_pVertices[iIndex] = pVertices[iIndex].vPosition = _float3(j, (pPixels[iIndex] & 0x000000ff) / 10.f, i);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVertex.x - 1.f), i / (m_iNumVertex.y - 1.f));
		}
	}

	D3D11_SUBRESOURCE_DATA	InitialVBData{};
	InitialVBData.pSysMem = pVertices;
#pragma endregion

#pragma region INDEX_BUFFER
	D3D11_BUFFER_DESC		IBDesc{};
	IBDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.StructureByteStride = m_iIndexStride;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;

	_uInt* pIndices = new _uInt[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uInt) * m_iNumIndices);
	_uInt		iNumIndices = {};

	for (_float i = 0; i < m_iNumVertex.y - 1; i++)
	{
		for (_float j = 0; j < m_iNumVertex.x - 1; j++)
		{
			_uInt		iIndex = _uInt(i * m_iNumVertex.x + j);

			_uInt		iIndices[4] = {
				iIndex + (_uInt)m_iNumVertex.x,
				iIndex + (_uInt)m_iNumVertex.x + 1,
				iIndex + 1,
				iIndex
			};

			_vector		vSourDir, vDestDir, vNormal;

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			vSourDir = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[1]].vNormal, XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

			vSourDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[3]].vNormal, XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal);
		}
	}

	for (size_t i = 0; i < m_iNumVertices; i++)
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));

	D3D11_SUBRESOURCE_DATA	InitialIBData{};
	InitialIBData.pSysMem = pIndices;
#pragma endregion

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitialVBData, &m_pVertexBuffer)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &InitialIBData, &m_pIndexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);

	Safe_Delete_Array(pPixels);
	CloseHandle(hFile);

    return S_OK;
}

HRESULT CVIBuffer_Terrian::Initialize(void* pArg)
{

    return S_OK;
}

void CVIBuffer_Terrian::Render_VIBuffer()
{
    __super::Render_VIBuffer();
}

HRESULT CVIBuffer_Terrian::ExportHeightMap(const WCHAR* ExportFilePath)
{
	//WCHAR       Extension[MAX_PATH] = {};
	//WCHAR       FileName[MAX_PATH] = {};
	//_wsplitpath_s(ExportFilePath, nullptr, 0, nullptr, 0, FileName, MAX_PATH, Extension, MAX_PATH);
	//wsprintf(FileName, TEXT("%s%s"), FileName, Extension);

	ID3D11Texture2D* pTexture2D = nullptr;
	D3D11_TEXTURE2D_DESC TexDesc = {};

	TexDesc.Width = (_uInt)m_iNumVertex.x;
	TexDesc.Height = (_uInt)m_iNumVertex.y;
	TexDesc.MipLevels = 1;
	TexDesc.ArraySize = 1;
	TexDesc.Format = DXGI_FORMAT_R32_FLOAT;

	TexDesc.Usage = D3D11_USAGE_DEFAULT;
	TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TexDesc.CPUAccessFlags = 0;
	TexDesc.SampleDesc.Count = 1;

	m_pDevice->CreateTexture2D(&TexDesc, nullptr, &pTexture2D);

	float* pData = new float[m_iNumVertices];
	for (_uInt i = 0; i < m_iNumVertices; ++i)
		pData[i] = m_pVertices[i].y;

	m_pContext->UpdateSubresource(
		pTexture2D,							// GPU 텍스처
		0,									// Subresource
		nullptr,							// Box (전체 복사)
		pData,								// CPU 데이터
		(_uInt)(m_iNumVertex.x * sizeof(_float)),	// RowPitch
		0									// DepthPitch
	);

	SaveWICTextureToFile(m_pContext, pTexture2D,
						 GUID_ContainerFormatPng,
						 ExportFilePath);

	Safe_Release(pTexture2D);
	Safe_Delete_Array(pData);

	return S_OK;
}

CVIBuffer_Terrian* CVIBuffer_Terrian::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const WCHAR* pHegithFilePath)
{
    CVIBuffer_Terrian* pVIBuffer_Terrian = new CVIBuffer_Terrian(pDevice, pContext);
    if (FAILED(pVIBuffer_Terrian->Initialize_Prototype(pHegithFilePath)))
    {
        Safe_Release(pVIBuffer_Terrian);
        MSG_BOX("CREATE FAIL : VI_BUFFER_TERRIAN");
    }

    return pVIBuffer_Terrian;
}

CVIBuffer_Terrian* CVIBuffer_Terrian::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt TileCnt)
{
	CVIBuffer_Terrian* pVIBuffer_Terrian = new CVIBuffer_Terrian(pDevice, pContext);
	if (FAILED(pVIBuffer_Terrian->Initialize_Prototype(TileCnt)))
	{
		Safe_Release(pVIBuffer_Terrian);
		MSG_BOX("CREATE FAIL : VI_BUFFER_TERRIAN");
	}

	return pVIBuffer_Terrian;
}

CComponent* CVIBuffer_Terrian::Clone(void* pArg)
{
    CVIBuffer_Terrian* pVIBuffer_Terrian = new CVIBuffer_Terrian(*this);
    if (FAILED(pVIBuffer_Terrian->Initialize(pArg)))
    {
        Safe_Release(pVIBuffer_Terrian);
        MSG_BOX("CLONE FAIL : VI_BUFFER_TERRIAN");
    }

    return pVIBuffer_Terrian;
}

void CVIBuffer_Terrian::Free()
{
    __super::Free();
}
