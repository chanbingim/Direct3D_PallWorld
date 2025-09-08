#include "DororongBody.h"

#include "GameInstance.h"
CDororongBody::CDororongBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CPellBody(pDevice, pContext)
{
}

CDororongBody::CDororongBody(const CDororongBody& rhs) : 
	CPellBody(rhs)
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

	return S_OK;
}

void CDororongBody::Priority_Update(_float fDeletaTime)
{
}

void CDororongBody::Update(_float fDeletaTime)
{
	m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime);
}

void CDororongBody::Late_Update(_float fDeletaTime)
{
	__super::Late_Update(fDeletaTime);
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CDororongBody::Render()
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

HRESULT CDororongBody::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Drorong_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
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
}