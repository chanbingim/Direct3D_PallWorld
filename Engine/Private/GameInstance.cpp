#include "GameInstance.h"

#include "Graphic_Device.h"

#pragma region Manager;
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Prototype_Manager.h"
#include "Timer_Manager.h"
#include "Renderer.h"
#include "Sound_Manager.h"
#include "InputManager.h"
#pragma endregion

IMPLEMENT_SINGLETON(CGameInstance);

HRESULT CGameInstance::Initialize_Engine(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    GAMEINSTANCE_DESC* GameSetting = static_cast<GAMEINSTANCE_DESC*>(pArg);

    m_pGraphic_Device = CGraphic_Device::Create(GameSetting->EngineSetting, GameSetting->ppDevice, GameSetting->ppContext);
    if (nullptr == m_pGraphic_Device)
        return E_FAIL;

    m_pTimer_Manager = CTimer_Manager::Create();
    if (nullptr == m_pTimer_Manager)
        return E_FAIL;

    m_pPrototype_Manager = CPrototype_Manager::Create(GameSetting->EngineSetting.iNumLevels);
    if (nullptr == m_pPrototype_Manager)
        return E_FAIL;

    m_pObject_Manager = CObject_Manager::Create(GameSetting->EngineSetting.iNumLevels);
    if (nullptr == m_pObject_Manager)
        return E_FAIL;

    m_pLevel_Manager = CLevel_Manager::Create();
    if (nullptr == m_pLevel_Manager)
        return E_FAIL;

    m_pRenderer = CRenderer::Create(*GameSetting->ppDevice, *GameSetting->ppContext);
    if (nullptr == m_pRenderer)
        return E_FAIL;

    m_pInput_Manager = CInputManager::Create(GameSetting->EngineSetting.Instance, GameSetting->EngineSetting.hWnd, GameSetting->EngineSetting.iWinSizeX, GameSetting->EngineSetting.iWinSizeY);
    if (nullptr == m_pInput_Manager)
        return E_FAIL;

    m_pSound_Manager = CSound_Manager::Create();
    if (nullptr == m_pSound_Manager)
        return E_FAIL;

    return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
    m_pInput_Manager->UpdateKeyFrame();

    m_pObject_Manager->Priority_Update(fTimeDelta);

    m_pObject_Manager->Update(fTimeDelta);
   
    m_pObject_Manager->Late_Update(fTimeDelta);
    m_pObject_Manager->Clear_DeadObject();

    m_pLevel_Manager->Update(fTimeDelta);

}

HRESULT CGameInstance::Draw()
{
    m_pRenderer->Render();

    m_pLevel_Manager->Render();

    return S_OK;
}

#pragma region Graphic_Device
void CGameInstance::Render_Begin(_float* Color)
{
    m_pGraphic_Device->Render_Begin(Color);
}

void CGameInstance::Render_End()
{
    m_pGraphic_Device->Render_End();
}
#pragma endregion

#pragma region Level_Manager
HRESULT CGameInstance::Change_Level(CLevel* pNewLevel)
{
    return m_pLevel_Manager->Change_Level(pNewLevel);
}

CLevel* CGameInstance::GetCurrentLevel()
{
    return m_pLevel_Manager->GetCurrentLevel();
}
#pragma endregion

#pragma region Object_Manager
HRESULT CGameInstance::Add_GameObject_ToLayer(_uInt iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uInt iLayerLevelIndex, const _wstring& strLayerTag, void* pArg)
{
    return m_pObject_Manager->Add_GameObject_ToLayer(iPrototypeLevelIndex, strPrototypeTag, iLayerLevelIndex, strLayerTag, pArg);
}
const list<CGameObject*>* CGameInstance::GetAllObejctToLayer(_uInt iLayerIndex, const _wstring& LayerTag)
{
    return m_pObject_Manager->GetAllObejctToLayer(iLayerIndex, LayerTag);
}

void CGameInstance::Clear_DeadObject()
{
    m_pObject_Manager->Clear_DeadObject();
}
#pragma endregion

#pragma region Prototype_Manager
HRESULT CGameInstance::Add_Prototype(_uInt iLevelNum, const _wstring& PrototypeTag, CBase* pPrototype)
{
    return m_pPrototype_Manager->Add_Prototype(iLevelNum, PrototypeTag, pPrototype);
}
#pragma endregion

void CGameInstance::Clear_Resource(_uInt iLevelIndex)
{
    m_pObject_Manager->Clear_Resource(iLevelIndex);
    m_pPrototype_Manager->Clear_Resource(iLevelIndex);
}

#pragma region Timer_Manager
HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
    return m_pTimer_Manager->Add_Timer(strTimerTag);
}

_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
    return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}
#pragma endregion

HRESULT CGameInstance::Add_RenderGroup(RENDER eRenderGroup, CGameObject* pRenderObject)
{
    return m_pRenderer->Add_RenderGroup(eRenderGroup, pRenderObject);
}

#pragma region Input Manager
_bool CGameInstance::KeyDown(KEY_INPUT eType, _uInt KeyState)
{
    return m_pInput_Manager->KeyDown(eType, KeyState);
}

_bool CGameInstance::KeyPressed(KEY_INPUT eType, _uInt KeyState)
{
    return m_pInput_Manager->KeyPressed(eType, KeyState);
}

_bool CGameInstance::KeyUp(KEY_INPUT eType, _uInt KeyState)
{
    return m_pInput_Manager->KeyUp(eType, KeyState);
}

LONG CGameInstance::GetMouseAxis(_uInt iAxis)
{
    return m_pInput_Manager->GetMouseAxis(iAxis);
}

#pragma endregion

#pragma region Sound Manager
void CGameInstance::Manager_PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
    m_pSound_Manager->Manager_PlaySound(pSoundKey, eID, fVolume);
}

void CGameInstance::Manager_PlayBGM(const TCHAR* pSoundKey, float fVolume)
{
    m_pSound_Manager->Manager_PlayBGM(pSoundKey, fVolume);
}

void CGameInstance::Manager_StopSound(CHANNELID eID)
{
    m_pSound_Manager->Manager_StopSound(eID);
}

void CGameInstance::Manager_StopAll()
{
    m_pSound_Manager->Manager_StopAll();
}

void CGameInstance::Manager_SetChannelVolume(CHANNELID eID, float fVolume)
{
    m_pSound_Manager->Manager_SetChannelVolume(eID, fVolume);
}
#pragma endregion


void CGameInstance::Release_Engine()
{
    DestroyInstance();

    Safe_Release(m_pTimer_Manager);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pPrototype_Manager);
    Safe_Release(m_pObject_Manager);
    Safe_Release(m_pLevel_Manager);
    Safe_Release(m_pInput_Manager);
    Safe_Release(m_pSound_Manager);
    Safe_Release(m_pGraphic_Device);
}

void CGameInstance::Free()
{

}
