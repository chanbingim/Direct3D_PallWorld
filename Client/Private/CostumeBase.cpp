#include "CostumeBase.h"

CCostumeBase::CCostumeBase(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	CItemBase(pGraphic_Device, pDeviceContext)
{
}

CCostumeBase::CCostumeBase(const CCostumeBase& rhs) :
	CItemBase(rhs)
{
}

HRESULT CCostumeBase::Initalize_Prototype(void* InitDesc)
{
	if (FAILED(__super::Initalize_Prototype(InitDesc)))
		return E_FAIL;

	INIT_COSTUME_DESC* Desc = static_cast<INIT_COSTUME_DESC*>(InitDesc);
	m_CostumType = Desc->eCostumeType;

	return S_OK;
}

HRESULT CCostumeBase::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCostumeBase::Priority_Update(_float fDeletaTime)
{
}

void CCostumeBase::Update(_float fDeletaTime)
{
}

void CCostumeBase::Late_Update(_float fDeletaTime)
{
}

HRESULT CCostumeBase::Render()
{
	return S_OK;
}

HRESULT CCostumeBase::Bind_ShaderResources()
{
	return S_OK;
}

HRESULT CCostumeBase::Apply_ConstantShaderResources()
{
	return S_OK;
}

HRESULT CCostumeBase::ADD_Components()
{
	return S_OK;
}

CGameObject* CCostumeBase::Clone(void* pArg)
{
	CCostumeBase* pCostume = new CCostumeBase(*this);
	if(FAILED(pCostume->Initialize(pArg)))
	{
		Safe_Release(pCostume);
		MSG_BOX("CLONE FAIL : COSTUME");
	}
	return nullptr;
}

void CCostumeBase::Free()
{
	__super::Free();
}
