#include "DororongBody.h"

#include "GameInstance.h"
CDororongBody::CDororongBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CPellBody(pDevice, pContext)
{
}

CDororongBody::CDororongBody(const CDororongBody& rhs) : 
	CPellBody(rhs),
	m_pTrailSocket(rhs.m_pTrailSocket)
{
}

HRESULT CDororongBody::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CDororongBody::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pTrailSocket = m_pVIBufferCom->GetCombinedTransformationMatrixPtr("spine");
	m_pNoiseTexture = m_pGameInstance->GetTextureResource(TEXT("T_Default_Noise.png"));
	return S_OK;
}

void CDororongBody::Priority_Update(_float fDeletaTime)
{
}

void CDororongBody::Update(_float fDeletaTime)
{
	m_bIsAnimFinished = m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime, 10.f , m_bIsAnimLoop);
	m_pTrail->Update_Trail(XMLoadFloat4x4(m_pTrailSocket) * XMLoadFloat4x4(&m_CombinedWorldMatrix), true);
}

void CDororongBody::Late_Update(_float fDeletaTime)
{
	UpdateCombinedMatrix();
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CDororongBody::Render()
{
	_uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();

	for (_uInt i = 0; i < iNumMeshes; ++i)
	{
		Apply_ConstantShaderResources(i);
		m_pShaderCom->Bind_RawValue("g_bIsDissolve", &m_bIsDissolve, sizeof(_bool));
		if (m_bIsDissolve)
		{
			
			m_pShaderCom->Bind_SRV("g_DissolveTexture", m_pNoiseTexture->GetTexture(0));
			m_pShaderCom->Bind_RawValue("g_fDissloveTime", &m_fAccDissolveTime, sizeof(_float));
		}

		m_pShaderCom->Update_Shader(0);
		m_pVIBufferCom->Render(i);
	}

	Apply_TrailShaderResource();
	m_pTrailShader->Update_Shader(0);
	m_pTrailTex->SetTexture(0, 0);
	m_pTrail->Render();

	return S_OK;
}

HRESULT CDororongBody::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Drorong_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Trail_VtxTex"), TEXT("TrailShader"), (CComponent**)&m_pTrailShader)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Trail_Defalut"), TEXT("Trail_Com"), (CComponent**)&m_pTrail)))
		return E_FAIL;

	// 여기서 텍스처도 가져오자.
	m_pTrailTex = m_pGameInstance->GetTextureResource(TEXT("T_Trail73.png"));
	if (nullptr == m_pTrailTex)
		return E_FAIL;

	return S_OK;
}

void CDororongBody::Apply_TrailShaderResource()
{
	_float4x4 IdentityMat = {};
	XMStoreFloat4x4(&IdentityMat, XMMatrixIdentity());
	m_pTrailShader->Bind_Matrix("g_WorldMatrix", &IdentityMat);
	m_pTrailShader->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->GetMatrix(MAT_STATE::VIEW));
	m_pTrailShader->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION));

	m_pTrailShader->Bind_RawValue("g_vColor", &m_vTrailColor, sizeof(_float4));
}

CDororongBody* CDororongBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDororongBody* pDrorongBody = new CDororongBody(pDevice, pContext);
	if (FAILED(pDrorongBody->Initalize_Prototype()))
	{
		Safe_Release(pDrorongBody);
		MSG_BOX("CREATE FAIL : DORORONG BODY");
	}
	return pDrorongBody;
}

CGameObject* CDororongBody::Clone(void* pArg)
{
	CDororongBody* pDrorongBody = new CDororongBody(*this);
	if (FAILED(pDrorongBody->Initialize(pArg)))
	{
		Safe_Release(pDrorongBody);
		MSG_BOX("CLONE FAIL : DORORONG BODY");
	}
	return pDrorongBody;
}

void CDororongBody::Free()
{
	__super::Free();

	Safe_Release(m_pTrail);
	Safe_Release(m_pTrailTex);
	Safe_Release(m_pTrailShader);
}