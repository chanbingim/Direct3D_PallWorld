#include "PalSpher.h"

#include "GameInstance.h"

CPalSpher::CPalSpher(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CPartObject(pDevice, pContext)
{
}

CPalSpher::CPalSpher(const CPalSpher& rhs) :
	CPartObject(rhs)
{
}

HRESULT CPalSpher::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CPalSpher::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (m_ObejctTag.c_str() == L"")
        m_ObejctTag = TEXT("Pal Spher");

    return S_OK;
}

void CPalSpher::Priority_Update(_float fDeletaTime)
{
}

void CPalSpher::Update(_float fDeletaTime)
{
	m_pVIBufferCom->PlayAnimation(0, 0, fDeletaTime);
}

void CPalSpher::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPalSpher::Render()
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

HRESULT CPalSpher::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Yeti_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CPalSpher* CPalSpher::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPalSpher* pPalSpher = new CPalSpher(pDevice, pContext);
	if (FAILED(pPalSpher->Initalize_Prototype()))
	{
		Safe_Release(pPalSpher);
		MSG_BOX("CREATE FAIL : Pal Spher");
	}
	return pPalSpher;
}

CGameObject* CPalSpher::Clone(void* pArg)
{
	CPalSpher* pPalSpher = new CPalSpher(*this);
	if (FAILED(pPalSpher->Initialize(pArg)))
	{
		Safe_Release(pPalSpher);
		MSG_BOX("CLONE FAIL : Pal Spher");
	}
	return pPalSpher;
}

void CPalSpher::Free()
{
	__super::Free();
}
