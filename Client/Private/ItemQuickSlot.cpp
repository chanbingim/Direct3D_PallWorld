#include "ItemQuickSlot.h"

CItemQuickSlot::CItemQuickSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CItemQuickSlot::CItemQuickSlot(const CItemQuickSlot& rhs) :
	CBackGround(rhs)
{
}

HRESULT CItemQuickSlot::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItemQuickSlot::Initialize(void* pArg)
{

	return S_OK;
}

void CItemQuickSlot::Update(_float fDeletaTime)
{
}

void CItemQuickSlot::Late_Update(_float fDeletaTime)
{
}

HRESULT CItemQuickSlot::Render()
{
	return E_NOTIMPL;
}

HRESULT CItemQuickSlot::ADD_Components()
{
	return E_NOTIMPL;
}

HRESULT CItemQuickSlot::ADD_Childs()
{
	return E_NOTIMPL;
}

CItemQuickSlot* CItemQuickSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CItemQuickSlot::Clone(void* pArg)
{
	return nullptr;
}

void CItemQuickSlot::Free()
{
	__super::Free();
}
