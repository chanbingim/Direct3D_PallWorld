#include "VIBuffer_Terrain.h"

#include "GameInstance.h"
#include "StringHelper.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs) :
    CVIBuffer(rhs),
	m_iNumVertex(rhs.m_iNumVertex)
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(_uInt TileCnt)
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
	VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const WCHAR* pHegithFilePath)
{
	char		FilePath[MAX_PATH], szExtend[MAX_PATH];

	CStringHelper::ConvertWideToUTF(pHegithFilePath, FilePath);
	_splitpath_s(FilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExtend, MAX_PATH);

	HRESULT hr = {};
	if (!strcmp(szExtend, ".png"))
		hr = CreateWICTerrian(pHegithFilePath);
	else if (!strcmp(szExtend, ".tga"))
	{
		MSG_BOX("NOT READ FILE");
		hr = E_FAIL;
	}
	else
		hr = CreateBMPTerrian(pHegithFilePath);

    return hr;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{

    return S_OK;
}

void CVIBuffer_Terrain::Render_VIBuffer()
{
    __super::Render_VIBuffer();
}

HRESULT CVIBuffer_Terrain::ExportHeightMap(const WCHAR* ExportFilePath)
{
	ID3D11Texture2D* pSaveTex = nullptr;
	D3D11_TEXTURE2D_DESC TexDesc = {};
	TexDesc.Width = m_iNumVertex.x;
	TexDesc.Height = m_iNumVertex.y;
	TexDesc.MipLevels = 1;
	TexDesc.ArraySize = 1;
	TexDesc.SampleDesc.Count = 1;
	TexDesc.SampleDesc.Quality = 0;

	TexDesc.Usage = D3D11_USAGE_DEFAULT;
	TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TexDesc.CPUAccessFlags = 0;
	TexDesc.MiscFlags = 0;

	unsigned __int8* pPixel = new unsigned __int8[m_iNumVertices * 4];
	for (_uInt i = 0; i < m_iNumVertex.y; ++i)
	{
		for (_uInt j = 0; j < m_iNumVertex.x; ++j)
		{
			_uInt iIndex = i * m_iNumVertex.x + j;
			_uInt iPixelIndex = iIndex * 4;

			pPixel[iPixelIndex] = m_pVertices[iIndex].y;
			pPixel[iPixelIndex + 1] = m_pVertices[iIndex].y;
			pPixel[iPixelIndex + 2] = m_pVertices[iIndex].y;
			pPixel[iPixelIndex + 3] = 255;
		}
	}
	D3D11_SUBRESOURCE_DATA SubData;
	
	SubData.pSysMem = pPixel;
	SubData.SysMemPitch = sizeof(_uInt) * m_iNumVertex.x;
	SubData.SysMemSlicePitch = 0;
	if (FAILED(m_pDevice->CreateTexture2D(&TexDesc, &SubData, &pSaveTex)))
		return E_FAIL;

	SaveWICTextureToFile(m_pContext, pSaveTex, GUID_ContainerFormatPng, ExportFilePath);
	Safe_Delete_Array(pPixel);
	Safe_Release(pSaveTex);

	return S_OK;
}

_bool CVIBuffer_Terrain::IsPicking(CTransform* pTransform, _float3* pOut)
{
	_matrix InvWorld = XMLoadFloat4x4(&pTransform->GetInvWorldMat());
	m_pGameInstance->Compute_LocalRay(&InvWorld);

	_uInt	iNumIndices = {};

	for (_uInt i = 0; i < (_uInt)m_iNumVertex.y - 1; i++)
	{
		for (_uInt j = 0; j < (_uInt)m_iNumVertex.x - 1; j++)
		{
			_uInt	iIndex = i * (_uInt)m_iNumVertex.x + j;

			_uInt	iIndices[4] = {
				iIndex + (_uInt)m_iNumVertex.x,
				iIndex + (_uInt)m_iNumVertex.x + 1,
				iIndex + 1,
				iIndex
			};

			//하단 삼각형
			if (true == m_pGameInstance->Picking_InLocal(m_pVertices[iIndices[0]], m_pVertices[iIndices[1]], m_pVertices[iIndices[2]], pOut))
			{
				XMStoreFloat3(pOut, XMVector3TransformCoord(XMLoadFloat3(pOut), XMLoadFloat4x4(&pTransform->GetWorldMat())));
				return true;
			}

			//상단 삼각형
			if (true == m_pGameInstance->Picking_InLocal(m_pVertices[iIndices[0]], m_pVertices[iIndices[2]], m_pVertices[iIndices[3]], pOut))
			{
				XMStoreFloat3(pOut, XMVector3TransformCoord(XMLoadFloat3(pOut), XMLoadFloat4x4(&pTransform->GetWorldMat())));
				return true;
			}
		}
	}

	return false;
}

_bool CVIBuffer_Terrain::IsPicking(_vector vRayOrizin, _vector vRayDir, CTransform* pTransform, _float3* pOut)
{
	_matrix InvWorld = XMLoadFloat4x4(&pTransform->GetInvWorldMat());

	vRayOrizin = XMVector3TransformCoord(vRayOrizin, InvWorld);

	vRayDir = XMVector3TransformNormal(vRayDir, InvWorld);
	vRayDir = XMVector3Normalize(vRayDir);

	_uInt	iNumIndices = {};
	for (_uInt i = 0; i < (_uInt)m_iNumVertex.y - 1; i++)
	{
		for (_uInt j = 0; j < (_uInt)m_iNumVertex.x - 1; j++)
		{
			_uInt	iIndex = i * (_uInt)m_iNumVertex.x + j;

			_uInt	iIndices[4] = {
				iIndex + (_uInt)m_iNumVertex.x,
				iIndex + (_uInt)m_iNumVertex.x + 1,
				iIndex + 1,
				iIndex
			};

			//하단 삼각형
			if (true == m_pGameInstance->RayPicking(vRayOrizin, vRayDir, m_pVertices[iIndices[0]], m_pVertices[iIndices[1]], m_pVertices[iIndices[2]], pOut))
			{
				XMStoreFloat3(pOut, XMVector3TransformCoord(XMLoadFloat3(pOut), XMLoadFloat4x4(&pTransform->GetWorldMat())));
				return true;
			}

			//상단 삼각형
			if (true == m_pGameInstance->RayPicking(vRayOrizin, vRayDir, m_pVertices[iIndices[0]], m_pVertices[iIndices[2]], m_pVertices[iIndices[3]], pOut))
			{
				XMStoreFloat3(pOut, XMVector3TransformCoord(XMLoadFloat3(pOut), XMLoadFloat4x4(&pTransform->GetWorldMat())));
				return true;
			}
		}
	}

	return false;
}

HRESULT CVIBuffer_Terrain::UpdateHegiht(CTransform* pTransform, _float Apply, _float Offset)
{
	/*if (!ComputeBoundingBox(pTransform))
		return E_FAIL;*/

	_float3 pOut = {};
	if (IsPicking(pTransform, &pOut))
	{
		_vector MinPos = { pOut.x - Offset, 0.f,  pOut.z - Offset, 1.f };
		_vector MaxPos = { pOut.x + Offset, 0.f,  pOut.z + Offset, 1.f };

		MinPos = XMVector3TransformCoord(MinPos, XMLoadFloat4x4(&pTransform->GetInvWorldMat()));
		MaxPos = XMVector3TransformCoord(MaxPos, XMLoadFloat4x4(&pTransform->GetInvWorldMat()));
	
		_float3 LocalMinPos{}, LocalMaxPos{};
		XMStoreFloat3(&LocalMinPos, MinPos);
		XMStoreFloat3(&LocalMaxPos, MaxPos);

		//범위를 제한한다 0 ~ 정점 최대 수까지
		LocalMinPos.x = Clamp<_float>(LocalMinPos.x, 0.f, m_iNumVertex.x);
		LocalMinPos.z = Clamp<_float>(LocalMinPos.z, 0.f, m_iNumVertex.y);
		
		LocalMaxPos.x = Clamp<_float>(LocalMaxPos.x, 0.f, m_iNumVertex.x);
		LocalMaxPos.z = Clamp<_float>(LocalMaxPos.z, 0.f, m_iNumVertex.y);

		D3D11_MAPPED_SUBRESOURCE MapData;
		if (S_OK == m_pContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MapData))
		{
			//피킹하는거 더 간단하게 바뀌면
			//그때 Normal 계산하는 로직까지 추가로해서 할것
			VTX_NORTEX* Vertex = (VTX_NORTEX*)MapData.pData;

			for (_uInt i = (_uInt)LocalMinPos.z; i < (_uInt)LocalMaxPos.z - 1; i++)
			{
				for (_uInt j = (_uInt)LocalMinPos.x; j < (_uInt)LocalMaxPos.x - 1; j++)
				{
					_uInt	iIndex = i * (_uInt)m_iNumVertex.x + j;
					_uInt	iIndices[4] = {
						iIndex + (_uInt)m_iNumVertex.x,
						iIndex + (_uInt)m_iNumVertex.x + 1,
						iIndex + 1,
						iIndex
					};

					// 이게 버퍼를 공유해서 문제가 발생함
					// 버퍼 공유 없이 vertexPos 값으로 지형 셰이더에 정점넘겨서
					// 변환 해서 사용하자
					m_pVertices[iIndices[0]].y = Clamp<_float>(m_pVertices[iIndices[0]].y + Apply, 0.f, 255.f);
					m_pVertices[iIndices[1]].y = Clamp<_float>(m_pVertices[iIndices[0]].y + Apply, 0.f, 255.f);
					m_pVertices[iIndices[2]].y = Clamp<_float>(m_pVertices[iIndices[0]].y + Apply, 0.f, 255.f);
					m_pVertices[iIndices[3]].y = Clamp<_float>(m_pVertices[iIndices[0]].y + Apply, 0.f, 255.f);

					Vertex[iIndices[0]].vPosition.y = m_pVertices[iIndices[0]].y;
					Vertex[iIndices[1]].vPosition.y = m_pVertices[iIndices[1]].y;
					Vertex[iIndices[2]].vPosition.y = m_pVertices[iIndices[2]].y;
					Vertex[iIndices[3]].vPosition.y = m_pVertices[iIndices[3]].y;
				}
			}

			m_pContext->Unmap(m_pVertexBuffer, 0);
		}

		return S_OK;
	}
	
	return E_FAIL;
}

HRESULT CVIBuffer_Terrain::CreateWICTerrian(const WCHAR* pHegithFilePath)
{
	ID3D11ShaderResourceView* newResource = nullptr;
	ID3D11Resource* HeightMapData = nullptr;
	if(FAILED(CreateWICTextureFromFile(m_pDevice, pHegithFilePath, &HeightMapData, &newResource)))
		return E_FAIL;

	ID3D11Texture2D* pTexHeight = static_cast<ID3D11Texture2D*>(HeightMapData);
	D3D11_TEXTURE2D_DESC TexDesc = {};
	pTexHeight->GetDesc(&TexDesc);

	m_iNumVertex.x = (_float)TexDesc.Width;
	m_iNumVertex.y = (_float)TexDesc.Height;
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

	ID3D11Texture2D* pStagingTex = nullptr;
	TexDesc.MipLevels = 1;
	TexDesc.ArraySize = 1;

	TexDesc.Usage = D3D11_USAGE_STAGING;
	TexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	TexDesc.BindFlags = 0;

	m_pDevice->CreateTexture2D(&TexDesc, nullptr, &pStagingTex);
	m_pContext->CopyResource(pStagingTex, pTexHeight);

	D3D11_MAPPED_SUBRESOURCE Map;
	if (S_OK == m_pContext->Map(pStagingTex, 0, D3D11_MAP_READ, 0, &Map))
	{
		_uInt* pPixels = reinterpret_cast<_uInt*>(Map.pData);
		for (_float i = 0; i < m_iNumVertex.y; i++)
		{
			for (_float j = 0; j < m_iNumVertex.x; j++)
			{
				_uInt		iIndex = _uInt(i * m_iNumVertex.x + j);

				m_pVertices[iIndex] = pVertices[iIndex].vPosition = _float3(j, (pPixels[iIndex] & 0x000000ff), i);
				pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
				pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVertex.x - 1.f), i / (m_iNumVertex.y - 1.f));
			}
		}
	}
	m_pContext->Unmap(pStagingTex, 0);
	Safe_Release(pStagingTex);
	Safe_Release(HeightMapData);
	Safe_Release(newResource);

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

HRESULT CVIBuffer_Terrain::CreateBMPTerrian(const WCHAR* pHegithFilePath)
{
	_ulong			dwByte = {};
	HANDLE			hFile = CreateFile(pHegithFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	BITMAPFILEHEADER			fh{};
	BITMAPINFOHEADER			ih{};

	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);

	m_iNumVertex.x = ih.biWidth;
	m_iNumVertex.y = ih.biHeight;
	m_iNumVertices = m_iNumVertex.x * m_iNumVertex.y;

	_uInt* pPixels = new _uInt[m_iNumVertices];
	ZeroMemory(pPixels, sizeof(_uInt) * m_iNumVertices);

	ReadFile(hFile, pPixels, sizeof(_uInt) * m_iNumVertices, &dwByte, nullptr);

	m_iNumVertexBuffers = 1;

	m_iVertexStride = sizeof(VTX_NORTEX);

	m_iNumIndices = (m_iNumVertex.x - 1) * (m_iNumVertex.y - 1) * 2 * 3;
	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	VTX_NORTEX* pVertices = new VTX_NORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTX_NORTEX) * m_iNumVertices);

	m_pVertices = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(_float3) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertex.y; i++)
	{
		for (size_t j = 0; j < m_iNumVertex.x; j++)
		{
			_uInt		iIndex = i * m_iNumVertex.x + j;

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

	for (size_t i = 0; i < m_iNumVertex.y - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVertex.x - 1; j++)
		{
			_uInt		iIndex = i * m_iNumVertex.x + j;

			_uInt		iIndices[4] = {
				iIndex + m_iNumVertex.x,
				iIndex + m_iNumVertex.x + 1,
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
	{
		XMStoreFloat3(&pVertices[i].vNormal,
			XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
	}

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

_bool CVIBuffer_Terrain::ComputeBoundingBox(CTransform* pTransform)
{
	BoundingBox LocalBoundBox, WorldBoundBox;

	_vector vMin = { m_pVertices[0].x , 0.f, m_pVertices[0].z };
	_vector vMax = { m_pVertices[m_iNumVertices - 1].x , 1.f, m_pVertices[m_iNumVertices - 1].z };
	//vMin = XMVector3TransformCoord(vMin, XMLoadFloat4x4(&pTransform->GetWorldMat()));
	//vMax = XMVector3TransformCoord(vMax, XMLoadFloat4x4(&pTransform->GetWorldMat()));

	BoundingBox::CreateFromPoints(LocalBoundBox, vMin, vMax);
	LocalBoundBox.Transform(WorldBoundBox, XMLoadFloat4x4(&pTransform->GetWorldMat()));

	_float dist = {};
	_vector WorldRayPos = XMLoadFloat3(&m_pGameInstance->GetPickingRayPos(RAY::WORLD));
	_vector WorldRayDir = XMLoadFloat3(&m_pGameInstance->GetPickingRayDir(RAY::WORLD));

	return WorldBoundBox.Intersects(WorldRayPos, WorldRayDir, dist);
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const WCHAR* pHegithFilePath)
{
    CVIBuffer_Terrain* pVIBuffer_Terrian = new CVIBuffer_Terrain(pDevice, pContext);
    if (FAILED(pVIBuffer_Terrian->Initialize_Prototype(pHegithFilePath)))
    {
        Safe_Release(pVIBuffer_Terrian);
        MSG_BOX("CREATE FAIL : VI_BUFFER_TERRIAN");
    }

    return pVIBuffer_Terrian;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt TileCnt)
{
	CVIBuffer_Terrain* pVIBuffer_Terrian = new CVIBuffer_Terrain(pDevice, pContext);
	if (FAILED(pVIBuffer_Terrian->Initialize_Prototype(TileCnt)))
	{
		Safe_Release(pVIBuffer_Terrian);
		MSG_BOX("CREATE FAIL : VI_BUFFER_TERRIAN");
	}

	return pVIBuffer_Terrian;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
    CVIBuffer_Terrain* pVIBuffer_Terrian = new CVIBuffer_Terrain(*this);
    if (FAILED(pVIBuffer_Terrian->Initialize(pArg)))
    {
        Safe_Release(pVIBuffer_Terrian);
        MSG_BOX("CLONE FAIL : VI_BUFFER_TERRIAN");
    }

    return pVIBuffer_Terrian;
}

void CVIBuffer_Terrain::Free()
{
    __super::Free();
}
