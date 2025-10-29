#include "SkyBox.h"

#include "GameInstance.h"
#include "SunLight.h"

CSkyBox::CSkyBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CNoneAnimMesh(pDevice, pContext)
{
}

CSkyBox::CSkyBox(const CSkyBox& rhs) :
	CNoneAnimMesh(rhs)
{
}

HRESULT CSkyBox::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CSkyBox::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	m_pSunLight = static_cast<CSunLight*>(m_pGameInstance->GetAllObejctToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("GamePlay_Layer_Dir_Light"))->front());
	m_pTransformCom->SetScale({ 2.f, 2.f, 2.f });
	return S_OK;
}

void CSkyBox::Priority_Update(_float fDeletaTime)
{
	_float3 vCameraPos = {};
	XMStoreFloat3(&vCameraPos, m_pGameInstance->GetCameraState(WORLDSTATE::POSITION));
	m_pTransformCom->SetPosition(vCameraPos);
}

void CSkyBox::Update(_float fDeletaTime)
{
	switch (m_pSunLight->GetDay())
	{
	case CSunLight::DAY_TYPE::MORNING :
	case CSunLight::DAY_TYPE::LUNCH:
		m_iSkyBoxIndex = 0;
		break;

	case CSunLight::DAY_TYPE::NIGHT:
	case CSunLight::DAY_TYPE::DAWN:
		m_iSkyBoxIndex = 1;
		break;
	}
}

void CSkyBox::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::PRIORITY, this);
}

HRESULT CSkyBox::Render()
{
	_uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
	for (_uInt i = 0; i < iNumMeshes; ++i)
	{
		Apply_ConstantShaderResources(i);

		m_pShaderCom->Update_Shader(0);
		m_pVIBufferCom->Render(i);
	}

	if (0 == m_iSkyBoxIndex)
	{
		for (_uInt i = 0; i < iNumMeshes; ++i)
		{
			m_pShaderCom->Update_Shader(0);
			m_pVIBufferCom->Render(i);
		}
	}
	



	return S_OK;
}

HRESULT CSkyBox::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->GetWorldMat());
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->GetMatrix(MAT_STATE::VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION));

	m_pShaderCom->Bind_SRV("g_DiffuseTexture", m_pBaseTextureCom->GetTexture(m_iSkyBoxIndex));
	m_pShaderCom->Bind_SRV("g_CloudsMaskTexture", m_pCloudsMaskTextureCom->GetTexture(0));

	return S_OK;
}

HRESULT CSkyBox::ADD_Components()
{
	// µ¹  ¸ðµ¨ Á¤º¸
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Sky"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// Shader
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Sky_Box_Shader"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// BaseTexture
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Sky_Mesh_BackGround"), TEXT("BaseTex_Com"), (CComponent**)&m_pBaseTextureCom)))
		return E_FAIL;

	// Clouds Mask Texture
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Sky_Mesh_Clouds_M"), TEXT("Cloud_MaskTex_Com"), (CComponent**)&m_pCloudsMaskTextureCom)))
		return E_FAIL;

	return S_OK;
}

CSkyBox* CSkyBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkyBox* pSky = new CSkyBox(pDevice, pContext);
	if (FAILED(pSky->Initalize_Prototype()))
	{
		Safe_Release(pSky);
		MSG_BOX("CREATE FAIL : SKY");
	}
	return pSky;
}

CGameObject* CSkyBox::Clone(void* pArg)
{
	CSkyBox* pSky = new CSkyBox(*this);
	if (FAILED(pSky->Initialize(pArg)))
	{
		Safe_Release(pSky);
		MSG_BOX("CLONE FAIL : SKY");
	}
	return pSky;
}

void CSkyBox::Free()
{
	__super::Free();

	Safe_Release(m_pBaseTextureCom);
	Safe_Release(m_pCloudsMaskTextureCom);
}
