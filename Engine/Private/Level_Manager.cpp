#include "Level_Manager.h"

#include "GameInstance.h"
#include "Level.h"

CLevel_Manager::CLevel_Manager() :
    m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CLevel_Manager::Initialize()
{
    return S_OK;
}

HRESULT CLevel_Manager::Change_Level(CLevel* pNewLevel)
{
    //나중에 레벨 있으면 초기화 없으면 만들기로 바꿀거임
    if (nullptr != m_pCurrentLevel)
    {
        m_pGameInstance->Clear_Resource(m_pCurrentLevel->GetLevelID());
    
        Safe_Release(m_pCurrentLevel);
    }

    m_pCurrentLevel = pNewLevel;

    return S_OK;
}

void CLevel_Manager::Update(_float fTimeDelta)
{
    if (nullptr == m_pCurrentLevel)
        return;

    m_pCurrentLevel->Update(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
    if (nullptr == m_pCurrentLevel)
        return E_FAIL;

    m_pCurrentLevel->Render();

    return S_OK;
}

CLevel* CLevel_Manager::GetCurrentLevel()
{
    return m_pCurrentLevel;
}

CLevel_Manager* CLevel_Manager::Create()
{
    CLevel_Manager* pLevelManager = new CLevel_Manager();
    if (FAILED(pLevelManager->Initialize()))
    {
        Safe_Release(pLevelManager);
        MSG_BOX("CREATE FAIL : LEVEL MANAGER");
    }
    return pLevelManager;
}

void CLevel_Manager::Free()
{
    Safe_Release(m_pGameInstance);
    Safe_Release(m_pCurrentLevel);
}
