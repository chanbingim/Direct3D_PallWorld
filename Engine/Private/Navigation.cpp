#include "Navigation.h"

#include "GameInstance.h"
#include "NaviMeshPaserStruct.h"
#include "Cell.h"

_float4x4 CNavigation::m_WorldMatrix = {};

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CComponent(pDevice, pContext)
{
}

CNavigation::CNavigation(const CNavigation& rhs) : 
	CComponent(rhs),
	m_Cells(rhs.m_Cells),
	m_pShaderCom(rhs.m_pShaderCom)
{
	for (auto& iter : m_Cells)
		Safe_AddRef(iter);
	Safe_AddRef(m_pShaderCom);
}

HRESULT CNavigation::Initialize_Prototype(const _tchar* pNavigationDataFiles)
{

#ifdef _DEBUG
	m_pShaderCom = CShader::Create(m_pDevice, m_pContext, VTX_COL::Elements, VTX_COL::iNumElements, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"));
	if (nullptr == m_pShaderCom)
		return E_FAIL;
	BindSahderResource();
#endif

	return S_OK;
}

HRESULT CNavigation::Initialize_Prototype(const CVIBuffer_Terrain* pTerrianBuffer)
{
	m_iNumNaviSize = pTerrianBuffer->GetTerrianSize();
	auto pVertices = pTerrianBuffer->GetVerticesPoint();

	for (_uInt i = 0; i < (_uInt)m_iNumNaviSize.y - 1; ++i)
	{
		for (_uInt j = 0; j < (_uInt)m_iNumNaviSize.x - 1; ++j)
		{
			_uInt iIndex = i * (_uInt)m_iNumNaviSize.x + j;
			_uInt		iIndices[4] = {
				iIndex + (_uInt)m_iNumNaviSize.x,
				iIndex + (_uInt)m_iNumNaviSize.x + 1,
				iIndex + 1,
				iIndex
			};

			_float3 vPoints[ENUM_CLASS(NAVI_POINT::END)] = {pVertices[iIndices[0]], pVertices[iIndices[2]], pVertices[iIndices[3]] };
			CCell* pCell = CCell::Create(m_pDevice, m_pContext, (_uInt)m_Cells.size(), 0, vPoints);
			if (nullptr == pCell)
				return E_FAIL;
			m_Cells.push_back(pCell);

			vPoints[ENUM_CLASS(NAVI_POINT::A)] = pVertices[iIndices[0]];
			vPoints[ENUM_CLASS(NAVI_POINT::B)] = pVertices[iIndices[1]];
			vPoints[ENUM_CLASS(NAVI_POINT::C)] = pVertices[iIndices[2]];
			pCell = CCell::Create(m_pDevice, m_pContext, (_uInt)m_Cells.size(), 0, vPoints);
			if (nullptr == pCell)
				return E_FAIL;
			m_Cells.push_back(pCell);
		}
	}

#ifdef _DEBUG
	m_pShaderCom = CShader::Create(m_pDevice, m_pContext, VTX_COL::Elements, VTX_COL::iNumElements, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"));
	if (nullptr == m_pShaderCom)
		return E_FAIL;
#endif

	SetUpNeighbors();
	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	NAVIGATION_DESC* pDesc = static_cast<NAVIGATION_DESC*>(pArg);

	m_iCurrentCellIndex = pDesc->iCurrentCellIndex;
	
	return S_OK;
}

void CNavigation::Update(_matrix WorldMatrix)
{
	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
}

_bool CNavigation::IsMove(_vector vPosition)
{
	_vector		vLocalPos = XMVector3TransformCoord(vPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
	_Int		iNeighborIndex = { -1 };
	_bool		isIn = m_Cells[m_iCurrentCellIndex]->IsCellIn(vLocalPos, &iNeighborIndex);
	if (isIn)
		return true;
	else
	{
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex || !m_Cells[iNeighborIndex]->IsMoveAble())
					return false;

				if (m_Cells[iNeighborIndex]->IsCellIn(vLocalPos, &iNeighborIndex))
					break;
			}
			m_iCurrentCellIndex = iNeighborIndex;
			return true;
		}
		else
			false;
	}

	return false;
}

void CNavigation::ComputeHeight(CTransform* pTransform)
{
	_float3		vPos = pTransform->GetPosition();
	_vector		vLocalPos = XMVector3TransformCoord(XMLoadFloat3(&vPos), XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
	_float		fHeight = m_Cells[m_iCurrentCellIndex]->ComputeHeight(vLocalPos);

	vPos.y = fHeight;
	pTransform->SetPosition(vPos);
}

_Int CNavigation::Find_Cell(_vector vPos)
{
	_vector		vLocalPos = XMVector3TransformCoord(vPos, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	_Int		iNeighborIndex = { -1 };
	for (auto& pCell : m_Cells)
	{
		pCell->IsCellIn(vPos, &iNeighborIndex);

		if (m_Cells[iNeighborIndex]->IsCellIn(vLocalPos, &iNeighborIndex))
			return pCell->GetCellIndex();
	}

	return -1;
}

void CNavigation::ComputePathfindingAStar(_float3 vStartPoint, _float3 vTargetPoint, list<_float3>* PathList)
{
	priority_queue<A_StarNode, vector<A_StarNode>, CompareNode> OpenList;
	unordered_map<_uInt, A_StarNode> HashList;

	unordered_map<_uInt, A_StarNode> CloseList;
	_Int  iNeighbor[3];
	//현재 셀과 vTargtetPoint
	vTargetPoint.y = vStartPoint.y = 0;

	_vector vStart = XMLoadFloat3(&vStartPoint);
	_vector vEnd = XMLoadFloat3(&vTargetPoint);
	vStart = XMVector3TransformCoord(vStart, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
	vEnd = XMVector3TransformCoord(vEnd, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
	_Int EndIndex = Find_Cell(vEnd);

	_vector TriCenterPoint = m_Cells[m_iCurrentCellIndex]->GetCellCenterPoint();
	TriCenterPoint.m128_f32[1] = 0.f;
	_float fLength = XMVectorGetX(XMVector3Length(vEnd - TriCenterPoint));
	if (fLength < 1)
		return;
	else
	{
		//처음 시작점을 세팅한다.
		A_StarNode StartNode = {};
		StartNode.fManhaattanDistance = fLength;
		StartNode.fTotalCost = StartNode.fManhaattanDistance + StartNode.fHeuristicCost;
		StartNode.iCellIndex = m_iCurrentCellIndex;
		StartNode.ParentIndex = -1;
		OpenList.push(StartNode);
		HashList.emplace(m_iCurrentCellIndex, StartNode);

		while (!OpenList.empty())
		{
			A_StarNode ParentNode = OpenList.top();
			OpenList.pop();
			
			if (EndIndex == ParentNode.iCellIndex)
			{
				CloseList.emplace(ParentNode.iCellIndex, ParentNode);
				break;
			}

			auto Closeiter = CloseList.find(ParentNode.iCellIndex);
			if (Closeiter != CloseList.end())
				continue;

			m_Cells[ParentNode.iCellIndex]->GetNeighborIndex(iNeighbor);
			for (_uInt i = 0; i < 3; ++i)
			{
				Closeiter = CloseList.find(iNeighbor[i]);
				if (-1 == iNeighbor[i] || !m_Cells[iNeighbor[i]]->IsMoveAble() || Closeiter != CloseList.end() )
					continue;

				A_StarNode NeighNode = {};
				NeighNode.fHeuristicCost = ParentNode.fHeuristicCost + 1.f;

				TriCenterPoint = m_Cells[iNeighbor[i]]->GetCellCenterPoint();
				TriCenterPoint.m128_f32[1] = 0.f;

				NeighNode.fManhaattanDistance = XMVectorGetX(XMVector3Length(vEnd - TriCenterPoint));
				NeighNode.fTotalCost = NeighNode.fManhaattanDistance + NeighNode.fHeuristicCost;
				NeighNode.iCellIndex = iNeighbor[i];
				NeighNode.ParentIndex = ParentNode.iCellIndex;

				auto Openiter = HashList.find(iNeighbor[i]);
				if (Openiter != HashList.end())
				{
					if (Openiter->second.fTotalCost > NeighNode.fTotalCost)
					{
						Openiter->second = NeighNode;
						OpenList.push(NeighNode);
					}
				}
				else
				{
					OpenList.push(NeighNode);
					HashList.emplace(iNeighbor[i], NeighNode);
				}
			}
			CloseList.emplace(ParentNode.iCellIndex, ParentNode);
		}
	}

	_float3 vPos;
	auto EndNode = CloseList.find(EndIndex);
	while (-1 != EndNode->second.ParentIndex)
	{
		XMStoreFloat3(& vPos, m_Cells[EndNode->second.iCellIndex]->GetCellCenterPoint());
		(*PathList).push_back(vPos);

		EndNode = CloseList.find(EndNode->second.ParentIndex);
	}
	(*PathList).reverse();
}

HRESULT CNavigation::Export(const char* FilePath)
{
	vector<CELL_SAVE_STRCUT> pSaveCells;
	pSaveCells.reserve(m_Cells.size());
	for (auto& iter : m_Cells)
	{
		CELL_SAVE_STRCUT Desc;
		iter->Export(&Desc);

		pSaveCells.push_back(Desc);
	}

	return S_OK;
}

HRESULT CNavigation::Render(_float4 vColor, _bool DarwCurCell)
{
#ifdef _DEBUG
	_float4x4	ComputeMat = m_WorldMatrix;
	BindSahderResource();
	ApplyShaderReSource(vColor);
	if (DarwCurCell)
		ComputeMat._42 += 0.2f;
	else
		ComputeMat._42 += 0.1f;

	m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&ComputeMat));
	m_pShaderCom->Update_Shader(0);
	if (DarwCurCell)
	{
		m_Cells[m_iCurrentCellIndex]->Render();
	}
	else
	{
		_Int iStart	= Clamp<_Int>(m_iCurrentCellIndex - 25, 0, (_Int)m_Cells.size());
		_Int iEnd		= Clamp<_Int>(m_iCurrentCellIndex + 25, 0, (_Int)m_Cells.size());

		for (_Int i = iStart; i < iEnd; ++i)
		{
			for (_Int j = iStart; j < iEnd; ++j)
			{
				_Int iIndex = i * (_uInt)m_iNumNaviSize.x + j;
				m_Cells[iIndex]->Render();
			}
		}
	}
#endif // _DEBUG

	return S_OK;
}

void CNavigation::BindSahderResource()
{
	m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
	m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
	m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
	m_pEVvColor = m_pShaderCom->GetVariable("g_Color");
}

void CNavigation::ApplyShaderReSource(_float4 vColor)
{

	m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
	m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));
	m_pEVvColor->SetRawValue(&vColor, 0, sizeof(_float4));
}

void CNavigation::SetUpNeighbors()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare(pSourCell->GetPoint(NAVI_POINT::A), pSourCell->GetPoint(NAVI_POINT::B)))
			{
				pSourCell->SetNeighbor(NAVI_LINE::AB, pDestCell->GetCellIndex());
			}

			if (true == pDestCell->Compare(pSourCell->GetPoint(NAVI_POINT::B), pSourCell->GetPoint(NAVI_POINT::C)))
			{
				pSourCell->SetNeighbor(NAVI_LINE::BC, pDestCell->GetCellIndex());
			}

			if (true == pDestCell->Compare(pSourCell->GetPoint(NAVI_POINT::C), pSourCell->GetPoint(NAVI_POINT::A)))
			{
				pSourCell->SetNeighbor(NAVI_LINE::CA, pDestCell->GetCellIndex());
			}
		}
	}
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFiles)
{
	CNavigation* pNavigation = new CNavigation(pDevice, pContext);
	if (FAILED(pNavigation->Initialize_Prototype(pNavigationDataFiles)))
	{
		Safe_Release(pNavigation);
		MSG_BOX("CREATE FAIL : NAVIGATION");
	}
	return pNavigation;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CVIBuffer_Terrain* pTerrianBuffer)
{
	CNavigation* pNavigation = new CNavigation(pDevice, pContext);
	if (FAILED(pNavigation->Initialize_Prototype(pTerrianBuffer)))
	{
		Safe_Release(pNavigation);
		MSG_BOX("CREATE FAIL : NAVIGATION");
	}
	return pNavigation;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pNavigation = new CNavigation(*this);
	if (FAILED(pNavigation->Initialize(pArg)))
	{
		Safe_Release(pNavigation);
		MSG_BOX("CLONE FAIL : NAVIGATION");
	}
	return pNavigation;
}

void CNavigation::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pShaderCom);
#endif

	for (auto& iter : m_Cells)
		Safe_Release(iter);
	m_Cells.clear();
}
