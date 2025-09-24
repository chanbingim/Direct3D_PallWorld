#include "WeaponQuickSlot.h"

#include "GameInstance.h"

CWeaponQuickSlot::CWeaponQuickSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CWeaponQuickSlot::CWeaponQuickSlot(const CWeaponQuickSlot& rhs) :
	CBackGround(rhs)
{
}

HRESULT CWeaponQuickSlot::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeaponQuickSlot::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(ADD_Childs()))
		return E_FAIL;

	return S_OK;
}

void CWeaponQuickSlot::Update(_float fDeletaTime)
{
}

void CWeaponQuickSlot::Late_Update(_float fDeletaTime)
{
}

HRESULT CWeaponQuickSlot::Render()
{
	//이제 전부 지오메트리로 그릴거임
	Bind_ShaderResources();
	m_pShaderCom->Update_Shader(0);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

HRESULT CWeaponQuickSlot::Bind_ShaderResources()
{
	return S_OK;
}

HRESULT CWeaponQuickSlot::ADD_Components()
{
	return S_OK;
}

HRESULT CWeaponQuickSlot::ADD_Childs()
{
	return S_OK;
}

CWeaponQuickSlot* CWeaponQuickSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeaponQuickSlot* pWeaponquickSlot = new CWeaponQuickSlot(pDevice, pContext);
	if (FAILED(pWeaponquickSlot->Initalize_Prototype()))
	{
		Safe_Release(pWeaponquickSlot);
		MSG_BOX("CREATE FAIL : WEAPON QUICK SLOT");
	}

	return pWeaponquickSlot;
}

CGameObject* CWeaponQuickSlot::Clone(void* pArg)
{
	CGameObject* pWeaponquickSlot = new CWeaponQuickSlot(*this);
	if (FAILED(pWeaponquickSlot->Initialize(pArg)))
	{
		Safe_Release(pWeaponquickSlot);
		MSG_BOX("CLONE FAIL : WEAPON QUICK SLOT");
	}

	return pWeaponquickSlot;
}

void CWeaponQuickSlot::Free()
{
	__super::Free();

	Safe_Release(m_pWeaponSlotIcon);
	Safe_Release(m_pWeaponSlotName);
	Safe_Release(m_pWeaponSlotCount);
}
