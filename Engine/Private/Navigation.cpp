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
	m_pShaderCom(rhs.m_pShaderCom),
	m_MinPoint(rhs.m_MinPoint),
	m_MaxPoint(rhs.m_MaxPoint),
	m_vCenterPoint(rhs.m_vCenterPoint)
{
	for (auto& iter : m_Cells)
		Safe_AddRef(iter);
	Safe_AddRef(m_pShaderCom);
}

HRESULT CNavigation::Initialize_Prototype(const char* pNavigationDataFilePath)
{
	ReadNaviMeshDataFile(pNavigationDataFilePath);

#ifdef _DEBUG
	m_pShaderCom = CShader::Create(m_pDevice, m_pContext, VTX_POINT::Elements, VTX_POINT::iNumElements, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"));
	if (nullptr == m_pShaderCom)
		return E_FAIL;
	BindSahderResource();
#endif
	SetUpNeighbors();
	return S_OK;
}

HRESULT CNavigation::Initialize_Prototype(const CModel* pMapModel)
{
	//Bowyer_WatsonAlgorithm(pMapModel, iMeshNum);

#ifdef _DEBUG
	m_pShaderCom = CShader::Create(m_pDevice, m_pContext, VTX_POINT::Elements, VTX_POINT::iNumElements, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"));
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

_float CNavigation::ComputeHeight(CTransform* pTransform)
{
	_float3		vPos = pTransform->GetPosition();
	_vector		vLocalPos = XMVector3TransformCoord(XMLoadFloat3(&vPos), XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	if (-1 == m_iCurrentCellIndex)
		return -1;

	_float		fHeight = m_Cells[m_iCurrentCellIndex]->ComputeHeight(vLocalPos);
	return fHeight;
}

void CNavigation::ComputeHeight(CTransform* pTransform, _bool bIsFindCell)
{
	_float3		vPos = pTransform->GetPosition();
	_vector		vLocalPos = XMVector3TransformCoord(XMLoadFloat3(&vPos), XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
	
	_uInt iIndex = m_iCurrentCellIndex;
	if (bIsFindCell)
		iIndex = Find_Cell(vLocalPos);
	if (-1 == iIndex)
		return;

	_float		fHeight = m_Cells[iIndex]->ComputeHeight(vLocalPos);

	vPos.y = fHeight;
	pTransform->SetPosition(vPos);
}

void CNavigation::ComputeHeight(_float3* pPosition)
{
	_float3		vPos = *pPosition;
	_vector		vLocalPos = XMVector3TransformCoord(XMLoadFloat3(&vPos), XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
	_uInt iIndex = Find_Cell(vLocalPos);

	if (-1 == iIndex)
		return;

	_float		fHeight = m_Cells[iIndex]->ComputeHeight(vLocalPos);
	pPosition->y = fHeight;
}

_Int CNavigation::Find_Cell(_vector vPos)
{
	_vector		vLocalPos = XMVector3TransformCoord(vPos, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	_Int		iNeighborIndex = { -1 };
	for (auto& pCell : m_Cells)
	{
		if (pCell->IsCellIn(vLocalPos, &iNeighborIndex))
			return pCell->GetCellIndex();
	}

	return -1;
}

_Int CNavigation::Find_CellEdge(_vector vPos)
{
	_vector		vLocalPos = XMVector3TransformCoord(vPos, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
	_Int		iNeighborIndex = { -1 };

	for (auto iIndex : m_EdgeCellIndex)
	{
		if (m_Cells[iIndex]->IsCellIn(vPos, &iNeighborIndex))
			return iIndex;
	}

	return -1;
}

_float3 CNavigation::CellCenterPos(_uInt iCellIndex)
{
	if (m_Cells.size() <= iCellIndex || 0 > iCellIndex)
		return { 0.f ,0.f, 0.f };

	_vector		vLocalPos = XMVector3TransformCoord(m_Cells[iCellIndex]->GetCellCenterPoint(), XMLoadFloat4x4(&m_WorldMatrix));
	_float3 vPos;
	XMStoreFloat3(&vPos, vLocalPos);
	return vPos;
}

void CNavigation::ComputePathfindingAStar(_float3 vStartPoint, _float3 vTargetPoint, list<_float3>* PathList)
{
	m_PathCells.clear();
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

	if (-1 == m_iCurrentCellIndex || -1 == EndIndex)
		return;

	_vector TriCenterPoint = m_Cells[m_iCurrentCellIndex]->GetCellCenterPoint();
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
				if (-1 == iNeighbor[i])
					continue;

				if (!m_Cells[iNeighbor[i]]->IsMoveAble() || Closeiter != CloseList.end())
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
	if (OpenList.empty() || CloseList.empty())
		return;

	auto EndNode = CloseList.find(EndIndex);
	if (EndNode == CloseList.end())
		return;

	while (-1 != EndNode->second.ParentIndex)
	{
		m_PathCells.push_back(make_pair(EndNode->second.ParentIndex, EndNode->second.iCellIndex));
		EndNode = CloseList.find(EndNode->second.ParentIndex);
		if (EndNode == CloseList.end())
			break;
	}

	m_PathCells.reverse();
	SimpleFunnelAlgorithm(vStart, PathList);
	PathList->push_back(vTargetPoint);
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
		for (auto Cell : m_Cells)
			Cell->Render();
	}
#endif // _DEBUG

	return S_OK;
}

_float3 CNavigation::DrawTriangle(_vector vPos, _float Radius)
{
	_float3 vPoint = { -1, -1, -1};
	for (auto Celliter = m_Cells.begin(); Celliter != m_Cells.end(); ++Celliter)
	{
		for (_uInt i = 0; i < ENUM_CLASS(NAVI_POINT::END); ++i)
		{
			_float fLength = XMVectorGetX(XMVector3Length((*Celliter)->GetCellPoint(NAVI_POINT(i)) - vPos));
			if (Radius >= fLength)
			{
				Radius = fLength;
				XMStoreFloat3(&vPoint, (*Celliter)->GetCellPoint(NAVI_POINT(i)));
			}
		}
	}
	return vPoint;
}

HRESULT CNavigation::InsertTriangle(NAVI_TRIANGLE& TriAngleDesc)
{
	

	_vector PointA = XMLoadFloat3(&TriAngleDesc.A);
	_vector PointB = XMLoadFloat3(&TriAngleDesc.B);
	_vector PointC = XMLoadFloat3(&TriAngleDesc.C);
	_vector vCross = XMVector3Cross(PointB - PointA, PointC - PointA);

	if (0 > XMVectorGetY(vCross))
	{
		_float3 temp = TriAngleDesc.B;
		TriAngleDesc.B = TriAngleDesc.C;
		TriAngleDesc.C = temp;
	}

	_float3 vPoints[ENUM_CLASS(NAVI_POINT::END)] = { TriAngleDesc.A , TriAngleDesc.B, TriAngleDesc.C };
	CCell* pCell = CCell::Create(m_pDevice, m_pContext, (_uInt)m_Cells.size(), 0, vPoints);
	if (nullptr == pCell)
		return E_FAIL;

	m_Cells.push_back(pCell);
	m_Triangles.push_back(TriAngleDesc);
	return S_OK;
}

void CNavigation::RemoveCell(_vector vPos, _float Radius)
{
	_vector		vLocalPos = XMVector3TransformCoord(vPos, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
	_Int		iNeighborIndex = { -1 };
	_uInt		iIndex = {};

	auto Triangleiter = m_Triangles.begin();
	for (auto Celliter = m_Cells.begin(); Celliter != m_Cells.end();)
	{
		if (Radius >= XMVectorGetX(XMVector3Length((*Celliter)->GetCellCenterPoint() - vPos)))
		{
			Safe_Release(*Celliter);
			Celliter = m_Cells.erase(Celliter);
			Triangleiter = m_Triangles.erase(Triangleiter);
		}
		else
		{
			Celliter++;
			Triangleiter++;
		}
	}
}

void CNavigation::Bowyer_WatsonAlgorithm(const CModel* pMapModel, _uInt iMeshNum)
{
	_uInt iNumVertices = pMapModel->GetNumVertices(iMeshNum);
	const _float3* Vertices = pMapModel->GetVerticesPoint(iMeshNum);

	_float3 Min{9999, 9999, 9999}, Max{-9999.f, -9999.f, -9999.f };

	for (_uInt i = 0; i < iNumVertices; ++i)
	{
		Min.x = min(Min.x, Vertices[i].x);
		Min.x = min(Min.y, Vertices[i].y);
		Min.z = min(Min.z, Vertices[i].z);

		Max.x = max(Max.x, Vertices[i].x);
		Max.y = max(Max.y, Vertices[i].y);
		Max.z = max(Max.z, Vertices[i].z);

		_bool bFlag = false;
	}

	m_MinPoint = Min;
	m_MaxPoint = Max;
	m_vCenterPoint = { Max.x - Min.x * 0.5f, Max.y - Min.y * 0.5f, Max.z - Min.z * 0.5f };

	//Step 1
	// 슈퍼 트라이앵글 생성
	// 이때 슈퍼 트라이앵글은 내가 만들고자 하는 지형의 크기보다 커야한다.
	// 변이 겹치면 직선이 되서 정확한 결과가 안나옴
	auto SuperTriangle = CreateSuperTriangle(Min, Max);
	m_Triangles.push_back(SuperTriangle);


	//Step 2
	// 점 추가 및 삼각형 갱신
	// 여기서 외접원과 점이 겹치는지 확인해서 새로운 삼각형 쭉쭉 생성해보자
	_uInt Apply = _uInt(iNumVertices * 0.001f);
	Apply = Clamp<_uInt>(Apply, 1, 100);

	for (_uInt i = 0; i < iNumVertices; i += Apply)
	{
		list<NAVI_TRIANGLE> BadTriangles = {};
		list<NAVI_EDGE> Polygon = {};

		_float3 vPoint = Vertices[i];
		//점 추가되면 현재 삼각형 과 비교
		for (auto& PossibleTri : m_Triangles)
		{
			//여기서 확인하고 BadTriangle 추가
			if (PossibleTri.PointInCircumeCircle(XMLoadFloat3(&vPoint)))
				BadTriangles.push_back(PossibleTri);
		}

		for (auto& BadTrianlge : BadTriangles)
		{
			// 삭제해야 하는 삼각형의 변과
			// 현재 들어온 점으로 폴리곤을 만들어낸다.
			for (auto& Edge : BadTrianlge.TriEdge)
			{
				_bool IsUnique = true;

				for (auto& OtherBadTrianlge : BadTriangles)
				{
					if (OtherBadTrianlge == BadTrianlge)
						continue;

					for (auto& OtherEdge : OtherBadTrianlge.TriEdge)
					{
						if (Edge == OtherEdge)
						{
							IsUnique = false;
							break;
						}
					}

					if (false == IsUnique)
						break;
				}

				if (IsUnique)
				{
					//여기서 unique 삼각형이라면 폴리곤 리스트에 추가
					Polygon.push_back(Edge);
				}
			}
		}

		for (auto& BadTrianlge : BadTriangles)
			m_Triangles.remove(BadTrianlge);
	
		//여기서 실질적인 삼각형 생성
		for (auto& Edge : Polygon)
		{
			_vector PointA = XMLoadFloat3(&Edge.A);
			_vector PointB = XMLoadFloat3(&Edge.B);
			_vector PointC = XMLoadFloat3(&vPoint);
			_vector vCross = XMVector3Cross(PointB - PointA, PointC - PointA);

			if (XMVector3Equal(PointA, PointB) || XMVector3Equal(PointA, PointC) || XMVector3Equal(PointB, PointC))
				continue;

			if (0 < XMVectorGetY(vCross))
				m_Triangles.emplace_back(Edge.A, Edge.B, Vertices[i]);
			else
				m_Triangles.emplace_back(Edge.A, Vertices[i], Edge.B);
			
			if (-1 == m_Triangles.back().Radius)
				m_Triangles.pop_back();
		}
	}

	//초기 삼각형 제거
	for (auto Triangle = m_Triangles.begin(); Triangle != m_Triangles.end();)
	{
		if (XMVector3Equal(XMLoadFloat3(&(*Triangle).A), XMLoadFloat3(&SuperTriangle.A)) ||
			XMVector3Equal(XMLoadFloat3(&(*Triangle).A), XMLoadFloat3(&SuperTriangle.B)) ||
			XMVector3Equal(XMLoadFloat3(&(*Triangle).A), XMLoadFloat3(&SuperTriangle.C)) ||
			XMVector3Equal(XMLoadFloat3(&(*Triangle).B), XMLoadFloat3(&SuperTriangle.A)) ||
			XMVector3Equal(XMLoadFloat3(&(*Triangle).B), XMLoadFloat3(&SuperTriangle.B)) ||
			XMVector3Equal(XMLoadFloat3(&(*Triangle).B), XMLoadFloat3(&SuperTriangle.C)) ||
			XMVector3Equal(XMLoadFloat3(&(*Triangle).C), XMLoadFloat3(&SuperTriangle.A)) ||
			XMVector3Equal(XMLoadFloat3(&(*Triangle).C), XMLoadFloat3(&SuperTriangle.B)) ||
			XMVector3Equal(XMLoadFloat3(&(*Triangle).C), XMLoadFloat3(&SuperTriangle.C)))
			Triangle = m_Triangles.erase(Triangle);
		else
			Triangle++;
	}

	//여기서 생성된 삼각형들을 Cell정보로 이용해서 생성한다.
	for (auto Triangle : m_Triangles)
	{
		_float3 vPoints[ENUM_CLASS(NAVI_POINT::END)] = { Triangle.A , Triangle.B, Triangle.C };
		CCell* pCell = CCell::Create(m_pDevice, m_pContext, (_uInt)m_Cells.size(), 0, vPoints);
		if (nullptr == pCell)
			return;

		for (auto i = 0; i < 3; ++i)
		{
			if (Min.x + 50 > vPoints[i].x || Max.x - 50 < vPoints[i].x ||
				Min.z + 50 > vPoints[i].z || Max.z - 50 < vPoints[i].z)
			{
				m_EdgeCellIndex.push_back(pCell->GetCellIndex());
				break;
			}
		}

		m_Cells.push_back(pCell);
	}
}

_bool CNavigation::IsInNaviMesh(_float3 vPos, _float fOffset, _float* pOut)
{
	_vector vCenter = XMLoadFloat3(&m_vCenterPoint);
	_float Range =	XMVectorGetX(XMVector3Length(vCenter - XMLoadFloat3(&m_MinPoint))) + 30.f;

	*pOut = XMVectorGetX(XMVector3Length(vCenter - XMLoadFloat3(&vPos)));
	if (Range + fOffset >= *pOut)
		return true;
	
	return false;
}

_vector CNavigation::GetCurrentCellNoraml()
{
	return m_Cells[m_iCurrentCellIndex]->ComputeNormal();
}

NAVI_TRIANGLE CNavigation::CreateSuperTriangle(_float3 vMin, _float3 vMax)
{
	_float dx = vMax.x - vMin.x;
	_float dz = vMax.z - vMin.z;

	_float3 vPointA = { vMin.x - dx, 0.f, vMax.z + dz * 3 };
	_float3 vPointB = { vMax.x + dx * 3, 0.f, vMin.z - dz };
	_float3 vPointC = { vMin.x - dx, 0.f, vMin.z - dz };

	return NAVI_TRIANGLE(vPointA, vPointB, vPointC);
}

HRESULT CNavigation::ReadNaviMeshDataFile(const char* szFilePath)
{
	_uInt NaviMeshTriCount = {};
	list<NAVI_TRIANGLE> ReadFileData = {};

	//파일 입출력 열어서 저장
	ios_base::openmode flag;
	flag = ios::in;
	ifstream file(szFilePath, flag);

	if (file.is_open())
	{
		file >> NaviMeshTriCount;
		_float3 Point[ENUM_CLASS(NAVI_POINT::END)] = {};
		for (_uInt i = 0; i < NaviMeshTriCount;)
		{
			file >> Point[ENUM_CLASS(NAVI_POINT::A)].x >> Point[ENUM_CLASS(NAVI_POINT::A)].y >> Point[ENUM_CLASS(NAVI_POINT::A)].z;
			++i;
			file >> Point[ENUM_CLASS(NAVI_POINT::B)].x >> Point[ENUM_CLASS(NAVI_POINT::B)].y >> Point[ENUM_CLASS(NAVI_POINT::B)].z;
			++i;
			file >> Point[ENUM_CLASS(NAVI_POINT::C)].x >> Point[ENUM_CLASS(NAVI_POINT::C)].y >> Point[ENUM_CLASS(NAVI_POINT::C)].z;
			++i;

			_bool bIsAdd = true;
			for (auto iter : ReadFileData)
			{
				Navi_Triangle NewTriangle = Navi_Triangle(Point[0], Point[1] , Point[2]);
				if (iter == NewTriangle)
				{
					bIsAdd = false;
					break;
				}
			}

			if(bIsAdd)
				ReadFileData.emplace_back(Point[ENUM_CLASS(NAVI_POINT::A)], Point[ENUM_CLASS(NAVI_POINT::B)], Point[ENUM_CLASS(NAVI_POINT::C)]);
		}
	}
	else
		return E_FAIL;
	file.close();


	for (auto& iter : ReadFileData)
	{
		_float3 vPoints[ENUM_CLASS(NAVI_POINT::END)] = {};
		_vector PointA = XMLoadFloat3(&iter.A);
		_vector PointB = XMLoadFloat3(&iter.B);
		_vector PointC = XMLoadFloat3(&iter.C);
		_vector vCross = XMVector3Cross(PointB - PointA, PointC - PointA);

		if (XMVector3Equal(PointA, PointB) || XMVector3Equal(PointA, PointC) || XMVector3Equal(PointB, PointC))
			continue;

		if (0 < XMVectorGetY(vCross))
		{
			vPoints[0] = iter.A;
			vPoints[1] = iter.B;
			vPoints[2] = iter.C;
		}
		else
		{
			vPoints[0] = iter.A;
			vPoints[1] = iter.C;
			vPoints[2] = iter.B;
		}

		CCell* pCell = CCell::Create(m_pDevice, m_pContext, (_uInt)m_Cells.size(), 0, vPoints);
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

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

_float CNavigation::Triarea2DCross(const _float3 a, const _float3 b, const _float3 c)
{
	_vector LineA = { b.x - a.x, 0.f, b.z - a.z , 0.f};
	_vector LineB = { c.x - a.x, 0.f, c.z - a.z , 0.f };

	return XMVectorGetY(XMVector2Cross(LineA, LineB));
}

void CNavigation::SimpleFunnelAlgorithm(_vector vStartPoint, list<_float3>* PathList)
{
	//맨 처음에 Cell을 탐색해서 변을 집어넣는거 하면될듯
	vector<PORTAL_DESC> Portals = {};
	Portals.reserve(m_PathCells.size());

	for (auto FromToCell : m_PathCells)
	{
		const PORTAL_DESC* Desc = m_Cells[FromToCell.first]->GetPortal(FromToCell.second);
		if(Desc)
			Portals.push_back(*Desc);
	}

	_float3 Apex{}, Left{}, Right{};
	XMStoreFloat3(&Apex, vStartPoint);
	if (Portals.empty())
		return;

	if (0 > Triarea2DCross(Apex, Portals[0].vLeftPoint, Portals[0].vRightPoint))
	{
		Left = Portals[0].vLeftPoint;
		Right = Portals[0].vRightPoint;
	}
	else
	{
		Left = Portals[0].vRightPoint;
		Right = Portals[0].vLeftPoint;
	}

	for (_uInt j = 1; j < Portals.size(); ++j)
	{
		_float3 Left2{}, Right2{};

		// 상세조건
		// 이전 right 와 새로운 left가 보다 큰지 확인 실패하면 right도 새로운걸로 바꿔서 연산
		// 위 조건 실패시 경로 집어넣기
		// 
		// j가 실패조건이 나왔을때 j - 1은 다시탐색해야할 i 증가량
		// j가 실패조건이 나왔을때 Right Left의 중점을 기준으로 경로를 넣자
		// 이거 2D 평면으로 바꿧을때 Cross연산은 오른손좌표 기준으로 연산을 한다고 한다.
		// 때문에 왼손 좌표계에서는 Cross연산의 부호를 바꿔줘야 정삭적으로 동작함

		if (0 <= Triarea2DCross(Apex, Portals[j].vLeftPoint, Portals[j].vRightPoint))
		{
			Left2 = Portals[j].vLeftPoint;
			Right2 = Portals[j].vRightPoint;
		}
		else
		{
			Right2 = Portals[j].vLeftPoint;
			Left2 = Portals[j].vRightPoint;
		}

		if (0 >= Triarea2DCross(Apex, Left, Left2))
		{
			if (0 > Triarea2DCross(Apex, Right, Left2))
			{
				_float3 WorldPos = {};
				_vector EdgeCenter = XMVectorLerp(XMLoadFloat3(&Left), XMLoadFloat3(&Right), 0.5f);
				XMStoreFloat3(&Apex, EdgeCenter);
				XMStoreFloat3(&WorldPos, XMVector3TransformCoord(EdgeCenter, XMLoadFloat4x4(&m_WorldMatrix)));
				PathList->push_back(WorldPos);
				j--;
			}
			else
				Left = Left2;
		}

		if (0 <= Triarea2DCross(Apex, Right, Right2))
		{
			if (0 > Triarea2DCross(Apex, Left, Right2))
			{
				_float3 WorldPos = {};
				_vector EdgeCenter = XMVectorLerp(XMLoadFloat3(&Left), XMLoadFloat3(&Right), 0.5f);
				XMStoreFloat3(&Apex, EdgeCenter);
				XMStoreFloat3(&WorldPos, XMVector3TransformCoord(EdgeCenter, XMLoadFloat4x4(&m_WorldMatrix)));
				PathList->push_back(WorldPos);
				j--;
			}
			else
				Right = Right2;
		}
	}
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pNavigationDataFilePath)
{
	CNavigation* pNavigation = new CNavigation(pDevice, pContext);
	if (FAILED(pNavigation->Initialize_Prototype(pNavigationDataFilePath)))
	{
		Safe_Release(pNavigation);
		MSG_BOX("CREATE FAIL : NAVIGATION");
	}
	return pNavigation;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CModel* pModel)
{
	CNavigation* pNavigation = new CNavigation(pDevice, pContext);
	if (FAILED(pNavigation->Initialize_Prototype(pModel)))
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
