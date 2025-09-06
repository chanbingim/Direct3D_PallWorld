#include "PlayerWeapon.h"

#include "GameInstance.h"

CPlayerWeapon::CPlayerWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CPartObject(pDevice, pContext)
{
}

CPlayerWeapon::CPlayerWeapon(const CPlayerWeapon& rhs) :
	CPartObject(rhs)
{
}

HRESULT CPlayerWeapon::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerWeapon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

void CPlayerWeapon::Priority_Update(_float fDeletaTime)
{
}

void CPlayerWeapon::Update(_float fDeletaTime)
{
}

void CPlayerWeapon::Late_Update(_float fDeletaTime)
{
	UpdateCombinedMatrix();

	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPlayerWeapon::Render()
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

HRESULT CPlayerWeapon::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_CatBlade"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CPlayerWeapon* CPlayerWeapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerWeapon* pPlayerWeapon = new CPlayerWeapon(pDevice, pContext);
	if (FAILED(pPlayerWeapon->Initalize_Prototype()))
	{
		Safe_Release(pPlayerWeapon);
		MSG_BOX("CREATE FAIL : PLAYER WEAPON");
	}

	return pPlayerWeapon;
}

CGameObject* CPlayerWeapon::Clone(void* pArg)
{
	CPlayerWeapon* pPlayerWeapon = new CPlayerWeapon(*this);
	if (FAILED(pPlayerWeapon->Initialize(pArg)))
	{
		Safe_Release(pPlayerWeapon);
		MSG_BOX("CLONE FAIL : PLAYER WEAPON");
	}

	return pPlayerWeapon;
}

void CPlayerWeapon::Free()
{
	__super::Free();
}
