#include "Hedgehog.h"

#include "GameInstance.h"

CHedgehog::CHedgehog(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	CPellBase(pGraphic_Device, pDeviceContext)
{
	m_PellID = 5;
}

CHedgehog::CHedgehog(const CHedgehog& rhs) :
	CPellBase(rhs)
{
}

HRESULT CHedgehog::Initalize_Prototype()
{

	return S_OK;
}

HRESULT CHedgehog::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CHedgehog::Priority_Update(_float fDeletaTime)
{
	__super::Priority_Update(fDeletaTime);
}

void CHedgehog::Update(_float fDeletaTime)
{
	__super::Update(fDeletaTime);
}

void CHedgehog::Late_Update(_float fDeletaTime)
{
	__super::Late_Update(fDeletaTime);
}

HRESULT CHedgehog::Render()
{

	return S_OK;
}

HRESULT CHedgehog::ADD_PartObjects()
{
	if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Herorong_Body_Default"), TEXT("Part_Body"))))
		return E_FAIL;
	return S_OK;
}

CHedgehog* CHedgehog::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
	CHedgehog* pHedgeHog = new CHedgehog(pGraphic_Device, pDeviceContext);
	if (FAILED(pHedgeHog->Initalize_Prototype()))
	{
		Safe_Release(pHedgeHog);
		MSG_BOX("CREATE FAIL : HEDGE HOG");
	}
	return pHedgeHog;
}

CGameObject* CHedgehog::Clone(void* pArg)
{
	CHedgehog* pHedgeHog = new CHedgehog(*this);
	if (FAILED(pHedgeHog->Initialize(pArg)))
	{
		Safe_Release(pHedgeHog);
		MSG_BOX("CLONE FAIL : HEDGE HOG");
	}
	return pHedgeHog;
}

void CHedgehog::Free()
{
	__super::Free();
}
