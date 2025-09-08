#include "YetiBody.h"

#include "GameInstance.h"
CYetiBody::CYetiBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CPellBody(pDevice, pContext)
{
}

CYetiBody::CYetiBody(const CYetiBody& rhs) :
	CPellBody(rhs)
{
}

HRESULT CYetiBody::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CYetiBody::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

void CYetiBody::Priority_Update(_float fDeletaTime)
{
}

void CYetiBody::Update(_float fDeletaTime)
{
	m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime);
}

void CYetiBody::Late_Update(_float fDeletaTime)
{
	__super::Late_Update(fDeletaTime);
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CYetiBody::Render()
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

HRESULT CYetiBody::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Yeti_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CYetiBody* CYetiBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CYetiBody* pYetiBody = new CYetiBody(pDevice, pContext);
	if (FAILED(pYetiBody->Initalize_Prototype()))
	{
		Safe_Release(pYetiBody);
		MSG_BOX("CREATE FAIL : Yeti Body");
	}
	return pYetiBody;
}

CGameObject* CYetiBody::Clone(void* pArg)
{
	CYetiBody* pYetiBody = new CYetiBody(*this);
	if (FAILED(pYetiBody->Initialize(pArg)))
	{
		Safe_Release(pYetiBody);
		MSG_BOX("CLONE FAIL : Yeti Body");
	}
	return pYetiBody;
}

void CYetiBody::Free()
{
	__super::Free();
}