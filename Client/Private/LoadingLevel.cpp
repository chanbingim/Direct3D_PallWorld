#include "LoadingLevel.h"

#include "GameInstance.h"
#include "Loader.h"

#include "LogoLevel.h"
#include "GamePlayLevel.h"

CLoadingLevel::CLoadingLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID) :
    CLevel(pDevice, pContext, _iID)
{
}

HRESULT CLoadingLevel::Initialize(LEVEL eNextLevel)
{
    m_eNextLevelID = eNextLevel;

    /* 다음 레벨에 대한 자원을 로드하여 준비해둔다. */
    m_pLoader = CLoader::Create(m_pGraphic_Device, m_pDeviceContext, eNextLevel);
    if (nullptr == m_pLoader)
        return E_FAIL;

    return S_OK;
}

void CLoadingLevel::Update(_float fTimeDelta)
{
    if (true == m_pLoader->isFinished())
    {
        CLevel* pNewLevel = { nullptr };
        switch (m_eNextLevelID)
        {
        case LEVEL::LOGO:
            pNewLevel = CLogoLevel::Create(m_pGraphic_Device, m_pDeviceContext, m_eNextLevelID);
            break;
        case LEVEL::GAMEPLAY:
            pNewLevel = CGamePlayLevel::Create(m_pGraphic_Device, m_pDeviceContext, m_eNextLevelID);
            break;
        }

        if (FAILED(m_pGameInstance->Change_Level(pNewLevel)))
            return;
    }
}

HRESULT CLoadingLevel::Render()
{
    m_pLoader->Output();
    return S_OK;
}

_bool CLoadingLevel::IsFinishedLoader()
{
    if (nullptr == m_pLoader)
        return false;

    return m_pLoader->isFinished();
}

CLoadingLevel* CLoadingLevel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eID, LEVEL eNextLevel)
{
    CLoadingLevel* pLoadingLevel = new CLoadingLevel(pDevice, pContext, ENUM_CLASS(eID));
    if (FAILED(pLoadingLevel->Initialize(eNextLevel)))
    {
        Safe_Release(pLoadingLevel);
        MSG_BOX("CREATE FAIL : LOADING LEVEL");
    }
    return pLoadingLevel;
}

void CLoadingLevel::Free()
{
    __super::Free();

    Safe_Release(m_pLoader);
}
