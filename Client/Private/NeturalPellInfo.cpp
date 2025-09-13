#include "NeturalPellInfo.h"

CNeturalPellInfo::CNeturalPellInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CNeturalPellInfo::CNeturalPellInfo(const CNeturalPellInfo& rhs) :
	CBackGround(rhs)
{
}

HRESULT CNeturalPellInfo::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CNeturalPellInfo::Initialize(void* pArg)
{
	return S_OK;
}

void CNeturalPellInfo::Update(_float fDeletaTime)
{
}

void CNeturalPellInfo::Late_Update(_float fDeletaTime)
{
}

HRESULT CNeturalPellInfo::Render()
{
	return S_OK;
}

HRESULT CNeturalPellInfo::ADD_Components()
{
	return S_OK;
}

CNeturalPellInfo* CNeturalPellInfo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNeturalPellInfo* pNeturalPellInfo = new CNeturalPellInfo(pDevice, pContext);
	if (FAILED(pNeturalPellInfo->Initalize_Prototype()))
	{
		Safe_Release(pNeturalPellInfo);
		MSG_BOX("CREATE FAIL : NETURAL PELL INFO");
	}
	return pNeturalPellInfo;
}

CGameObject* CNeturalPellInfo::Clone(void* pArg)
{
	CNeturalPellInfo* pNeturalPellInfo = new CNeturalPellInfo(*this);
	if (FAILED(pNeturalPellInfo->Initialize(pArg)))
	{
		Safe_Release(pNeturalPellInfo);
		MSG_BOX("CLONE FAIL : NETURAL PELL INFO");
	}
	return pNeturalPellInfo;
}

void CNeturalPellInfo::Free()
{
	__super::Free();

	Safe_Release(m_pHealthBar);
	Safe_Release(m_pTypeIcon);
}
