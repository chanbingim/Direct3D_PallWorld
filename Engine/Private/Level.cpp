#include "Level.h"

#include "GameInstance.h"
#include "HeadUpDisplay.h"

CLevel::CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID, CHeadUpDisplay* pHUD) :
m_pGraphic_Device(pDevice),
m_pDeviceContext(pContext),
m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pDeviceContext);
    Safe_AddRef(m_pGraphic_Device);

    if (pHUD)
        m_pHUD = pHUD;
}

HRESULT CLevel::Initialize()
{
    if (m_pHUD)
    {
        if (FAILED(m_pHUD->Initialize()))
            return E_FAIL;
    }
      
    return S_OK;
}

void CLevel::Update(_float fTimeDelta)
{
    m_pHUD->Update(fTimeDelta);
}

HRESULT CLevel::Render()
{
    return S_OK;
}

void CLevel::Free()
{
    Safe_Release(m_pGameInstance);
    Safe_Release(m_pDeviceContext);
    Safe_Release(m_pGraphic_Device);
}
