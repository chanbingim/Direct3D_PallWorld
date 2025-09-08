#include "HerorongBody.h"

#include "GameInstance.h"
CHerorongBody::CHerorongBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CPellBody(pDevice, pContext)
{
}

CHerorongBody::CHerorongBody(const CHerorongBody& rhs) :
	CPellBody(rhs)
{
}

HRESULT CHerorongBody::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CHerorongBody::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

void CHerorongBody::Priority_Update(_float fDeletaTime)
{
}

void CHerorongBody::Update(_float fDeletaTime)
{
	m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime);
}

void CHerorongBody::Late_Update(_float fDeletaTime)
{
	__super::Late_Update(fDeletaTime);
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CHerorongBody::Render()
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

HRESULT CHerorongBody::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Herorong_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CHerorongBody* CHerorongBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHerorongBody* pHerorongBody = new CHerorongBody(pDevice, pContext);
	if (FAILED(pHerorongBody->Initalize_Prototype()))
	{
		Safe_Release(pHerorongBody);
		MSG_BOX("CREATE FAIL : HERORONG BODY");
	}
	return pHerorongBody;
}

CGameObject* CHerorongBody::Clone(void* pArg)
{
	CHerorongBody* pHerorongBody = new CHerorongBody(*this);
	if (FAILED(pHerorongBody->Initialize(pArg)))
	{
		Safe_Release(pHerorongBody);
		MSG_BOX("CLONE FAIL : HERORONG BODY");
	}
	return pHerorongBody;
}

void CHerorongBody::Free()
{
	__super::Free();
}