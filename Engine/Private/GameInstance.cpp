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
#include "Pipeline.h"
#include "LightManager.h"
#include "FontManager.h"
#include "CollisionManager.h"
#pragma endregion

#include "Level.h"

IMPLEMENT_SINGLETON(CGameInstance);

HRESULT CGameInstance::Initialize_Engine(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    GAMEINSTANCE_DESC* GameSetting = static_cast<GAMEINSTANCE_DESC*>(pArg);

    m_ScreenSize = { (_float)GameSetting->EngineSetting.iWinSizeX, (_float)GameSetting->EngineSetting.iWinSizeY };

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

    m_pPipeline = CPipeline::Create(*GameSetting->ppDevice, *GameSetting->ppContext);
    if (nullptr == m_pPipeline)
        return E_FAIL;

    m_pLightManager = CLightManager::Create();
    if (nullptr == m_pLightManager)
        return E_FAIL;

    m_pFontManager = CFontManager::Create(*GameSetting->ppDevice, *GameSetting->ppContext);
    if (nullptr == m_pFontManager)
        return E_FAIL;

    m_pCollisionManager = CCollisionManager::Create();
    if (nullptr == m_pCollisionManager)
        return E_FAIL;

#ifdef _DEBUG
    m_pTimer_Manager->Add_Timer(TEXT("PriorityUpdate_Loop"));
    m_pTimer_Manager->Add_Timer(TEXT("Update_Loop"));
    m_pTimer_Manager->Add_Timer(TEXT("LateUpdate_Loop"));
    m_pTimer_Manager->Add_Timer(TEXT("Collision_Loop"));
    m_pTimer_Manager->Add_Timer(TEXT("Render_Loop"));

#endif // _DEBUG

    return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
    _float Time = fTimeDelta;
    m_pInput_Manager->UpdateKeyFrame();

    m_pMouse->Update(fTimeDelta);

    if (m_bIsPause)
        fTimeDelta = 0.f;

#ifdef _DEBUG
    m_pTimer_Manager->Get_TimeDelta(TEXT("PriorityUpdate_Loop"));
#endif // _DEBUG

    m_pObject_Manager->Priority_Update(fTimeDelta);

#ifdef _DEBUG
    m_pTimer_Manager->Get_TimeDelta(TEXT("Update_Loop"));
#endif // _DEBUG
    m_pObject_Manager->Update(fTimeDelta);
   
#ifdef _DEBUG
    m_pTimer_Manager->Get_TimeDelta(TEXT("LateUpdate_Loop"));
#endif // _DEBUG
    m_pObject_Manager->Late_Update(fTimeDelta);

#ifdef _DEBUG
    m_pTimer_Manager->Get_TimeDelta(TEXT("Collision_Loop"));
#endif // _DEBUG

    m_pCollisionManager->Compute_Collision();

    m_pObject_Manager->Clear_DeadObject();

    m_pLevel_Manager->Update(Time);

}

HRESULT CGameInstance::Draw()
{
#ifdef _DEBUG
    m_pTimer_Manager->Get_TimeDelta(TEXT("Render_Loop"));
#endif // _DEBUG

    m_pRenderer->Render();

    m_pLevel_Manager->Render();

    m_pMouse->Render();

    return S_OK;
}

HRESULT CGameInstance::DrawRender()
{
    m_pRenderer->Render();
    m_pLevel_Manager->Render();

    return S_OK;
}

#pragma region Graphic_Device
HRESULT CGameInstance::ADD_Window(const ENGINE_DESC& Win_Desc)
{
    return m_pGraphic_Device->ADD_Window(Win_Desc);
}

void CGameInstance::Set_RenderResource(_uInt iIndex)
{
    m_pGraphic_Device->Set_RenderResource(iIndex);
}

HRESULT CGameInstance::GetBackBuffer(ID3D11Texture2D** ppOut)
{
    return m_pGraphic_Device->GetBackBuffer(ppOut);
}

_uInt CGameInstance::GetRenderTargetNum()
{
    return m_pGraphic_Device->GetRenderTargetNum();
}

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
HRESULT CGameInstance::Add_GameObject_ToLayer(_uInt iLayerLevelIndex, const _wstring& strLayerTag, CGameObject* pObject)
{
    return m_pObject_Manager->Add_GameObject_ToLayer(iLayerLevelIndex, strLayerTag, pObject);
}
const list<CGameObject*>* CGameInstance::GetAllObejctToLayer(_uInt iLayerIndex, const _wstring& LayerTag)
{
    return m_pObject_Manager->GetAllObejctToLayer(iLayerIndex, LayerTag);
}

void CGameInstance::Clear_DeadObject()
{
    m_pObject_Manager->Clear_DeadObject();
}

const unordered_map<_wstring, CLayer*>* CGameInstance::GetCurLevelLayer()
{
    return m_pObject_Manager->GetLayer(m_pLevel_Manager->GetCurrentLevel()->GetLevelID());
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

CBase* CGameInstance::Clone_Prototype(OBJECT_ID eType, _uInt iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
    return m_pPrototype_Manager->Clone_Prototype(eType, iLevelIndex, strPrototypeTag, pArg);
}

#pragma region Timer_Manager
void CGameInstance::GetPrototypeName(const char* classTypeName, _string& OutName)
{
    m_pPrototype_Manager->GetPrototypeName(classTypeName, OutName);
}

_uInt CGameInstance::GetPrototypeLevel(const _wstring& PrototypeName)
{
    return m_pPrototype_Manager->GetPrototypeLevel(PrototypeName);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
    return m_pTimer_Manager->Add_Timer(strTimerTag);
}

_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag, _bool bFlag)
{
    return m_pTimer_Manager->Get_TimeDelta(strTimerTag, bFlag);
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
void CGameInstance::ShowInGameMouse(VISIBILITY eVisible)
{
    m_pMouse->SetVisibility(eVisible);
}

HRESULT CGameInstance::SetMouseTexture(_uInt iLevelIndex, const WCHAR* Proto_TexTag, const WCHAR* ComTex_Tag, void* pTexArg, const WCHAR* Proto_ShaderTag, const WCHAR* ComShader_Tag, void* pShaderArg, const WCHAR* Proto_BufferTag, const WCHAR* ComBuffer_Tag, void* pBufferArg)
{
    return m_pMouse->SetTexture(iLevelIndex, Proto_TexTag, ComTex_Tag, pTexArg, Proto_ShaderTag, ComShader_Tag, pShaderArg, Proto_BufferTag, ComBuffer_Tag, pBufferArg);
}

void CGameInstance::SetMousePosition(_float3 vPos)
{
    m_pMouse->SetLocation(vPos);
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

_float3& CGameInstance::GetRayPos(RAY eType)
{
    return m_pMouse->GetRayPos(eType);
}

_float3& CGameInstance::GetRayDireaction(RAY eType)
{
    return m_pMouse->GetRayDireaction(eType);
}

void CGameInstance::Compute_LocalRay(const _matrix* InvWorldMatrix)
{
    m_pMouse->Compute_LocalRay(InvWorldMatrix);
}

_bool CGameInstance::RayPicking(_vector vRayOrizin, _vector vRayDir, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
    return m_pMouse->RayPicking(vRayOrizin, vRayDir, vPointA, vPointB, vPointC, pOut);
}

_bool CGameInstance::Picking_InWorld(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
    return m_pMouse->Picking_InWorld(vPointA, vPointB, vPointC, pOut);
}
_bool CGameInstance::Picking_InLocal(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
    return m_pMouse->Picking_InLocal(vPointA, vPointB, vPointC, pOut);
}

_float3 CGameInstance::GetMouseWorldPos()
{
    return m_pMouse->GetMouseWorldPoint();
}

void CGameInstance::ResetMouseData()
{
    m_pMouse->SetMouseFocus(nullptr);
    m_pMouse->SetDrag(false);
}
#pragma endregion

#pragma region PICKING


#pragma endregion

#pragma region PipeLine
void CGameInstance::SetMatrix(MAT_STATE eState, _float4x4 Matrix)
{
    m_pPipeline->SetMatrix(eState, Matrix);
}
void CGameInstance::SetCameraInfo(const _float4& pCmaeraInfo)
{
    m_pPipeline->SetCameraInfo(pCmaeraInfo);
}
const _float4& CGameInstance::GetCameraINFO()
{
    return m_pPipeline->GetCameraINFO();
}
const _float4x4& CGameInstance::GetMatrix(MAT_STATE eState)
{
    return m_pPipeline->GetMatrix(eState);
}
const _float4x4& CGameInstance::GetInvMatrix(MAT_STATE eState)
{
    return m_pPipeline->GetInvMatrix(eState);
}
const _float4x4& CGameInstance::GetIndentityMatrix()
{
    return m_pPipeline->GetIndentityMatrix();
}
void CGameInstance::SetPlayerWorldMatrix(_float4x4 Matrix)
{
    return m_pPipeline->SetPlayerWorldMatrix(Matrix);
}
void CGameInstance::SetPostBuffer(_uInt iIndex, ID3D11ShaderResourceView* pTex)
{
    m_pPipeline->SetPostBuffer(iIndex, pTex);
}

ID3D11ShaderResourceView* CGameInstance::GetPostBuffer(_uInt iIndex)
{
    return m_pPipeline->GetPostBuffer(iIndex);
}
_vector CGameInstance::GetCameraState(WORLDSTATE eType)
{
    return m_pPipeline->GetCameraState(eType);
}
_vector CGameInstance::GetPlayerState(WORLDSTATE eType)
{
    return m_pPipeline->GetPlayerState(eType);
}
#pragma endregion

#pragma region GAME_INSTANCE
const _float2& CGameInstance::GetScreenSize()
{
    return m_ScreenSize;
}
void CGameInstance::SetGameMode(GAMEMODE eMode)
{
    m_eGameMode = eMode;
}
void CGameInstance::GetGamePause(_bool bFlag)
{
    m_bIsPause = bFlag;
}
_float CGameInstance::Random_Normal()
{
    return static_cast<_float>(rand()) / RAND_MAX;
}
_float CGameInstance::Random(_float fMin, _float fMax)
{
    return fMin + Random_Normal() * (fMax - fMin);
}

_float CGameInstance::GetPipeLineLoopTime(const TCHAR* Str)
{
    return m_pTimer_Manager->Get_TimeDelta(Str);
}
#pragma endregion

#pragma region LIGHT_MANAGER

void CGameInstance::ADDLight(CLight* pLight)
{
    m_pLightManager->ADDLight(pLight);
}
const CLight* CGameInstance::GetLight(_uInt iIndex)
{
    return m_pLightManager->GetLight(iIndex);
}

#pragma endregion

#pragma region COLLISION_MANAGER
void CGameInstance::ADD_CollisionList(CCollision* pObject)
{
    m_pCollisionManager->ADD_CollisionList(pObject);
}

#pragma endregion

#pragma region FONT MANAGER
HRESULT CGameInstance::Add_Font(const _wstring& FontTag, const _tchar* pFontFilePath)
{
    return m_pFontManager->Add_Font(FontTag, pFontFilePath);
}

HRESULT CGameInstance::Render_Font(const _wstring& FontTag, const _tchar* pText, const _float2& vPosition, _vector vColor)
{
    return m_pFontManager->Render(FontTag, pText, vPosition, vColor);
}

#pragma endregion

void CGameInstance::Release_Engine()
{
    DestroyInstance();
   
    Safe_Release(m_pTimer_Manager);
    Safe_Release(m_pCollisionManager);
    Safe_Release(m_pFontManager);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pPrototype_Manager);
    Safe_Release(m_pObject_Manager);
    Safe_Release(m_pLevel_Manager);
    Safe_Release(m_pInput_Manager);
    Safe_Release(m_pSound_Manager);
    Safe_Release(m_pMouse);
    Safe_Release(m_pLightManager);
    Safe_Release(m_pPipeline);
    Safe_Release(m_pGraphic_Device);

}

void CGameInstance::Free()
{

}
