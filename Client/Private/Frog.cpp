#include "Frog.h"

#include "GameInstance.h"

CFrog::CFrog(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	CPellBase(pGraphic_Device, pDeviceContext)
{
	m_PellID = 4;
}

CFrog::CFrog(const CFrog& rhs) :
	CPellBase(rhs)
{
}

HRESULT CFrog::Initalize_Prototype()
{

	return S_OK;
}

HRESULT CFrog::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_PartObjects()))
		return E_FAIL;


	return S_OK;
}

void CFrog::Priority_Update(_float fDeletaTime)
{
	__super::Priority_Update(fDeletaTime);
}

void CFrog::Update(_float fDeletaTime)
{
	__super::Update(fDeletaTime);
}

void CFrog::Late_Update(_float fDeletaTime)
{
	__super::Late_Update(fDeletaTime);
}

HRESULT CFrog::Render()
{


	return S_OK;
}

HRESULT CFrog::ADD_PartObjects()
{
	if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Frog_Body_Default"), TEXT("Part_Body"))))
		return E_FAIL;
	return S_OK;
}

CFrog* CFrog::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
	CFrog* pFrog = new CFrog(pGraphic_Device, pDeviceContext);
	if (FAILED(pFrog->Initalize_Prototype()))
	{
		Safe_Release(pFrog);
		MSG_BOX("CREATE FAIL : Frog");
	}
	return pFrog;
}

CGameObject* CFrog::Clone(void* pArg)
{
	CFrog* pFrog = new CFrog(*this);
	if (FAILED(pFrog->Initialize(pArg)))
	{
		Safe_Release(pFrog);
		MSG_BOX("CLONE FAIL : Frog");
	}
	return pFrog;
}

void CFrog::Free()
{
	__super::Free();
}
