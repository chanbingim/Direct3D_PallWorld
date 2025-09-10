#include "Navigation.h"

#include "GameInstance.h"
#include "Cell.h"

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CComponent(pDevice, pContext)
{
}

CNavigation::CNavigation(const CNavigation& rhs) : 
	CComponent(rhs)
{
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

HRESULT CNavigation::Initialize(void* pArg)
{

	
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
	{
		if (-1 == iNeighborIndex)
			return false;
		else
		{
			if(m_Cells[iNeighborIndex]->IsMoveAble())
				return true;
		}
	}

	return false;
}

HRESULT CNavigation::Render()
{
#ifdef _DEBUG
	ApplyShaderReSource();
#endif // _DEBUG
	
	for (auto& iter : m_Cells)
		iter->Render();

	return S_OK;
}

void CNavigation::BindSahderResource()
{
	m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
	m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
	m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
}

void CNavigation::ApplyShaderReSource()
{
	m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_WorldMatrix));
	m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
	m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));
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
