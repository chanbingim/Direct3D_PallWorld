#include "Level.h"

#include "GameInstance.h"
#include "HeadUpDisplay.h"

CLevel::CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID) :
m_pGraphic_Device(pDevice),
m_pDeviceContext(pContext),
m_ID(_iID),
m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pDeviceContext);
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CLevel::Initialize()
{
      
    return S_OK;
}

void CLevel::Update(_float fTimeDelta)
{
    if(m_pHUD)
        m_pHUD->Update(fTimeDelta);
}

HRESULT CLevel::Render()
{
    return S_OK;
}

void CLevel::SetHUD(CHeadUpDisplay* pHUD)
{
    m_pHUD = pHUD;
}

void CLevel::Free()
{
    Safe_Release(m_pGameInstance);
    Safe_Release(m_pDeviceContext);
    Safe_Release(m_pGraphic_Device);

    Safe_Release(m_pHUD);
}
