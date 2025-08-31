#include "Terrian.h"

#include "GameInstance.h"
#include "PaseDataHeader.h"
#include "StringHelper.h"

CTerrian::CTerrian(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	CActor(pGraphic_Device, pDeviceContext)
{

}

CTerrian::CTerrian(const CTerrian& rhs) :
	CActor(rhs)
{
}

HRESULT CTerrian::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrian::Initialize(void* pArg)
{
	_uInt TileCnt = 0;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	TERRIAN_DESC* Desc = static_cast<TERRIAN_DESC*>(pArg);
	if (Desc)
	{
		TileCnt = Desc->iGridCnt;
		m_bIsPicking = Desc->IsPicking;
	}
	else
		m_ObejctTag = TEXT("Terrian");

	if (FAILED(ADD_Components(TileCnt)))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	
	return S_OK;
}

void CTerrian::Priority_Update(_float fDeletaTime)
{
	__super::Priority_Update(fDeletaTime);
}

void CTerrian::Update(_float fDeletaTime)
{
	__super::Update(fDeletaTime);

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

void CTerrian::Late_Update(_float fDeletaTime)
{
	__super::Late_Update(fDeletaTime);
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CTerrian::Render()
{
	Apply_ConstantShaderResources();

	m_pShaderCom->Update_Shader(0);
	m_pTextureCom->SetTexture(0, 0);

	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

#ifdef _DEBUG
void CTerrian::ExportData(void* pArg)
{
	__super::ExportData(pArg);
	SAVE_LEVEL_DESC* Desc = static_cast<SAVE_LEVEL_DESC*>(pArg);
	Desc->ObjectDesc.TerrianDesc.TerrainType = 0;
	Desc->ObjectDesc.TerrianDesc.TileCnt = m_pVIBufferCom->GetTerrianSize();
	CStringHelper::ConvertWideToUTF(szVIBuffer, Desc->ObjectDesc.TerrianDesc.HeightMapCom);
}
#endif // _DEBUG

HRESULT CTerrian::Apply_ConstantShaderResources()
{
	m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
	m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
	m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

	return S_OK;
}

HRESULT CTerrian::ADD_Components(_uInt iGridCnt)
{
	if (iGridCnt > 0)
	{
		wsprintf(szVIBuffer, TEXT("Prototype_Component_VIBuffer_Terrian%dx%d"), iGridCnt, iGridCnt);

		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), szVIBuffer, TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
	}
	else
	{
		wsprintf(szVIBuffer, TEXT("Prototype_Component_VIBuffer_Terrian"));
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrian"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
	}

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Terrian"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VTXNorTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrian::Bind_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
	m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
	m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();

	return S_OK;
}

CTerrian* CTerrian::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
	CTerrian* pTerrian = new CTerrian(pGraphic_Device, pDeviceContext);
	if (FAILED(pTerrian->Initalize_Prototype()))
	{
		Safe_Release(pTerrian);
		MSG_BOX("CREATE FAIL : TERRAIN");
	}
	return pTerrian;
}

CGameObject* CTerrian::Clone(void* pArg)
{
	CTerrian* pTerrian = new CTerrian(*this);
	if (FAILED(pTerrian->Initialize(pArg)))
	{
		Safe_Release(pTerrian);
		MSG_BOX("CLONE FAIL : TERRAIN");
	}
	return pTerrian;
}

void CTerrian::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
