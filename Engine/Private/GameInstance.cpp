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
#include "Mouse.h"
#pragma endregion

#include "Level.h"

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

    m_pMouse = CMouse::Create(*GameSetting->ppDevice, *GameSetting->ppContext, GameSetting->EngineSetting.hWnd);
    if (nullptr == m_pMouse)
        return E_FAIL;

    return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
    m_pInput_Manager->UpdateKeyFrame();

    m_pMouse->Update(fTimeDelta);

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

    m_pMouse->Render();

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
CHeadUpDisplay* CGameInstance::GetCurrentHUD()
{
    return m_pLevel_Manager->GetCurrentLevel()->GetHUD();
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
    ResetMouseData();
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

#pragma region Mouse
HRESULT CGameInstance::SetMouseTexture(_uInt iLevelIndex, const WCHAR* Proto_TexTag, const WCHAR* ComTex_Tag, void* pTexArg, const WCHAR* Proto_ShaderTag, const WCHAR* ComShader_Tag, void* pShaderArg, const WCHAR* Proto_BufferTag, const WCHAR* ComBuffer_Tag, void* pBufferArg)
{
    return m_pMouse->SetTexture(iLevelIndex, Proto_TexTag, ComTex_Tag, pTexArg, Proto_ShaderTag, ComShader_Tag, pShaderArg, Proto_BufferTag, ComBuffer_Tag, pBufferArg);
}

POINT& CGameInstance::GetMousePoint()
{
    return m_pMouse->GetMousePoint();
}

void CGameInstance::SetDrag(_bool flag)
{
    m_pMouse->SetDrag(flag);
}

BOOL CGameInstance::IsMouseDrag()
{
    return m_pMouse->IsDrag();
}

void CGameInstance::SetMouseFocus(CUserInterface* Widget)
{
    m_pMouse->SetMouseFocus(Widget);
}

BOOL CGameInstance::IsMouseFocus(CUserInterface* Widget)
{
    return m_pMouse->IsFocus(Widget);
}

_float3& CGameInstance::GetRayPos(RAY eRayState)
{
    return m_pMouse->GetRayPos(eRayState);
}

void CGameInstance::ResetMouseData()
{
    m_pMouse->SetMouseFocus(nullptr);
    m_pMouse->SetDrag(false);
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
    Safe_Release(m_pMouse);
    Safe_Release(m_pGraphic_Device);
}

void CGameInstance::Free()
{

}
