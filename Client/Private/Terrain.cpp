#include "Terrain.h"

#include "GameInstance.h"
#include "PaseDataHeader.h"
#include "StringHelper.h"

CTerrain::CTerrain(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	CActor(pGraphic_Device, pDeviceContext)
{

}

CTerrain::CTerrain(const CTerrain& rhs) :
	CActor(rhs)
{
}

HRESULT CTerrain::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	_uInt TileCnt = 0;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	TERRIAN_DESC* Desc = static_cast<TERRIAN_DESC*>(pArg);
	if (Desc)
	{
		TileCnt = Desc->iGridCnt;
		m_bIsPicking = Desc->IsPicking;
		if (FAILED(ADD_Components(*Desc)))
			return E_FAIL;
	}
	else
		m_ObejctTag = TEXT("Terrian");

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	
	return S_OK;
}

void CTerrain::Priority_Update(_float fDeletaTime)
{
	__super::Priority_Update(fDeletaTime);
}

void CTerrain::Update(_float fDeletaTime)
{
	__super::Update(fDeletaTime);
	m_pNavigationCom->Update(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));

	_float3 vOut = {};
#ifdef _DEBUG
	if (m_bIsPicking)
	{
		if (m_pGameInstance->KeyDown(KEY_INPUT::MOUSE, 0) ||
			GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			m_pVIBufferCom->UpdateHegiht(m_pTransformCom, 1.f, 2.f);
		}

		if (m_pGameInstance->KeyDown(KEY_INPUT::MOUSE, 1) ||
			GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			m_pVIBufferCom->UpdateHegiht(m_pTransformCom, -1.f, 2.f);
		}
	}
#endif // _DEBUG
}

void CTerrain::Late_Update(_float fDeletaTime)
{
	__super::Late_Update(fDeletaTime);
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	//Apply_ConstantShaderResources();

	//m_pShaderCom->Update_Shader(0);
	//m_pTextureCom->SetTexture(0, 0);

	//m_pVIBufferCom->Render_VIBuffer();

	_float4 vColor = { 0.f, 1.f, 0.f, 1.f };
	m_pNavigationCom->Render(vColor);
	return S_OK;
}

_bool CTerrain::IsMoveTerrian(_vector vPosition)
{
	return m_pNavigationCom->IsMove(vPosition);
}

#ifdef _DEBUG
void CTerrain::ExportData(void* pArg)
{
	__super::ExportData(pArg);
	SAVE_LEVEL_DESC* Desc = static_cast<SAVE_LEVEL_DESC*>(pArg);
	Desc->ObjectDesc.TerrianDesc.TerrainType = 0;
	Desc->ObjectDesc.TerrianDesc.TileCnt = m_pVIBufferCom->GetTerrianSize();
	CStringHelper::ConvertWideToUTF(szVIBuffer, Desc->ObjectDesc.TerrianDesc.HeightMapCom);
	CStringHelper::ConvertWideToUTF(L"", Desc->ObjectDesc.TerrianDesc.NaviMeshPath);
}

void CTerrain::ExportNaivMeshData(void* pArg)
{

}
#endif // _DEBUG

HRESULT CTerrain::Apply_ConstantShaderResources()
{
	m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
	m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
	m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

	return S_OK;
}

HRESULT CTerrain::ADD_Components(const TERRIAN_DESC& Desc)
{
	if (Desc.iGridCnt > 0)
	{
		wsprintf(szVIBuffer, TEXT("Prototype_Component_VIBuffer_Terrian%dx%d"), Desc.iGridCnt, Desc.iGridCnt);

		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), szVIBuffer, TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
	}
	else
	{
		wsprintf(szVIBuffer, Desc.HeightMap);
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), Desc.HeightMap, TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
	}

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Terrian"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (!lstrcmp(Desc.NavigationData, L""))
	{
		CVIBuffer_Terrain* pVIBuffer_Terrian = dynamic_cast<CVIBuffer_Terrain*>(m_pVIBufferCom);
		if(pVIBuffer_Terrian)
			m_pNavigationCom = CNavigation::Create(m_pGraphic_Device, m_pDeviceContext, pVIBuffer_Terrian);
	}
	else
		m_pNavigationCom = CNavigation::Create(m_pGraphic_Device, m_pDeviceContext, Desc.NavigationData);
	m_pComponentMap.emplace(TEXT("NaviMesh_Com"), m_pNavigationCom);
	

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VTXNorTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Bind_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
	m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
	m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();

	return S_OK;
}

CTerrain* CTerrain::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
	CTerrain* pTerrian = new CTerrain(pGraphic_Device, pDeviceContext);
	if (FAILED(pTerrian->Initalize_Prototype()))
	{
		Safe_Release(pTerrian);
		MSG_BOX("CREATE FAIL : TERRAIN");
	}
	return pTerrian;
}

CGameObject* CTerrain::Clone(void* pArg)
{
	CTerrain* pTerrian = new CTerrain(*this);
	if (FAILED(pTerrian->Initialize(pArg)))
	{
		Safe_Release(pTerrian);
		MSG_BOX("CLONE FAIL : TERRAIN");
	}
	return pTerrian;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
