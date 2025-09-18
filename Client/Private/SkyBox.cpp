#include "SkyBox.h"

#include "GameInstance.h"

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

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

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

	return S_OK;
}

HRESULT CSkyBox::Bind_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
	m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
	m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
	m_pSRVEffect = m_pShaderCom->GetVariable("g_DiffuseTexture")->AsShaderResource();

	return S_OK;
}

HRESULT CSkyBox::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
	m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
	m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
	m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

	ID3D11ShaderResourceView* pResourceVeiw = {};
	m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
	if (pResourceVeiw)
		m_pSRVEffect->SetResource(pResourceVeiw);

	return S_OK;
}

HRESULT CSkyBox::ADD_Components()
{
	// µ¹  ¸ðµ¨ Á¤º¸
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Sky"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// NonAnimShader
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_SkyMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
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
}
